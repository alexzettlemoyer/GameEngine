#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include "../GameRunner/GameRunner.h"

// 60 fps
enum InputType { UP, DOWN, LEFT, RIGHT, HALF, REAL, DOUBLE, PAUSE, CLOSE, NONE };
std::mutex reqMutex;

/**
 * request-reply socket thread function
 * thread is started in the main function that runs the requestSocket function
 *
 * checks for input, which is handled by the main thread
 *  - if there is input (an event or key press)
 *  - format the input using getInputString
 *  - send the request to the server via the reqSocket
 *  - wait for a reply from the server confirming they received the request
 *
 * params:
 *  - reqSocket, the rep-req socket to send requests to
 *  - clientId, this clients' id assigned by the server on first connection
 *  - event, a reference to the event variable which stores any input found in the main thread
 * 
 */
void requestSocket(zmq::socket_t& reqSocket, std::string clientId, std::list<InputType>& events)
{
    bool open = true;
    while (open)
    {
        if (events.size() > 0)
        {
            std::string inputData = clientId;
            {
                std::lock_guard<std::mutex> lock(reqMutex);
                for (InputType const& i: events)
                {
                    inputData += " " + std::to_string(i);
                }

                // check if any of the inputs are WINDOW CLOSE
                auto it = std::find(events.begin(), events.end(), CLOSE);
                if (it != events.end())
                    open = false;

                events.clear();
            }

                // send input request
            zmq::message_t inputRequest(inputData.data(), inputData.size());
            reqSocket.send(inputRequest, zmq::send_flags::none);

                // wait to confirm the client received the request
            zmq::message_t confirm;
            zmq::recv_result_t result = reqSocket.recv(confirm, zmq::recv_flags::none);
        }
    }
}

/**
 * handles keyboard input from the user
 * checks for keypresses: up/space, down, left, right
 * polls the window for keypressed events: close, P, 1, 2, 3
 *
 * parameter: the window to poll for events
 * returns: the InputType found, or NONE
 */
std::list<InputType> handleInput(sf::RenderWindow *window)
{
    std::list<InputType> list;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        list.push_back(UP);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        list.push_back(DOWN);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        list.push_back(LEFT);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        list.push_back(RIGHT);

    sf::Event localEvent;
    while (window -> pollEvent(localEvent))
    {
        if (localEvent.type == sf::Event::Closed)
        {
            window -> close();
            list.push_back(CLOSE);
        }
        else if (localEvent.type == sf::Event::KeyPressed)
        {
            switch (localEvent.key.code)
            {
                case sf::Keyboard::Key::P:
                    std::cout << "P" << std::endl;
                    list.push_back(PAUSE);
                    break;
                case sf::Keyboard::Key::Num1:
                    list.push_back(HALF);
                    break;
                case sf::Keyboard::Key::Num2:
                    list.push_back(REAL);
                    break;
                case sf::Keyboard::Key::Num3:
                    list.push_back(DOUBLE);
                    break;
            }
        }
    }
    list.unique();
    return list;
}

int main ()
{
    std::list<InputType> events;

    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t reqSocket(context, zmq::socket_type::req);

    std::cout << "Connecting to server..." << std::endl;
    reqSocket.connect("tcp://localhost:5555");

    zmq::socket_t subSocket(context, zmq::socket_type::sub);
    subSocket.set(zmq::sockopt::conflate, 1);       // CONFLATE OPTION FOR SUB SOCKET

    subSocket.connect("tcp://localhost:5556");
    subSocket.set(zmq::sockopt::subscribe, "");

        // send inital request
    zmq::message_t request(3);
    memcpy(request.data (), "REQ", 3);
    reqSocket.send(request, zmq::send_flags::none);

        // receive the client id assigned by the server
    zmq::message_t reply;
    zmq::recv_result_t result = reqSocket.recv(reply);

        // get the connection number / client id from the reply
    int connectionNumber;
    memcpy(&connectionNumber, reply.data(), sizeof(int));
    std::string clientId = std::to_string(connectionNumber);

    std::cout << "id" << clientId << std::endl;

        // start the game window!
    GameRunner *game = GameRunner::getInstance(std::stoi(clientId));
    game -> drawGraphics();

    // start thread for io handling
    std::thread reqThread(requestSocket, std::ref(reqSocket), clientId, std::ref(events));

        // wait for server messages
    while (game -> getWindow() -> isOpen())
    {
        zmq::message_t reply;
        zmq::recv_result_t result = subSocket.recv(reply, zmq::recv_flags::none);

        std::string data = std::string(static_cast<char*>(reply.data()), reply.size());

        game -> deserialize(data);
        game -> checkWindowScroll();
        game -> drawGraphics();

            // handle input
        if ( game -> getWindow() -> hasFocus())
        {
            std::list inputList = handleInput(game -> getWindow());
            {
                std::lock_guard<std::mutex> lock(reqMutex);
                events = inputList;
            }
        }
    }
    
    reqThread.join();
    return 0;
}

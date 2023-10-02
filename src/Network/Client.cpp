#include <zmq.hpp>
#include <string>
#include <iostream>
#include "../GameRunner/GameRunner.h"

enum InputType { UP, DOWN, LEFT, RIGHT, HALF, REAL, DOUBLE, PAUSE, CLOSE, NONE };

/**
 * handles keyboard input from the user
 * checks for keypresses: up/space, down, left, right
 * polls the window for keypressed events: close, P, 1, 2, 3
 *
 * parameter: the window to poll for events
 * returns: the InputType found, or NONE
 */
InputType handleInput(sf::RenderWindow *window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        return UP;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        return DOWN;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        return LEFT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        return RIGHT;

    sf::Event event;
    while (window -> pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window -> close();
            return CLOSE;
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            // std::cout << event.key.code << std::endl;
            switch (event.key.code)
            {
                case sf::Keyboard::Key::P:
                    return PAUSE;
                    break;
                case sf::Keyboard::Key::Num1:
                    return HALF;
                    break;
                case sf::Keyboard::Key::Num2:
                    return REAL;
                    break;
                case sf::Keyboard::Key::Num3:
                    return DOUBLE;
                    break;
            }
        }
    }
    return NONE;
}

/**
 * creates the input string to send to the client
 *
 * params: this clients' ID (the characters' id on the server), and the event found
 * returns: the formatted input string
 */
std::string getInputString(std::string clientID, InputType event)
{
    return "" + clientID + " " + std::to_string(event);
}

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t reqSocket(context, zmq::socket_type::req);

    std::cout << "Connecting to server..." << std::endl;
    reqSocket.connect("tcp://localhost:5555");

    zmq::socket_t subSocket(context, zmq::socket_type::sub);
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
    std::string clientID = std::to_string(connectionNumber);

        // start the game window!
    GameRunner *game = GameRunner::getInstance();
    game -> drawGraphics();

    // start thread for io handling ?

        // wait for server messages
    while (game -> getWindow() -> isOpen())
    {
        zmq::message_t reply;
        zmq::recv_result_t result = subSocket.recv(reply, zmq::recv_flags::none);

        std::string data = std::string(static_cast<char*>(reply.data()), reply.size());

        game -> deserialize(data);
        game -> drawGraphics();

            // handle input
        InputType event = handleInput(game -> getWindow());

            // if there is some input, send a request to the client
        if ( event != NONE )
        {
                // send input request
            std::string inputData = getInputString(clientID, event);
            zmq::message_t inputRequest(inputData.data(), inputData.size());
            reqSocket.send(inputRequest, zmq::send_flags::none);

                // wait to confirm the client received the request
            zmq::message_t confirm;
            result = reqSocket.recv(confirm, zmq::recv_flags::none);
        }
    }
    return 0;
}

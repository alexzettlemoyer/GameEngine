#include <zmq.hpp>
#include <string>
#include <iostream>
#include "../GameRunner/GameRunner.h"

enum InputType { UP, DOWN, LEFT, RIGHT, HALF, REAL, DOUBLE, PAUSE, CLOSE, NONE };

InputType handleInput()
{
    // std::cout << "HANDLE" << std::endl;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        return UP;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        return DOWN;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        return LEFT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        return RIGHT;

    return NONE;
}

InputType pollWindowEvent(sf::RenderWindow *window)
{
    // std::cout << "POLL EVENT" << std::endl;
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

std::string getInputString(std::string clientID, InputType event, InputType keys)
{
    return "" + clientID + " " + std::to_string(event) + " " + std::to_string(keys);
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

    // start thread for io handling

        // wait for server messages
    while (game -> getWindow() -> isOpen())
    {
        zmq::message_t reply;
        zmq::recv_result_t result = subSocket.recv(reply, zmq::recv_flags::none);

        std::string data = std::string(static_cast<char*>(reply.data()), reply.size());
        std::cout << data << std::endl;

        game -> deserialize(data);
        game -> drawGraphics();

            // handle input
        InputType events = pollWindowEvent(game -> getWindow());
        InputType keys = handleInput();

        if ( events == CLOSE )
            break;

            // if there is some input, send a request to the client
        if ( events != NONE && keys != NONE )
        {
            std::cout << "INPUT" << std::endl;
                // send input request
            std::string inputData = getInputString(clientID, events, keys);
            zmq::message_t inputRequest(inputData.data(), inputData.size());
            reqSocket.send(inputRequest, zmq::send_flags::none);

                // wait to confirm the client received the request
            zmq::message_t confirm;
            result = reqSocket.recv(confirm, zmq::recv_flags::none);
        }
    }
    return 0;
}

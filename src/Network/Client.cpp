#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <sstream>
#include "../GameRunner/GameRunner.h"
#include "../GameRunner/ClientGameState.h"
#include "../Time/Timeline.h"
#include "../Events/Event.h"
#include "../Events/EventHandler.h"

// 60 fps
enum InputType { HALF, REAL, DOUBLE, PAUSE, CLOSE };
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
        std::stringstream inputData;
        inputData << clientId << " ";
        {
            std::lock_guard<std::mutex> lock(reqMutex);

            sf::Vector2f charPosition = ClientGameState::getInstance() -> getCharacterPosition();
            inputData << "[ " << charPosition.x << " " << charPosition.y << " ]";

            for (InputType const& i: events)
            {
                inputData << " " + std::to_string(i);
            }

            // check if any of the inputs are WINDOW CLOSE
            auto it = std::find(events.begin(), events.end(), CLOSE);
            if (it != events.end())
                open = false;

            events.clear();
        }

        std::string inputString = inputData.str();
            // send input request
        zmq::message_t inputRequest(inputString.data(), inputString.size());
        reqSocket.send(inputRequest, zmq::send_flags::none);

            // wait to confirm the client received the request
        zmq::message_t confirm;
        zmq::recv_result_t result = reqSocket.recv(confirm, zmq::recv_flags::none);
    }
}

std::list<InputType> handleInputToServer(sf::RenderWindow *window)
{
    std::list<InputType> list;
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

/**
 * handles keyboard input from the user
 * checks for keypresses: up/space, down, left, right
 * polls the window for keypressed events: close, P, 1, 2, 3
 *
 * parameter: the window to poll for events
 * returns: the InputType found, or NONE
 */
void handleInput(EventHandler* eventHandler, Timeline* t)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        std::shared_ptr<Event> e = std::make_shared<Event>(Event::C_UP, t -> getTimeStamp()); 
        e -> addCharacterVariant(ClientGameState::getInstance() -> getCharacter().get());
        e -> addMetaData("Client from input up");
        eventHandler -> onEvent(e);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        std::shared_ptr<Event> e = std::make_shared<Event>(Event::C_DOWN, t -> getTimeStamp()); 
        e -> addCharacterVariant(ClientGameState::getInstance() -> getCharacter().get());
        e -> addMetaData("Client from input down");

        eventHandler -> onEvent(e);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        std::shared_ptr<Event> e = std::make_shared<Event>(Event::C_LEFT, t -> getTimeStamp()); 
        e -> addCharacterVariant(ClientGameState::getInstance() -> getCharacter().get());
        e -> addMetaData("Client from input left");

        eventHandler -> onEvent(e);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        std::shared_ptr<Event> e = std::make_shared<Event>(Event::C_RIGHT, t -> getTimeStamp()); 
        e -> addCharacterVariant(ClientGameState::getInstance() -> getCharacter().get());
        e -> addMetaData("Client from input right");

        eventHandler -> onEvent(e);    
    }
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

    Timeline* t = new Timeline();
    EventHandler* eventHandler = EventHandler::getInstance( t );

    // start thread for io handling
    std::thread reqThread(requestSocket, std::ref(reqSocket), clientId, std::ref(events));

    ClientGameState *gameState = ClientGameState::getInstance();

        // wait for server messages
    while (game -> getWindow() -> isOpen())
    {
        zmq::message_t publishedMsg;
        zmq::recv_result_t result = subSocket.recv(publishedMsg, zmq::recv_flags::none);
        std::string data = std::string(static_cast<char*>(publishedMsg.data()), publishedMsg.size());

        if ( !data.empty() )
            game -> deserialize(data);

        // move character
        gameState -> updateGameState();
        game -> drawGraphics();

            // handle input
        if ( game -> getWindow() -> hasFocus())
        {
            handleInput(eventHandler, t);
            std::list inputList = handleInputToServer(game -> getWindow());
            {
                std::lock_guard<std::mutex> lock(reqMutex);
                events = inputList;
            }
        }
    }
    reqThread.join();

    delete game;
    delete gameState;
    delete eventHandler;
    delete t;
    return 0;
}

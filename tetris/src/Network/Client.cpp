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
#include "../Scripting/ScriptManager.h"
#include <v8/v8.h>
#include <libplatform/libplatform.h>

// 60 fps
enum InputType { PAUSE, CLOSE, NEW_PIECE };
std::mutex reqMutex;
v8::Isolate *isolate;
std::shared_ptr<ScriptManager> scriptManager;

bool upKeyPressed = false;
int consecutiveUpKeyPresses = 0;
sf::Clock triplePressClock;
const sf::Time triplePressThreshold = sf::milliseconds(350);

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

            if ( ClientGameState::getInstance() -> newPieces.size() < 3 )
                events.push_back( NEW_PIECE );

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
        zmq::message_t response;
        zmq::recv_result_t result = reqSocket.recv(response, zmq::recv_flags::none);
        
        std::string message = std::string(static_cast<char*>(response.data()), response.size());
        if (message != "R")
            ClientGameState::getInstance() -> newPieces.push( message );
    }
}

std::list<InputType> handleInputToServer(sf::RenderWindow *window, EventHandler* eventHandler, Timeline* t, std::shared_ptr<ScriptManager> scriptManager)
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
                    scriptManager -> runOne("raise_pause_event", true, "object_context");
                    break;
                case sf::Keyboard::Key::Up:
                    scriptManager -> runOne("raise_rotate_event", true, "object_context"); 
                    break;
                case sf::Keyboard::Key::Down:
                    scriptManager -> runOne("raise_down_event", true, "object_context"); 
                    break;
                case sf::Keyboard::Key::Left:
                    scriptManager -> runOne("raise_left_event", true, "object_context"); 
                    break;
                case sf::Keyboard::Key::Right:
                    scriptManager -> runOne("raise_right_event", true, "object_context"); 
                    break;
            }
        }
        else if (localEvent.type == sf::Event::MouseButtonReleased) {
            if (localEvent.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2f mousePos = window -> mapPixelToCoords(sf::Mouse::getPosition());
                sf::FloatRect bounds = GameRunner::getInstance() -> getStartText().getGlobalBounds();
                bounds.height += 25;

                if (bounds.contains(mousePos))
                    scriptManager -> runOne("raise_game_start_event", true, "object_context");
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
// void handleInput(EventHandler* eventHandler, Timeline* t, std::shared_ptr<ScriptManager> scriptManager)
// {
//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
//         scriptManager -> runOne("raise_rotate_event", true, "object_context"); 

//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
//         scriptManager -> runOne("raise_down_event", true, "object_context"); 

//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
//         scriptManager -> runOne("raise_left_event", true, "object_context"); 

//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
//         scriptManager -> runOne("raise_right_event", true, "object_context"); 
// }

int main ()
{
    std::list<InputType> events;

    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t reqSocket(context, zmq::socket_type::req);

    std::cout << "Connecting to server..." << std::endl;
    reqSocket.connect("tcp://localhost:5555");

    zmq::socket_t subSocket(context, zmq::socket_type::sub);
    // subSocket.set(zmq::sockopt::conflate, 1);       // CONFLATE OPTION FOR SUB SOCKET
    subSocket.setsockopt(ZMQ_CONFLATE, 1);

    subSocket.connect("tcp://localhost:5556");
    subSocket.setsockopt(ZMQ_SUBSCRIBE, "", 0);

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

    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.release());
    v8::V8::InitializeICU();
    v8::V8::Initialize();
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = v8::Isolate::New(create_params);

    { // anonymous scope for managing handle scope
        v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
        v8::HandleScope handle_scope(isolate);

		// Best practice to isntall all global functions in the context ahead of time.
        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
        // Bind the global 'print' function to the C++ Print callback.
        global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
		global->Set(isolate, "eventobjectfactory", v8::FunctionTemplate::New(isolate, Event::ScriptedEventFactory));
        global->Set(isolate, "eventhandler_raise", v8::FunctionTemplate::New(isolate, EventHandler::ScriptedRaiseEvent));

        v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
		v8::Context::Scope default_context_scope(default_context); // enter the context

        scriptManager = std::make_shared<ScriptManager>(isolate, default_context); 

		// Create a new context
		v8::Local<v8::Context> object_context = v8::Context::New(isolate, NULL, global);
		scriptManager->addContext(isolate, object_context, "object_context");

        scriptManager -> addScript("raise_rotate_event", "src/Scripting/scripts/raise_rotate_event.js", "object_context");
        scriptManager -> addScript("raise_left_event", "src/Scripting/scripts/raise_left_event.js", "object_context");
        scriptManager -> addScript("raise_right_event", "src/Scripting/scripts/raise_right_event.js", "object_context");
        scriptManager -> addScript("raise_down_event", "src/Scripting/scripts/raise_down_event.js", "object_context");
        scriptManager -> addScript("raise_new_piece_event", "src/Scripting/scripts/raise_new_piece_event.js", "object_context");
        scriptManager -> addScript("raise_game_start_event", "src/Scripting/scripts/raise_game_start_event.js", "object_context");
        scriptManager -> addScript("raise_pause_event", "src/Scripting/scripts/raise_pause_event.js", "object_context");


        scriptManager -> addScript("handle_up_event", "src/Scripting/scripts/handle_up_event.js", "object_context");
        scriptManager -> addScript("handle_down_event", "src/Scripting/scripts/handle_down_event.js", "object_context");
        scriptManager -> addScript("handle_left_event", "src/Scripting/scripts/handle_left_event.js", "object_context");
        scriptManager -> addScript("handle_right_event", "src/Scripting/scripts/handle_right_event.js", "object_context");
        scriptManager -> addScript("handle_new_piece_event", "src/Scripting/scripts/handle_new_piece_event.js", "object_context");
        scriptManager -> addScript("handle_game_start_event", "src/Scripting/scripts/handle_game_start_event.js", "object_context");
        scriptManager -> addScript("handle_pause_event", "src/Scripting/scripts/handle_pause_event.js", "object_context");

        scriptManager -> addScript("new_level", "src/Scripting/scripts/new_level.js", "object_context");

        
        t -> exposeToV8(isolate, object_context);
        eventHandler -> addScriptManager(scriptManager);
        eventHandler -> exposeToV8(isolate, object_context);
        gameState -> addScriptManager(scriptManager);
        gameState -> exposeToV8(isolate, object_context);

        // scriptManager -> runOne("handle_new_piece_event", false, "object_context");

            // wait for server messages
        while (game -> getWindow() -> isOpen())
        {
            zmq::message_t publishedMsg;
            zmq::recv_result_t result = subSocket.recv(publishedMsg, zmq::recv_flags::none);
            std::string data = std::string(static_cast<char*>(publishedMsg.data()), publishedMsg.size());

            if ( !data.empty() )
                game -> deserialize(data);
            
            std::shared_ptr<Piece> currentPiece = gameState -> getCurrentPiece();
            if (currentPiece != nullptr )
                currentPiece -> exposeToV8(isolate, object_context);

            // move character
            gameState -> updateGameState();
            game -> drawGraphics();

                // handle input
            if ( game -> getWindow() -> hasFocus())
            {
                // handleInput(eventHandler, t, scriptManager);
                std::list inputList = handleInputToServer(game -> getWindow(), eventHandler, t, scriptManager);
                {
                    std::lock_guard<std::mutex> lock(reqMutex);
                    events = inputList;
                }
            }
        }
    }

    reqThread.join();

    delete game;
    delete gameState;
    delete eventHandler;
    delete t;

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();

    return 0;
}

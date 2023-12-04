#include <SFML/Graphics.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include "../GameRunner/ServerGameState.h"
#include <v8/v8.h>
#include <libplatform/libplatform.h>

std::mutex repMutex;
v8::Isolate *isolate;
std::shared_ptr<ScriptManager> scriptManager;
int id = 0;

void handleDisconnects(std::unordered_map<std::string, int>& connections)
{
    while (true)
    {
        if ( connections.size() > 0)
        {
            std::vector<std::string> clientsToRemove;

            // sleep for 5 seconds
            std::this_thread::sleep_for(std::chrono::seconds(200));

            for ( auto& i : connections )
            {
                if ( i.second == 0 )  // client disconnected
                {
                    clientsToRemove.push_back( i.first );
                }
                i.second = 0;
            }

            // remove any clients with 0 new messages
            for ( std::string key : clientsToRemove )
            {
                std::cout << "Client " << key << " disconnected." << std::endl;
                ServerGameState::getInstance() -> input( key, "8" );

                std::lock_guard<std::mutex> lock(repMutex);
                connections.erase( key );
            }
        }
    }
}



/**
 * reply-request socket function
 * thread is started in the main function that runs the replySocket function
 * 
 * waits for requests from new clients
 *  - new clients send a "REQ" message
 *  - the gameState makes a new character and assigns it an id
 *  - the reply to the client is its characters' id
 * 
 * waits for requests from existing clients
 *  - takes the input string and decodes it
 *  - calls the gameState's input function to apply the input to the clients' character
 *  - sends a simple reply to the client to confirm the message was received
 *
 * params:
 *  - repSocket, the rep-req socket to wait for requests from
 *  - connections, the map of clients connected
 *  
 */
void replySocket(zmq::socket_t& repSocket, std::unordered_map<std::string, int>& connections)
{
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
        global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));

        v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
		v8::Context::Scope default_context_scope(default_context); // enter the context

        scriptManager = std::make_shared<ScriptManager>(isolate, default_context); 

		// Create a new context
		v8::Local<v8::Context> object_context = v8::Context::New(isolate, NULL, global);
		scriptManager->addContext(isolate, object_context, "object_context");

        // expose objects for selecting the next piece
        v8::Local<v8::Array> pieceSelector = v8::Array::New(isolate);
        v8::Local<v8::String> pieceSelectorName = v8::String::NewFromUtf8(isolate, "piece_selector");

        v8::Local<v8::Value> pieceSelectorValue = v8::Undefined(isolate);
        v8::Local<v8::String> pieceSelectorValueName = v8::String::NewFromUtf8(isolate, "piece_selector_value");

        auto globalObj = object_context->Global();
        auto setResult1 = globalObj->Set(object_context, pieceSelectorName, pieceSelector);
        auto setResult2 = globalObj->Set(object_context, pieceSelectorValueName, pieceSelectorValue);
        v8::Local<v8::Object> instance = global->NewInstance(object_context).ToLocalChecked();
        v8::Local<v8::String> instanceName = v8::String::NewFromUtf8(isolate, "instance");
        auto setResult3 = globalObj->Set(object_context, instanceName, instance);

        scriptManager -> addScript("select_piece", "src/Scripting/scripts/select_piece.js", "object_context");


        while (true)
        {
            //  Wait for next request from connecting client
            zmq::message_t request;
            // assign to a zmq recv_result_t to supress compiler warnings about ignored return
            zmq::recv_result_t result = repSocket.recv(request, zmq::recv_flags::none);

            // if clients have already connected, they start all messages with their clientID
            // which is assigned to them on first connection
            std::string clientMessage = std::string(static_cast<char*>(request.data()), request.size());        
            std::vector<std::string> dataVector = ServerGameState::getInstance() -> split(clientMessage, ' ');
            std::string clientID = dataVector[0];

            ServerGameState* game = ServerGameState::getInstance();

            if (clientID == "REQ" || connections.find(clientID) == connections.end()) 
            {
                // Assign a connection number to the client
                int idNum = id++;
                clientID = std::to_string(idNum);
                {
                    std::lock_guard<std::mutex> lock(repMutex);
                    connections[clientID] = 1;
                }
                std::cout << "Client " << idNum << " connected." << std::endl;

                    // send the client's ID back to them
                zmq::message_t idReply(sizeof(int));
                memcpy(idReply.data (), &idNum, sizeof(int));
                repSocket.send(idReply, zmq::send_flags::none);
            }
            else
            {
                std::string nextPiece = "0";
                {
                    std::lock_guard<std::mutex> lock(repMutex);
                    connections[clientID] += 1;
                }

                for (int i = 1; i < dataVector.size(); i++)
                {
                    if ( dataVector[i] == "0" )
                        game -> input( clientID, dataVector[ i ] );
                    if ( dataVector[ i ] == "1" )
                    {
                        std::cout << "Client " << clientID << " disconnected." << std::endl;
                        std::lock_guard<std::mutex> lock(repMutex);
                        connections.erase( clientID );
                    }
                    if ( dataVector[ i ] == "2" )
                    {
                        // code for getting the next piece
                        scriptManager -> runOne("select_piece", false, "object_context");
                        v8::Local<v8::String> varName = v8::String::NewFromUtf8(isolate, "piece_selector_value");
                        if (!object_context->Global()->Get(object_context, varName).ToLocal(&pieceSelectorValue)) {
                            std::cout << "error" << std::endl;
                        }
                        else if (!pieceSelectorValue->IsUndefined()) {
                            v8::String::Utf8Value utf8(isolate, pieceSelectorValue);
                            nextPiece = *utf8;
                        }
                    }
                }

                std::string response;
                if (nextPiece != "0")
                    response = nextPiece;
                else
                    response = "R";
                
                // tell the client we received their update
                // std::string response = "R";
                zmq::message_t reply(response.data(), response.size());
                repSocket.send(reply, zmq::send_flags::none);
            }
        }
    }
}

int main() 
{
            //  Prepare our context and socket
    zmq::context_t context(2);
    zmq::socket_t repSocket(context, zmq::socket_type::rep);
    repSocket.bind("tcp://*:5555");

    zmq::socket_t pubSocket(context, zmq::socket_type::pub);
    pubSocket.bind("tcp://*:5556");

        // maps client connection number to client loop counter
    std::unordered_map<std::string, int> connections;

        // thread to handle server replies
    std::thread repThread(replySocket, std::ref(repSocket), std::ref(connections));

        // thread to check if any clients disconnected
    std::thread disconnectThread(handleDisconnects, std::ref(connections));

    // std::shared_ptr<ScriptRunner> scriptRunner = std::make_shared<ScriptRunner>();
    ServerGameState* game = ServerGameState::getInstance();

        // Publish loop to all clients
    while(true)
    {
        if ( connections.size() > 0 )
        {
                // update the gameState each iteration
            game -> updateGameState();

            std::string data = game -> serialize();

            zmq::message_t publishData(data.data(), data.size());
            pubSocket.send(publishData, zmq::send_flags::none);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    disconnectThread.join();
    repThread.join();

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete game;
    return 0;
}
#include <SFML/Graphics.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include "../GameRunner/ServerGameState.h"

std::mutex repMutex;

void handleDisconnects(std::unordered_map<std::string, int>& connections)
{
    while (true)
    {
        if ( connections.size() > 0)
        {
            std::vector<std::string> clientsToRemove;

            // sleep for 5 seconds
            std::this_thread::sleep_for(std::chrono::seconds(2));

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
                ServerGameState::getInstance() -> removeObject( stoi(key) );

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
            int idNum = game -> newCharacter();
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
            {
                std::lock_guard<std::mutex> lock(repMutex);
                connections[clientID] += 1;
            }

            game -> updateCharacterPosition(clientID, stof(dataVector[2]), stof(dataVector[3]));

            for (int i = 5; i < dataVector.size(); i++)
            {
                game -> input( clientID, dataVector[ i ] );
                if ( dataVector[ i ] == "8" )
                {
                    std::cout << "Client " << clientID << " disconnected." << std::endl;
                    std::lock_guard<std::mutex> lock(repMutex);
                    connections.erase( clientID );
                }
            }

            // tell the client we received their update
            std::string response = "R";
            zmq::message_t reply(response.data(), response.size());
            repSocket.send(reply, zmq::send_flags::none);
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

    ServerGameState* game = ServerGameState::getInstance();
        // Publish loop to all clients
    while(true)
    {
        if ( connections.size() > 0 )
        {
                // update the gameState each iteration
            game -> updateGameState();
            std::string data = game -> serialize();

            // std::cout << data << std::endl;

            zmq::message_t publishData(data.data(), data.size());
            pubSocket.send(publishData, zmq::send_flags::none);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    disconnectThread.join();
    repThread.join();
    delete game;
    return 0;
}
#include <SFML/Graphics.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include "../GameRunner/ServerGameState.h"

std::mutex repMutex;

void publisherSocket(zmq::socket_t& pubSocket, std::unordered_map<std::string, int>& connections)
{
    while(true)
    {
        if ( connections.size() > 0 )
        {
                // update the gameState each iteration
            ServerGameState::getInstance() -> updateGameState();
            std::string data = ServerGameState::getInstance() -> serialize();

            zmq::message_t publishData(data.data(), data.size());
            pubSocket.send(publishData, zmq::send_flags::none);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void processInput( std::vector<std::string>& dataVector, int clientID)
{
    for (int i = 1; i < dataVector.size(); i++)
        ServerGameState::getInstance() -> input(std::to_string(clientID), dataVector[i]);
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

        // thread to handle the publisher socket
    std::thread publisherThread( [&] () { publisherSocket(pubSocket, connections); });


    std::vector<std::thread> clientThreads;

        // Publish loop to all clients
    while(true)
    {
        zmq::message_t request;
        // assign to a zmq recv_result_t to supress compiler warnings about ignored return
        zmq::recv_result_t result = repSocket.recv(request, zmq::recv_flags::none);
        
        // if clients have already connected, they start all messages with their clientID
        // which is assigned to them on first connection
        std::string clientMessage = std::string(static_cast<char*>(request.data()), request.size());
        std::vector<std::string> dataVector = ServerGameState::getInstance() -> split(clientMessage, ' ');
        std::string clientID = dataVector[0];

        if (clientID == "REQ" || connections.find(clientID) == connections.end()) 
        {
            // Assign a connection number to the client
            int idNum = ServerGameState::getInstance() -> newCharacter();
            clientID = std::to_string(idNum);
            {
                std::lock_guard<std::mutex> lock(repMutex);
                connections[clientID] = 0;
            }
            std::cout << "Client " << idNum << " connected." << std::endl;

                // send the client's ID back to them
            zmq::message_t idReply(sizeof(int));
            memcpy(idReply.data (), &idNum, sizeof(int));
            repSocket.send(idReply, zmq::send_flags::none);
        }
        else
        { 
            std::thread clientThread(processInput, std::ref(dataVector), std::stoi(clientID));

                // tell the client we received their update
            std::string response = "R";
            zmq::message_t reply(response.data(), response.size());
            repSocket.send(reply, zmq::send_flags::none);
            
            clientThread.join();
        }
    }
    publisherThread.join();

    return 0;
}

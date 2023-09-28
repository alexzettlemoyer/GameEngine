#include <SFML/Graphics.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include "../GameRunner/GameState.h"

std::mutex repMutex;

void replySocket(zmq::socket_t& repSocket, std::unordered_map<std::string, int>& connections, int& nextConnection)
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
        std::vector<std::string> dataVector = GameState::split(clientMessage, ' ');
        std::string clientID = dataVector[0];

        if (clientID == "REQ" || connections.find(clientID) == connections.end()) 
        {
            // Assign a connection number to the client
            int idNum = GameState::getInstance() -> newCharacter();
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
                // get any inputs from the original message the clients sent
            if (dataVector.size() > 1) 
            {
                if (dataVector[1] != "9")
                {
                    std::lock_guard<std::mutex> lock(repMutex);
                    GameState::getInstance() -> input(clientID, dataVector[1]);
                }
                if (dataVector[2] != "9")
                {
                    std::lock_guard<std::mutex> lock(repMutex);
                    GameState::getInstance() -> input(clientID, dataVector[2]);
                }
            }

            {
                // update GameState: object movement
                std::lock_guard<std::mutex> lock(repMutex);
                GameState::getInstance() -> updateGameState();
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
        // atomic counter to assign connection numbers
    int nextConnection(1);

    std::thread repThread(replySocket, std::ref(repSocket), std::ref(connections), std::ref(nextConnection));

        // Publish loop to all clients
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // update the gameState each iteration
        GameState::getInstance() -> updateGameState();
        std::string data = GameState::getInstance() -> serialize();

        zmq::message_t iterationReply(data.data(), data.size());
        pubSocket.send(iterationReply, zmq::send_flags::none);
    }
    return 0;
}

#include <SFML/Graphics.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <atomic>
#include <thread>
#include "../GameRunner/GameState.h"

int main() 
{
        //  Prepare our context and socket
    zmq::context_t context(2);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

        // vector of poll items to keep track of multiple clients
    std::vector<zmq::pollitem_t> pollItems;

        // maps client connection number to client loop counter
    std::unordered_map<std::string, int> connections;
        // atomic counter to assign connection numbers
    std::atomic<int> nextConnection(1);

        // put a server item in the poll items list
    zmq::pollitem_t serverItem = {static_cast<void*>(socket), 0, ZMQ_POLLIN, 0};
    pollItems.push_back(serverItem);

        // Start a server process that listens for incoming network connections
    while(true)
    {
            // poll for incoming data on server socket
        int rc = zmq::poll(pollItems.data(), pollItems.size(), std::chrono::milliseconds(-1));
        if (rc < 0) {
            std::cout << "Error in poll" << zmq_strerror(errno) << std::endl;
            break;
        }

            // check for incoming data on the server socket
        if (pollItems[0].revents & ZMQ_POLLIN) 
        {
                //  Wait for next request from client
            zmq::message_t request;
                // assign to a zmq recv_result_t to supress compiler warnings about ignored return
            zmq::recv_result_t result = socket.recv(request, zmq::recv_flags::none);

                // if clients have already connected, they start all messages with their clientID
                // which is assigned to them on first connection
            std::string clientID = std::string(static_cast<char*>(request.data()), request.size());
            int idNum;

                // if this is the clients' first time connecting
            if (clientID == "REQ" || connections.find(clientID) == connections.end()) 
            {
                // Assign a connection number to the client
                idNum = nextConnection.fetch_add(1);
                clientID = std::to_string(idNum);
                connections[clientID] = 0;
                std::cout << "Client " << idNum << " connected." << std::endl;

                    // send the client's ID back to them
                zmq::message_t idReply(sizeof(int));
                memcpy(idReply.data (), &idNum, sizeof(int));
                socket.send(idReply, zmq::send_flags::none);
            }
                // if the client has connected before
            else
            {

                
                // apply any input to the state
                // update the character's movement
                // update the platform's movement

                // GameState *state = GameState::getInstance();
                // zmq::message_t gameState(state -> data(), state -> size());
                // socket.send(gameState, zmq::send_flags::none);

                    //  do some 'work'
                std::this_thread::sleep_for(std::chrono::seconds(1));

                //     // increase the client's iteration number and send their message
                connections[clientID] += 1;
                std::string iterationString = "Client " + clientID + ": Iteration " + std::to_string(connections[clientID]) + "\n";
                zmq::message_t iterationReply(iterationString.data(), iterationString.size());
                socket.send(iterationReply, zmq::send_flags::none);
            }
        }
    }
    return 0;
}
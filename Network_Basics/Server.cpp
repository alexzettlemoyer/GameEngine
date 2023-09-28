#include <zmq.hpp>
#include <string>
#include <iostream>
#include <atomic>
#include <thread>

int main () 
{
        //  Prepare our context and socket
    zmq::context_t context(2);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

        // maps client connection number to client loop counter
    std::unordered_map<std::string, int> connections;
        // atomic counter to assign connection numbers
    std::atomic<int> nextConnection(1);


        // Start a server process that listens for incoming network connections
    while (true) 
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
                //  do some 'work'
            std::this_thread::sleep_for(std::chrono::seconds(1));

            for (auto const& c : connections )
            {
                
            }
                    // increase the client's iteration number and send their message
            connections[clientID] += 1;
            std::string iterationString = "Client " + clientID + ": Iteration " + std::to_string(connections[clientID]) + "\n";
            zmq::message_t iterationReply(iterationString.data(), iterationString.size());
            socket.send(iterationReply, zmq::send_flags::none);
        }
    }
    return 0;
}

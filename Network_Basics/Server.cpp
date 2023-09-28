#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <sstream>
#include <mutex>

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
        std::string clientID = std::string(static_cast<char*>(request.data()), request.size());

        if (clientID == "REQ" || connections.find(clientID) == connections.end()) 
        {
            // Assign a connection number to the client
            int idNum = nextConnection++;
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
    }
}


int main () 
{
        //  Prepare our context and socket
    zmq::context_t context(2);
    zmq::socket_t repSocket(context, zmq::socket_type::rep);
    repSocket.bind("tcp://*:5555");             // replier port

    zmq::socket_t pubSocket(context, zmq::socket_type::pub);
    pubSocket.bind("tcp://*:5556");             // publisher port

        // maps client connection number to client loop counter
    std::unordered_map<std::string, int> connections;
        // atomic counter to assign connection numbers
    int nextConnection = 1;

    // START REPLY THREAD HERE
    std::thread repThread(replySocket, std::ref(repSocket), std::ref(connections), std::ref(nextConnection));

   // Start a server process that listens for incoming network connections
    while (true) 
    {
            //  do some 'work'
        std::this_thread::sleep_for(std::chrono::seconds(1));


        std::stringstream publishData;
        for (auto &c : connections )
        {
            c.second++;
            publishData << "Client " << c.first << ": Iteration " << std::to_string(c.second) + "\n";
        }

        // std::cout << connections.size() << std::endl;

        std::string publishString = publishData.str();
        zmq::message_t pubMessage(publishString.data(), publishString.size());
        pubSocket.send(pubMessage, zmq::send_flags::none);
    }

    repThread.join();
    return 0;
}



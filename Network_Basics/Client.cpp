#include <zmq.hpp>
#include <string>
#include <iostream>

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t reqSocket(context, zmq::socket_type::req);

    std::cout << "Connecting to server..." << std::endl;
    reqSocket.connect("tcp://localhost:5555");      // requester port

    zmq::socket_t subSocket(context, zmq::socket_type::sub);
    subSocket.connect("tcp://localhost:5556");      // subscriber port

        // subscribe to all topics (empty string)
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

        // wait for server messages
    while (true)
    {
        // std::cout << "hereeee" << std::endl;
        zmq::message_t reply;
        zmq::recv_result_t result = subSocket.recv(reply, zmq::recv_flags::none);

        std::string serverMessage = std::string(static_cast<char*>(reply.data()), reply.size());
        std::cout << serverMessage;

        // zmq::message_t request3(clientID.data(), clientID.size());
        // reqSocket.send(request3, zmq::send_flags::none);
    }
    return 0;
}
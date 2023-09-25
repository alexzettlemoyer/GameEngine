#include <zmq.hpp>
#include <string>
#include <iostream>


void sendEmptyMessage(zmq::socket_t *socket, int clientID)
{
    std::cout << clientID << std::endl;
    zmq::message_t request(sizeof(int));
    memcpy(request.data (), &clientID, sizeof(int));
    socket -> send(request, zmq::send_flags::none);
}

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);

    std::cout << "Connecting to server..." << std::endl;
    socket.connect("tcp://localhost:5555");

        // send inital request
    zmq::message_t request(3);
    memcpy(request.data (), "REQ", 3);
    socket.send(request, zmq::send_flags::none);

        // receive the client id assigned by the server
    zmq::message_t reply;
    zmq::recv_result_t result = socket.recv(reply);

        // get the connection number / client id from the reply
    int connectionNumber;
    memcpy(&connectionNumber, reply.data(), sizeof(int));
    std::string clientID = std::to_string(connectionNumber);

        // send message to start receiving iteration updates
    zmq::message_t request2(clientID.data(), clientID.size());
    socket.send(request2, zmq::send_flags::none);

        // wait for server messages
    while (true)
    {
        zmq::message_t reply;
        zmq::recv_result_t result = socket.recv(reply, zmq::recv_flags::none);

        std::string serverMessage = std::string(static_cast<char*>(reply.data()), reply.size());
        std::cout << serverMessage;

        zmq::message_t request3(clientID.data(), clientID.size());
        socket.send(request3, zmq::send_flags::none);
    }
    return 0;
}
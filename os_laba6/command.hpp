#include <iostream>
#include <zmq.hpp>
#include <unistd.h>
#include <string>

using namespace std;

void create_node(int& id, int& port) {
    char* arg_id = strdup((to_string(id)).c_str());
    char* arg_port = strdup((to_string(port)).c_str());
    char* args[] = {arg_id, arg_port, NULL};
    execv("./child", args);
}

string get_port(int& port) {
    return "tcp://127.0.0.1:" + to_string(port);
}

int bind_socket(zmq::socket_t& socket) {
    int port = 3000;
    while (true) {
        try {
            socket.bind(get_port(port));
            break;
        } catch(zmq::error_t &e) {
            ++port;
        }
    }
    return port;
}

bool message_send(zmq::socket_t& socket, const string& msg) {
    int msg_size = msg.size();
    zmq::message_t message(msg_size);
    memcpy(message.data(), msg.c_str(), msg_size);
    try {
        socket.send(message, zmq::send_flags::none);
        return true;
    } catch(...) {
        return false;
    }
}

string message_recieve(zmq::socket_t& socket) {
    zmq::message_t request;
    zmq::send_result_t answer;
    try {
        answer = socket.recv(request, zmq::recv_flags::none);
    } catch(zmq::error_t &e) {
        answer = false;
    }
    string recieve_msg(static_cast<char*>(request.data()), request.size());
    if (recieve_msg.empty() || !answer)
        return "Error: Node is not available";
    else
        return recieve_msg;
}
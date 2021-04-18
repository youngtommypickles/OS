#include "command.hpp"
#include <csignal>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]){
    if(argc != 2) {
        cout << "Error: child's parametrs" << endl;
        return -1;
    }

    int id = stoi(argv[0]);
    int port = stoi(argv[1]);
    zmq::context_t context(2);
    zmq::socket_t parent_socket(context, ZMQ_REP);
    zmq::socket_t child_socket(context, ZMQ_REQ);

    parent_socket.connect(get_port(port));
    int child_port = bind_socket(child_socket);

    string request;
    string cmd;
    string sub_cmd;
    string msg;
    string result;
    int input_id;
    int child_pid = 0;
    int child_id = 0;
    int send_child = 0;
    int last_heartbit_time = -1;

    auto begin = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    auto elapsed_ms = 0;

    for(;;){
        request = message_recieve(parent_socket);
        istringstream cmd_stream(request);
        cmd_stream >> cmd;
        if(cmd == "pid") {
            msg = "OK: " + to_string(getpid());
            message_send(parent_socket, msg);
        } else if (cmd == "kill_child") {
            if (child_pid == 0) {
                msg = "OK";
                message_send(parent_socket, msg);
            } else {
                msg = "kill_child";
                message_send(child_socket, msg);
                result = message_recieve(child_socket);
                if(result == "OK"){
                    message_send(parent_socket, result);
                }else{
                    cout << "Error: kill" << endl;
                }
                kill(child_pid, SIGTERM);
                kill(child_pid, SIGKILL);
                message_send(parent_socket, result);
            }
        }else if(cmd == "ping"){
            if(child_pid == 0){
                msg = "OK: ";
                message_send(parent_socket, msg);
            }else{
                message_send(child_socket, cmd);
                string str = message_recieve(child_socket);
                result = str + to_string(child_id) + " ";
                message_send(parent_socket, result);
            }
        }else if(cmd == "create") {
            cmd_stream >> input_id;
            if (input_id == id) {
                msg = "Error: Already exists";
                message_send(parent_socket, msg);
            } else if (child_pid == 0) {
                    child_pid = fork();
                    if (child_pid == 0) {
                        create_node(input_id, child_port);
                    } else {
                        child_id = input_id;
                        msg = "pid";
                        message_send(child_socket, msg);
                        result = message_recieve(child_socket);
                        message_send(parent_socket, result);
                    }
            } else {
                message_send(child_socket, request);
                result = message_recieve(child_socket);
                message_send(parent_socket, result);
            }
        }else if(cmd == "remove"){
            cmd_stream >> input_id;
            if(child_pid == 0){
                msg = "Error: Not found";
                message_send(parent_socket, msg);
            }else if(child_id == input_id){
                msg = "kill_child";
                message_send(child_socket, msg);
                result = message_recieve(child_socket);
                if(result == "OK"){
                    message_send(parent_socket, result);
                }else{
                    cout << "Error: kill" << endl;
                }
                kill(child_pid, SIGTERM);
                kill(child_pid, SIGKILL);
                child_pid = 0;
                child_id  = 0;
                message_send(parent_socket, result);
            } else{
                message_send(child_socket, request);
                result = message_recieve(child_socket);
                message_send(parent_socket, result);
            }
        }else if(cmd == "exec"){
            cmd_stream >> input_id;
            if(id == input_id){
                cmd_stream >> sub_cmd;
                if(sub_cmd == "start"){
                    begin = std::chrono::steady_clock::now();
                    result = "OK: start";
                }else if(sub_cmd == "stop"){
                    end = std::chrono::steady_clock::now();
                    elapsed_ms = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                    result = "OK: stop";
                }else if(sub_cmd == "time"){
                    result = "OK: " + to_string(elapsed_ms) + " ms";
                    elapsed_ms = 0;
                }else{
                    result = "Error: bad subcommand";
                }
                message_send(parent_socket, result);
            }else{
                if(child_pid == 0){
                    msg = "Error: Not found";
                    message_send(child_socket, msg);
                }else{
                    message_send(child_socket, request);
                    result = message_recieve(child_socket);
                    message_send(parent_socket, result);
                }
            }
        }else if(cmd == "heartbeat"){
            int time;
            cmd_stream >> time;
            if(child_pid == 0){
                msg = to_string(id);
            }else{
                auto t1 = std::chrono::steady_clock::now();
                message_send(child_socket, request);
                result = message_recieve(child_socket);
                auto t2 = std::chrono::steady_clock::now();
                auto T =  chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
                if(T > 4*time){
                    msg = to_string(id);
                }else{
                    msg = result + " " + to_string(id);
                }
            }
            message_send(parent_socket, msg);
        }
    }
}
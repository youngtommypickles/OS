#include "command.hpp"
#include <csignal>
#include <vector>
#include <map>

using namespace std;

int main(){
    vector<int> vec;
    map<int, int> works;
    zmq::context_t context(1);
    zmq::socket_t main_socket(context, ZMQ_REQ);
    int port = bind_socket(main_socket);
    
    string cmd;
    string msg;
    string sub_cmd;
    string result;
    int input_id;
    int n = 5;
    int child_pid = 0;
    int child_id = 0;

    auto begin = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    auto elapsed_ms = 0;

    for(;;){
        cin >> cmd;
        if(cmd == "create") {
            cin >> input_id;
            if (child_pid == 0) {
                child_pid = fork();
                if (child_pid == 0) {
                    create_node(input_id, port);
                } else{
                    child_id = input_id;
                    msg = "pid";
                    message_send(main_socket, msg);
                    result = message_recieve(main_socket);
                    if(result.substr(0,2) == "OK"){
                        vec.push_back(input_id);
                    }
                }
            }else {
                ostringstream msg_stream;
                msg_stream << "create " << input_id;
                message_send(main_socket, msg_stream.str());
                result = message_recieve(main_socket);
                if(result.substr(0,2) == "OK"){
                    vec.push_back(input_id);
                }
            }
            cout << result << endl;
        }else if(cmd == "remove") {
            if (child_pid == 0) {
                cout << "Error: Not found" << endl;
                continue;
            }
            cin >> input_id;
            if (input_id == child_id) {
                msg = "kill_child";
                message_send(main_socket, msg);
                result = message_recieve(main_socket);
                if(result == "OK"){
                    kill(child_pid, SIGTERM);
                    kill(child_pid, SIGKILL);
                    child_id = 0;
                    child_pid = 0;
                    cout << result << endl;
                    vec.clear();
                }else{
                    cout << "Error: exit" << endl;
                }
                continue;
            }
            ostringstream msg_stream;
            msg_stream << "remove " << input_id;
            message_send(main_socket, msg_stream.str());
            result = message_recieve(main_socket);
            cout << result << endl;
            if(result.substr(0,2) == "OK"){
                for(int i = vec.size() - 1; i >= 0; --i){
                    if(vec[i] != input_id){
                        vec.pop_back();
                    }else{
                        vec.pop_back();
                        break;;
                    }
                }
            }
        }else if(cmd == "all"){
            if (child_pid == 0) {
                cout << "Error: Not found" << endl;
                continue;
            }
            for(int i = 0; i < vec.size(); i++){
                cout << vec[i] << " ";
            }
            cout << endl;
        }else if(cmd == "exec"){
            if (child_pid == 0) {
                cout << "Error: Not found" << endl;
                continue;
            }
            cin >> input_id;
            cin >> sub_cmd;
            ostringstream msg_stream;
            msg_stream << "exec " << input_id << " " << sub_cmd;
            message_send(main_socket, msg_stream.str());
            result = message_recieve(main_socket);
            cout << result << endl; 
        }else if(cmd == "heartbeat"){
            if (child_pid == 0) {
                cout << "Error: Not found" << endl;
                continue;
            }
            works.clear();
            int time;
            cin >> time;
            cmd = cmd + " " + to_string(time);
            sleep(time/1000);
            message_send(main_socket, cmd);
            result = message_recieve(main_socket);
            istringstream is = istringstream(result);
            while(is){
                is >> input_id;
                works.insert(make_pair(input_id, 1));
            }
            cout << "OK" << endl;
        }else if (cmd == "ping") {
            if(works.size() == 0){
                continue;
            }
            cin >> input_id;
            if(works[input_id] == 1){
                cout << "OK: 1" << endl;
            }else{
                cout << "OK: -1" << endl;
            }
        }else if(cmd == "exit"){
            if(child_pid == 0){
                cout << "OK\n";
                return 0;
            }
            msg = "kill_child";
            message_send(main_socket, msg);
            result = message_recieve(main_socket);
            if(result == "OK"){
                kill(child_pid, SIGTERM);
                kill(child_pid, SIGKILL);
                child_id = 0;
                child_pid = 0;
                cout << result << endl;
            }else{
                cout << "Error: exit" << endl;
            }
            return 0;
        }else{
            cout << "Error: bad command" << endl;
        }
    }
}
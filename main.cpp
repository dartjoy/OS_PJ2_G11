#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

const int CMD_LENGTH = 4;

const string DATA_DIR ("./test_data");
const string FILE_NAME("data_1.txt");

class Cmd{
    public:
        string proc_name;
        int priority;
        int runtime;
        int arrival_time;
        Cmd(string name, int p, int run, int arrive){
            proc_name = name;
            priority = p;
            runtime = run;
            arrival_time = arrive;
        }
        Cmd(string name, string p, string run, string  arrive){
            std::string::size_type sz;
            proc_name = name;
            priority = std::stoi(p, &sz);
            runtime = std::stoi(run, &sz);
            arrival_time = std::stoi(arrive, &sz);
        }
        void print(){
            cout << "Proccess Name: " << proc_name << endl;
        }
};

bool load_cmd(vector<string>* cmd, string l){
    istringstream input(l);
    string s;
    while( input >> s )
        cmd->push_back(s);
}

bool load_from_file(queue<Cmd>* q, string filename){
    ifstream cmd_file;
    cmd_file.open(filename, ios::in);
    if(!cmd_file)   return false;

    string line_str;
    while(getline(cmd_file, line_str)){
        vector<string> v;
        load_cmd(&v, line_str);
        if(v.size() != CMD_LENGTH) return false;
        Cmd new_cmd(v[0], v[1], v[2], v[3]);
        q->push(new_cmd);
    }
    cmd_file.close();
    return true;
}

int main(){
    string cmd_filename = DATA_DIR + '/' + FILE_NAME;
    queue<Cmd> cmd_queue;
    if(load_from_file(&cmd_queue, cmd_filename)){
        cout << "Tasks read." << endl;
        while(cmd_queue.size() > 0){
            Cmd p = cmd_queue.front();
            cmd_queue.pop();
            p.print();
        }
    }
    else
        cout << "ERROR! Can't open file: " << cmd_filename << endl;
    return 0;
}

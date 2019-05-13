#include <string>
#include <queue>
#include <iostream>

#include "command.cpp"
#include "scheduler_fcfs.cpp"
#include "scheduler_sjr.cpp"
#include <unistd.h>

using namespace std;

const string DATA_DIR ("./test_data");
const string FILE_NAME("data_1.txt");

int main(){
    string cmd_filename = DATA_DIR + '/' + FILE_NAME;
    queue<Cmd> cmd_queue;
    if(!Cmd::load_from_file(&cmd_queue, cmd_filename))
        cout << "ERROR! Can't open file: " << cmd_filename << endl;
    else{
        cout << "Tasks read." << endl;
        Scheduler_FCFS fcfs(&cmd_queue);
        while(!fcfs.is_empty()){
            fcfs.work();
            //fcfs.print_detail();
        }
        fcfs.print_summary();
    }
    return 0;
}

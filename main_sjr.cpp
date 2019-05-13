#include <string>
#include <queue>
#include <iostream>

#include "command.cpp"
#include "scheduler_fcfs.cpp"
#include "scheduler_sjr.cpp"
#include <unistd.h>

using namespace std;

const string DATA_DIR ("./test_data");
const string FILE_NAME("data_3.txt");

int main(){
    string cmd_filename = DATA_DIR + '/' + FILE_NAME;
    queue<Cmd> cmd_queue;
    if(!Cmd::load_from_file(&cmd_queue, cmd_filename))
        cout << "ERROR! Can't open file: " << cmd_filename << endl;
    else{
        cout << "Tasks read." << endl;
        Scheduler_SJR sjr(&cmd_queue);
        while(!sjr.is_empty()){
            sjr.work();
            //sjr.print_detail();
        }
        sjr.print_summary();
    }
    return 0;
}

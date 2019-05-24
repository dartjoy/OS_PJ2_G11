#include <string>
#include <queue>
#include <iostream>

#include "command.cpp"
//#include "scheduler_fcfs.cpp"
//#include "scheduler_sjr.cpp"
#include "scheduler_srtf.cpp"
//#include "scheduler_priority.cpp"
//#include "scheduler_rr.cpp"

using namespace std;

const string DATA_DIR ("./test_data");
string FILE_NAME("data_1");

int main(int argc, char *argv[]){
    if(argv[1])
        FILE_NAME = string(argv[1]);
    string cmd_filename = DATA_DIR + '/' + FILE_NAME + ".txt";
    queue<Cmd> cmd_queue;
    if(!Cmd::load_from_file(&cmd_queue, cmd_filename))
        cout << "ERROR! Can't open file: " << cmd_filename << endl;
    else{
        Scheduler_SRTF scheduler(cmd_queue, FILE_NAME);
        while(!scheduler.is_finish()){
            scheduler.work();
            //scheduler.print_detail();
        }
        scheduler.print_summary();

    }
    return 0;
}

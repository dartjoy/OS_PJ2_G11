#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include <queue>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#include <string>
#include "command.cpp"

#define SIM_TIME 5000 // Print delay in microseconds
#define PERFORMANCE_TIME_UNIT 10000

#define uint unsigned int

using namespace std;

/* The class which help record the properties of schduler */
/*
   class Batch{
   public:
   uint total_waiting_time;
   uint total_idle_time;
   uint throughput;
   uint total_turnaround_time;
   uint context_switch;

   Batch(uint waiting, uint idle, uint turn, uint count, uint con){
   total_waiting_time = waiting;
   total_idle_time = idle;
   throughput = count;
   total_turnaround_time = turn;
   context_switch = con;
   }
   Batch(){
   reset();
   }
// Set all properties to 0
void reset(){
total_waiting_time = 0;
total_idle_time = 0;
throughput = 0;
total_turnaround_time = 0;
context_switch = 0;
} 
void make_summary(Performance *p, uint len){
for(int i=0; i<len; i++){
total_waiting_time += p[i].waiting_time;
}
}
void print(){
cout << "avg_waiting: " << total_waiting_time/throughput
<< "\tidle_time: " << total_idle_time
<< "\tthroughput: " << throughput
<< "\tturnaround: " << total_turnaround_time/throughput
<< "\tcontext_switch: " << context_switch << endl;
}
};*/

class Performance{
    public:
        bool complete;
        uint proc_id;           // Proc number
        uint turnaround_time;   // Finish time - submission time
        uint waiting_time;      // Time in ready_queue
        uint response_time;     // Submission to first run
        uint runtime;
        Performance(){
            Cmd default_cmd;
            default_cmd.proc_name = string("P0");
            init(default_cmd);
        }
        static uint get_id(Cmd cmd){
            int l = cmd.proc_name.length();
            std::string::size_type sz;
            //cout << cmd.proc_name << endl;
            return std::stoi(cmd.proc_name.substr(1, l-1), &sz);
        }
        void init(Cmd cmd){
            complete = false;
            proc_id = get_id(cmd);
            turnaround_time = 0;
            waiting_time = 0;
            response_time = 0;
            runtime = cmd.runtime;
        }
        // This function should be called after runtime and before arrival_time update
        void record(uint now_time, Cmd cmd){
            if(cmd.runtime <= 0){
                // Last run
                turnaround_time = now_time - cmd.commit_time;
            }
            if(waiting_time <= 0){
                // First run -> response_time
                response_time = now_time - cmd.commit_time;
                waiting_time = response_time;
            }
            else{
                waiting_time += now_time - cmd.arrival_time;
            }
        }
};

/* Parant class of Schduler, provide
   1. Basic properties recording function
   - You need to call 'record_task_complete()' to mark completed task
   2. You can print all batched properties with 'print_summary()'
   3. If you want to print details during schduling, call 'print_detail()' after 'work()' called
 */
class Scheduler{
    protected:
        uint now_time;                          // The schduler time while working
        Cmd now_task = Cmd();                   // Current running task
        queue<Cmd> cmd_queue;                  // Original cmd queue
        //queue<Batch> batches;                   // Batched properties
        //Batch now_batch = Batch(0, 0, 0, 0, 0); // Current properties
        Performance *performance; 
        uint num_of_cmd;
        uint idle_time;
        uint context_switch;

        string scheduler_name = string("default");
        ofstream ofd_time;
        ofstream ofd_summary;
        //unsigned int batch_counter = 0;
        void record_waiting_time(Cmd task){
            uint proc_id = Performance::get_id(task);
            uint waiting_t = now_time - task.arrival_time;
            if(performance[proc_id].waiting_time <= 0)      // First time excute
                performance[proc_id].response_time = waiting_t;
            performance[proc_id].waiting_time += waiting_t;
        }
        void record_idle_time(unsigned int idle_t){
            idle_time += idle_t;
        }
        void record_task_complete(Cmd task){
            uint proc_id = Performance::get_id(task);
            performance[proc_id].complete = true;
            performance[proc_id].turnaround_time = now_time - task.commit_time;
        }
        void record_switch(){
            context_switch++;
        }
        /*
           void create_new_batch(){
// New batch
batches.push(now_batch);
now_batch.reset();
batch_counter++;
}
// If current batch excceed running time, switch to next.
void record_check_batch(){            
uint proc_id = Performance::get_id(task);
performance[proc_id].record(task);

if( now_time - batch_counter * PERFORMANCE_TIME_UNIT > PERFORMANCE_TIME_UNIT ){
create_new_batch();
}
if(!ofd_time.fail()){
cout << "Time=" << now_time << endl;//", ready_size=" << ready_queue.size() << endl;
}
}*/
void set_output_file(string name){
    string dir("./analysis/");
    //dir += scheduler_name + "/";
    string time_stamp = dir + name + "_time";
    string summary = dir + name + "_summary";
    ofd_time.open(time_stamp, std::ios_base::app);
    ofd_summary.open(summary, std::ios_base::app);
}

public:
Scheduler(queue<Cmd> q){
    cmd_queue = q;
    now_time = 0;
    idle_time = 0;
    context_switch = 0;
    num_of_cmd = cmd_queue.size();
    performance = new Performance[num_of_cmd];
    queue<Cmd> temp = cmd_queue;
    for(int i=0; i<num_of_cmd; i++){
        Cmd cmd = temp.front();
        performance[i].init(cmd);
        temp.pop();
    }
}
bool is_empty(){
    return cmd_queue.size() <= 0;
}       
void print_summary(){
    uint total_waiting = 0;
    uint total_response = 0;
    uint var_response = 0;
    uint total_turnaround = 0;
    uint total_throughput = 0;
    for(int i=0; i<num_of_cmd; i++){
        if(performance[i].complete){
            total_waiting += performance[i].waiting_time;
            total_response += performance[i].response_time;
            var_response += performance[i].response_time * performance[i].response_time;
            total_turnaround += performance[i].turnaround_time;
            total_throughput ++;
        }
    }
    var_response -= total_response * total_response;
    var_response /= total_throughput;
    total_waiting /= total_throughput;
    total_response /= total_throughput;
    total_turnaround /= total_throughput;

    cout << "Total time used: " << now_time << endl;
    cout << "throughput= " << total_throughput << endl;
    cout << "idle= " << idle_time << endl;
    cout << "waiting= " << total_waiting << endl;
    cout << "avg_response= " << total_response << endl;
    cout << "variance_response= " << var_response << endl;
    cout << "turnaround= " << total_turnaround << endl;
    cout << "context_switch= " << context_switch << endl;
    if(!ofd_summary.fail()){
        ofd_summary << "time_used, throughput, idle, waiting, avg_response" << 
                   ", variance_response, turnaround, context_switch" << endl;
        ofd_summary << now_time << "," << total_throughput << "," << idle_time << ","
            << total_waiting << "," << total_response << "," << var_response << ","
            << total_turnaround << "," << context_switch << endl;
    }
    if(!ofd_time.fail()){
        ofd_time << "Proc_name, runtime, waiting, turnaround, response" << endl;
        for(int i=0; i<num_of_cmd; i++){
            ofd_time << "P" << performance[i].proc_id << "," << performance[i].runtime << ","
                << performance[i].waiting_time << "," << performance[i].turnaround_time << ","
                << performance[i].response_time << endl;
        }
    }
    ofd_time.close();
    ofd_summary.close();
}
void print_running_task(){
    cout << "Time: " << now_time << "\tProc: " << now_task.proc_name <<
        "\tRun: " << now_task.runtime << endl;
}
void print_detail(){
    print_running_task();
    usleep(SIM_TIME);
}
// Child classes need to implement this function, which push tasks from cmd_queue into
// ready_queue, and consume tasks in ready queue.
virtual bool is_finish(){
    return is_empty();
}
virtual void work() = 0;
};

#endif

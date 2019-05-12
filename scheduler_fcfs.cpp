#ifndef SCHEDULER_FCFS
#define SCHEDULER_FCFS

#include <queue>
#include <unistd.h>

#include "command.cpp"

#define SIMULATION
#define SIM_TIME 500000 // Print delay in microseconds

class Scheduler_FCFS{
    private:
        queue<Cmd> *cmd_queue;
    public:
        Scheduler_FCFS(queue<Cmd> *q){
            cmd_queue = q;
        }
        bool is_empty(){
            return cmd_queue->size() <= 0;
        }
        void work(){
            if(!is_empty()){
                static unsigned int now_time = 0;
                Cmd task = cmd_queue->front();
                if(now_time >= task.arrival_time){  // Overlapped
                    unsigned int waiting_time = task.arrival_time - now_time;
                }
                else{                               // Idle
                    unsigned int idle_time = task.arrival_time - now_time;
                    now_time = task.arrival_time;
                    cout << "Idle " << idle_time << endl;
                }
#ifdef SIMULATION
                print(task, now_time);
                usleep(SIM_TIME);
#endif
                now_time += task.runtime;
                cmd_queue->pop();
            }
        }
        void print(Cmd task, unsigned int now_time){
            cout << "Time: " << now_time << "\tProc: " << task.proc_name <<
                "\tRun: " << task.runtime << endl;
        }
};

#endif

#ifndef SCHEDULER_FCFS
#define SCHEDULER_FCFS

#include <queue>

#include "command.cpp"
#include "scheduler.cpp"

class Scheduler_FCFS:public Scheduler{
    private:
    public:
        Scheduler_FCFS(queue<Cmd> q, string dataset_name):Scheduler(q){
            set_output_file("fcfs" + dataset_name);
        }
        virtual void work(){
            if(!is_empty()){
                now_task = cmd_queue.front();          // new task
                if(now_time >= now_task.arrival_time){  // Proc overlapped
                    record_waiting_time(now_task);
                }
                else{                                   // Processor idle
                    record_idle_time(now_task.arrival_time - now_time);
                    now_time = now_task.arrival_time;   // Move to next task
                }

                /* Run Task */
                now_time += now_task.runtime;
                now_task.runtime = 0;
                record_task_complete(now_task);         // Mark task complete
                record_switch();
                cmd_queue.pop();
                /* Task Finished */
            }
        } 
};

#endif

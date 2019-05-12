#ifndef SCHEDULER_FCFS
#define SCHEDULER_FCFS

#include <queue>

#include "command.cpp"
#include "scheduler.cpp"

class Scheduler_FCFS:public Scheduler{
    private:
    public:
        Scheduler_FCFS(queue<Cmd> *q):Scheduler(q){
        }
        bool is_empty(){
            return cmd_queue->size() <= 0;
        }
        virtual void work(){
            if(!is_empty()){
                now_task = cmd_queue->front();          // new task
                if(now_time >= now_task.arrival_time){  // Proc overlapped
                    record_waiting_time(now_time - now_task.arrival_time);
                }
                else{                                   // Processor idle
                    record_idle_time(now_task.arrival_time - now_time);
                    now_time = now_task.arrival_time;   // Move to next task
                }

                /* Run Task */
                now_time += now_task.runtime;
                record_task_complete(now_task);         // Mark task complete
                cmd_queue->pop();
                /* Task Finished */
            }
        } 
};

#endif

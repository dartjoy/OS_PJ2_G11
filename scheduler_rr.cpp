#ifndef SCHEDULER_RR
#define SCHEDULER_RR

#include <queue>

#include "command.cpp"
#include "scheduler.cpp"

#define TIME_SLICE 5
#define DEBUG

class Scheduler_RR:public Scheduler{
    private:
        queue<Cmd> ready_queue;
    public:
        Scheduler_RR(queue<Cmd> *q):Scheduler(q){
        }
        bool is_finish(){
            return is_empty() && ready_queue.size()<=0;
        }
        virtual void work(){
            if(!is_finish()){
                // There is no task in ready_queue -> Idle
                if(ready_queue.size()<=0){
                    now_task = cmd_queue->front();
                    cmd_queue->pop();
                    record_idle_time(now_task.arrival_time - now_time);
                    now_time = now_task.arrival_time;
                    record_switch();
#ifdef DEBUG
                    cout << "Processor Idle" << endl;
#endif
                }
                uint rest_runtime = (now_task.runtime < TIME_SLICE) ? now_task.runtime : TIME_SLICE;
                // Task running with interrupt (Never preempt)
                Cmd next_task = cmd_queue->front();
                while(!is_empty() && 
                        next_task.arrival_time <= now_time + rest_runtime ){
                    ready_queue.push(next_task);        // Push the incoming task into ready_queue
                    cmd_queue->pop();
                    next_task = cmd_queue->front();
                }
                // Run the task
                record_waiting_time(now_time - now_task.arrival_time);
                now_time += rest_runtime;
                now_task.arrival_time = now_time;
                now_task.runtime -= rest_runtime;
                if(now_task.runtime > 0){
                    ready_queue.push(now_task);
#ifdef DEBUG
                    cout << "Time excceed: Proc= " << now_task.proc_name 
                         << " , Remain= " << now_task.runtime << endl;
#endif
                }
                else
                    record_task_complete(now_task);
                now_task = ready_queue.front();
                ready_queue.pop();
                record_switch();
            }
        } 
};

#endif

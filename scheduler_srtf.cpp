/*
 * Shortest-Remaining-Time-First Scheduler
 */

#ifndef SCHEDULER_SRTF
#define SCHEDULER_SRTF

#include <queue>
#include <vector>
#include <algorithm>

#include "command.cpp"
#include "scheduler.cpp"
bool operator<(const Cmd &a, const Cmd &b){
    if(a.runtime == b.runtime)
        return a.arrival_time > b.arrival_time;
    return a.runtime > b.runtime;
}
bool operator>(const Cmd &a, const Cmd &b){
    if(a.runtime == b.runtime)
        return a.arrival_time < b.arrival_time;
    return a.runtime < b.runtime;
}

class Scheduler_SRTF:public Scheduler{
    private:
        vector<Cmd> ready_queue;                 // Ready queue

    public:
        Scheduler_SRTF(queue<Cmd> *q):Scheduler(q){
            make_heap(ready_queue.begin(), ready_queue.end());
        }

        virtual void work(){
            if( !is_empty() ){
                // At the moment of a complete task, check for new cmd
                Cmd now_cmd = cmd_queue->front();              // new task
                while( now_time >= now_cmd.arrival_time && !is_empty() ){     // Task arrive
                    ready_queue.push_back(now_cmd);
                    push_heap(ready_queue.begin(), ready_queue.end());
                    cmd_queue->pop();
                    //now_cmd.print();
                    //cout << "Pushed into ready_queue" << endl;
                    now_cmd = cmd_queue->front();          // new task
                }
            }
            if( ready_queue.size() <= 0 ){
                // No more task in ready queue -> Idle
                if( !is_empty()){
                    now_task = cmd_queue->front();          // new task
                    record_idle_time(now_task.arrival_time - now_time);
                    now_time = now_task.arrival_time;       // Move to next task
                }
            }
            else{
                // Get shortest time task from ready_queue -> Some task must waiting
                now_task = ready_queue.front();
                pop_heap(ready_queue.begin(), ready_queue.end());  // new task
                ready_queue.pop_back();

                if(now_time >= now_task.arrival_time){  // Proc overlapped
                    record_waiting_time(now_time - now_task.arrival_time);
                }
                else{                                   // Processor idle
                    now_time = now_task.arrival_time;   // Move to next task
                    record_idle_time(now_task.arrival_time - now_time);
                }

                /* Run Task */
                //now_time += now_task.runtime;
                //record_task_complete(now_task);         // Mark task complete
                //record_switch();
                
                Cmd now_cmd = cmd_queue->front();              // new task
                uint task_start_time = now_time;
                while( now_time <= task_start_time + now_task.runtime
                        && !is_empty()
                        && now_time >= now_cmd.arrival_Time ){     // Task arrive
                    ready_queue.push_back(now_cmd);
                    push_heap(ready_queue.begin(), ready_queue.end());
                    cmd_queue->pop();
                    now_time += now_cmd.arrival_time - task_start_time;
                    //now_cmd.print();
                    //cout << "Pushed into ready_queue" << endl;
                    now_cmd = cmd_queue->front();          // new task
                }
                /* Task Finished */
            }
        } 
};

#endif

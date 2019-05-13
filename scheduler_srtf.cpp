/*
 * Shortest-Remaining-Time-First Scheduler
 */

#ifndef SCHEDULER_SRTF
#define SCHEDULER_SRTF

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
            if( ready_queue.size() <= 0 ){
                // No more task in ready queue -> Idle
                if( !is_empty()){
                    now_task = cmd_queue->front();          // new task
                    cmd_queue->pop();
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
                /* Confuse, I don't know whether a condition may satisfy this. It should not.*/
                else{                                   // Processor idle
                    record_idle_time(now_task.arrival_time - now_time);
                    now_time = now_task.arrival_time;   // Move to next task
                }

                /* Run Task, keep checking if there a task interrupts */
                Cmd now_cmd = cmd_queue->front();                   // check next task in cmd_queue
                // A new Task arrive during a running task -> push this task into ready queue
                // and determine whether to preempt
                if( !is_empty()
                    && now_time + now_task.runtime >= now_cmd.arrival_time){
                    cmd_queue->pop();
                    // now_time here is the time that the original one task start running
                    uint rest_runtime = now_cmd.arrival_time - now_time; 
                    // Replace original task with new runtime
                    now_task.runtime = rest_runtime; 

                    // New task is shorter -> context switch, push original task into ready_queue
                    if( now_cmd.runtime < rest_runtime ){
                        // Replace original task with new arrival_time(the moment pushed into q)
                        now_task.arrival_time = now_cmd.arrival_time;
                        // Push original one task into ready_queue
                        ready_queue.push_back(now_task);
                        push_heap(ready_queue.begin(), ready_queue.end());
                        
                        record_switch();
                    }
                    // Do the original one task -> push new cmd into ready_queue
                    else{
                        // Push new arrival task into ready_queue
                        ready_queue.push_back(now_cmd);                                    
                        push_heap(ready_queue.begin(), ready_queue.end());
                    }
                    // now_time moves to the time of the new task arriving
                    now_time = now_cmd.arrival_time;
                }
                // There is no task arrive during this task running -> Finish this task and
                // get new task in ready_queue
                else{
                    // Move now_time to the end of this task
                    now_time += now_task.runtime;
                    record_task_complete(now_task);
                    record_switch();
                    cout << "Task complete: ";
                    now_task.print();
                }
                /* Task Finished */
            }
        } 
};

#endif

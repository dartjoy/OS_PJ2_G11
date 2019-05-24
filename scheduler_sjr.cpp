/*
 * Shortest-Job-Next Scheduling
 */

#ifndef SCHEDULER_SJR
#define SCHEDULER_SJR

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

class Scheduler_SJR:public Scheduler{
    private:
        vector<Cmd> ready_queue;                 // Ready queue

    public:
        Scheduler_SJR(queue<Cmd> q, string dataset_name):Scheduler(q){
            set_output_file("sjr" + dataset_name);
            make_heap(ready_queue.begin(), ready_queue.end());
        }
        virtual bool is_finish(){
            return is_empty() && (ready_queue.size()<=0);
        }
        virtual void work(){
            if( !is_empty() ){
                // At the moment of a complete task, check next cmd
                Cmd now_cmd = cmd_queue.front();              // new task
                while( now_time >= now_cmd.arrival_time && !is_empty() ){     // Task arrive
                    ready_queue.push_back(now_cmd);
                    push_heap(ready_queue.begin(), ready_queue.end());
                    cmd_queue.pop();
                    //cout << "Pushed into ready_queue" << endl;
                    now_cmd = cmd_queue.front();          // new task
                }
            }
            if( ready_queue.size() <= 0 ){
                // No more task in ready queue -> Idle
                if( !is_empty()){
                    now_task = cmd_queue.front();          // new task
                    cmd_queue.pop();
                    ready_queue.push_back(now_task);
                    push_heap(ready_queue.begin(), ready_queue.end());

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
                    record_waiting_time(now_task);
                }/*
                else{                                   // Processor idle
                    record_idle_time(now_task.arrival_time - now_time);
                    now_time = now_task.arrival_time;   // Move to next task
                }*/

                /* Run Task */
                now_time += now_task.runtime;
                now_task.runtime = 0;
                record_task_complete(now_task);         // Mark task complete
                record_switch();
                /* Task Finished */
            }
        } 
};

#endif

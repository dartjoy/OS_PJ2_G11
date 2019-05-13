#ifndef SCHEDULER_CPP
#define SCHEDULER_CPP

#include <queue>
#include <unistd.h>
#include <iomanip>
#include "command.cpp"
    
#define SIM_TIME 5000 // Print delay in microseconds
#define PERFORMANCE_TIME_UNIT 1000

#define uint unsigned int

/* The class which help record the properties of schduler */
class Batch{
    public:
        uint total_waiting_time;
        uint total_idle_time;
        uint throughput;
        uint total_turnaround_time;

        Batch(uint waiting, uint idle, uint turn, uint count){
            total_waiting_time = waiting;
            total_idle_time = idle;
            throughput = count;
            total_turnaround_time = turn;
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
        } 
        void print(){
            cout << "waiting: " << total_waiting_time
                 << "\tidle_time: " << total_idle_time
                 << "\tthroughout: " << throughput
                 << "\tturnaround: " << total_turnaround_time << endl;
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
        queue<Cmd> *cmd_queue;                  // Original cmd queue
        queue<Batch> batches;                   // Batched properties
        Batch now_batch = Batch(0, 0, 0, 0);    // Current properties
        void record_waiting_time(unsigned int waiting_t){
            now_batch.total_waiting_time += waiting_t;
            record_check_batch();
        }
        void record_idle_time(unsigned int idle_t){
            now_batch.total_idle_time += idle_t;
            record_check_batch();
        }
        void record_task_complete(Cmd task){
            now_batch.total_turnaround_time += (now_time - task.arrival_time);
            now_batch.throughput += 1;
            record_check_batch();
        }
        // If current batch excceed running time, switch to next.
        void record_check_batch(){
            static unsigned int batch_counter = 0;
            if( now_time - batch_counter * PERFORMANCE_TIME_UNIT > PERFORMANCE_TIME_UNIT ){
                // New batch
                batches.push(now_batch);
                now_batch.reset();
                batch_counter++;
            }
        }

    public:
        Scheduler(queue<Cmd> *q){
            cmd_queue = q;
            now_time = 0;
        }
        bool is_empty(){
            return cmd_queue->size() <= 0;
        }       
        void print_summary(){
            cout << "Total time used: " << now_time << endl
                 << "Batched time unit: " << PERFORMANCE_TIME_UNIT << endl;
            int count = 0;
            while(batches.size()>0){
                cout << "Batch " << setw(2) << count << "   ";
                batches.front().print();
                batches.pop();
                count++;
            }
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
        virtual void work() = 0;
};

#endif

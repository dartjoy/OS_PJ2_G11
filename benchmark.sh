#!/bin/bash
TEST_FILE="data_1 data_2 data_3"
MAIN="main_fcfs main_sjr main_srtf main_priority main_rr"

for data in $TEST_FILE
do
    for main in $MAIN
    do
        echo "Runing " $main " with data"
        ./output/$main $data
    done
done

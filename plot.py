#!/bin/python3

import matplotlib.pyplot as plt
import numpy as np

DATA_TYPE = ['data_1', 'data_2', 'data_3']
SCHEDULER = ['fcfs', 'sjr', 'srtf', 'priority', 'rr']
PROPERTIES = {'task_name':0, 'runtime':1, 'waiting_time':2, 'turnaround_time':3, 'response_time':4}
DATA_DIR = './analysis/'

def load_from_filename(name):
    print("Reading " + name)
    with open(name) as f:
        content = f.readlines()
        proc = []
        for line in content[1:-1]:
            l = line.split(',')
            if(len(l) >= 5 ):
                l[0] = l[0][1:]
                for i in range(len(l)):
                    if(l[i] == ''):
                        l[i] = '0'
                l[-1] = l[-1][0:-1]
                proc.append(l)
    arr = np.asarray(proc, dtype=np.int32)
    return arr

def load_data(data_type):
    summary = {}
    time_stamp = {}
    for sche in SCHEDULER:
        name = DATA_DIR + sche + data_type + '_time'
        arr = load_from_filename(name)
        time_stamp[sche] = arr
        name = DATA_DIR + sche + data_type + '_summary'
        arr = load_from_filename(name)
        summary[sche] = arr
    return summary, time_stamp

def scatter(p1, p2, time_stamp):
    name = p1 + ' and ' + p2
    plt.figure( name )
    plt.title( name )
    for sche in SCHEDULER:
        plt.scatter(time_stamp[sche][:, PROPERTIES[p1]], time_stamp[sche][:, PROPERTIES[p2]], alpha=0.7, s=0.5)
        #plt.plot()
    plt.legend(SCHEDULER)

def main():
    summary, time_stamp = load_data(DATA_TYPE[0])
    scatter('runtime', 'waiting_time', time_stamp)
    scatter('runtime', 'turnaround_time', time_stamp)
    scatter('runtime', 'response_time', time_stamp)
    plt.show()
    
if __name__ == "__main__":
    main()

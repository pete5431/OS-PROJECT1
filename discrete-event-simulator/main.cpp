#include <iostream>
#include <queue>
#include <cstdlib>
#include "FileParam.h"
#include "Event.h"
using namespace std;

// The System time.
int sys_time;
// The current number of jobs.
int job_num = 1;

int CPU_BUSY = 0;

enum{
	PROCESS_ARRV = 1,
        CPU_ARRV = 2,
        CPU_FIN = 3,
        PROCESS_EXIT = 4,
        DISK1_ARRV = 5,
        DISK1_FIN = 6,
	DISK2_ARRV = 7,
	DISK2_FIN = 8,
	NETWORK_ARRV = 9,
	NETWORK_FIN = 10,
	SYS_FIN = 11
};

queue<Event> CPU;
queue<Event> Disk1;
queue<Event> Disk2;
queue<Event> Network;

void job_arrival_handler(FileParam, priority_queue<Event>&, Event);

int main(){

	FileParam sys;

	sys.read_file((char*)"config.txt");

	sys_time = sys.INIT_TIME;	

	srand(sys.SEED);

	priority_queue<Event> events;	

	// Creates the finish system event and pushes to priority queue. 

	Event finish = create_event(sys.FIN_TIME, SYS_FIN, 0);

	events.push(finish);

	// Create first job.

	int next_arrival_time = gen_rand(sys.ARRIVE_MAX, sys.ARRIVE_MIN);
	Event next_event;

	next_event = create_event(next_arrival_time, PROCESS_ARRV, job_num);
	events.push(next_event);
	job_num++;

	/*

	int i = 0;

	while(i < 10){
		next_arrival_time = gen_rand(sys.ARRIVE_MAX, sys.ARRIVE_MIN);
		next_event = create_event(next_arrival_time, PROCESS_ARRV, job_num);
		events.push(next_event);
		job_num++;
		print_log(next_event);
		i++;
	}

	while(!events.empty()){
		next_event = events.top();
		events.pop();
		cout << next_event.time << "\n";
	}

	*/

	// While the priority queue is not empty and the system time is not equal to FIN_TIME.
	while(!events.empty() && sys_time < sys.FIN_TIME){
		
		next_event = events.top();
		events.pop();

		switch(next_event.type){
			
			case PROCESS_ARRV:
				job_arrival_handler(sys, events, next_event);
				break;
			default:
				break;

		}		

	}

	while(!CPU.empty()){
		Event e = CPU.front();
		CPU.pop();
		cout << e.time << "\n";
	}

	return 0;
}

void job_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event){
	print_log(next_event);
	sys_time = next_event.time;

	int next_arrival_time = gen_rand(sys.ARRIVE_MAX, sys.ARRIVE_MIN) + sys_time;
	Event new_event = create_event(next_arrival_time, PROCESS_ARRV, job_num);
	events.push(new_event);
	job_num++;
	
	if(!CPU.empty()){
		CPU_BUSY = 1;
		int cpu_time = gen_rand(sys.CPU_MAX, sys.CPU_MIN);
		new_event = create_event(cpu_time, CPU_ARRV, next_event.pid);
	}
	else CPU.push(next_event);	
}

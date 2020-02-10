#include <iostream>
#include <queue>
#include <cstdlib>
#include <stdio.h>
#include "FileParam.h"
#include "Event.h"
using namespace std;

// The System time.
int sys_time;
// The current number of jobs.
int job_num = 1;
// The jobs that exited the system.
int job_left = 0;

int job_enter_network = 0;

bool CPU_BUSY = false;
bool DISK1_BUSY = false;
bool DISK2_BUSY = false;
bool NETWORK_BUSY = false;

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
queue<Event> DISK1;
queue<Event> DISK2;
queue<Event> NETWORK;

void job_arrival_handler(FileParam, priority_queue<Event>&, Event);
void job_finish_handler(Event);
void system_finish_handler(FileParam, Event);
void cpu_arrival_handler(FileParam, priority_queue<Event>&, Event);
void cpu_finish_handler(FileParam, priority_queue<Event>&, Event);
void disk1_arrival_handler(FileParam, priority_queue<Event>&, Event);
void disk1_finish_handler(priority_queue<Event>&, Event);
void disk2_arrival_handler(FileParam, priority_queue<Event>&, Event);
void disk2_finish_handler(priority_queue<Event>&, Event);
void network_arrival_handler(FileParam, priority_queue<Event>&, Event);
void network_finish_handler(priority_queue<Event>&, Event);

int main(){

	FileParam sys;

	sys.read_file((char*)"config.txt");

	sys_time = sys.INIT_TIME;	

	srand(sys.SEED);

	priority_queue<Event> events;	

	FILE* fp = fopen("log.txt", "w");

	if(fp == NULL){
		perror("Error: ");
		exit(1);
	}

	// Creates the finish system event and pushes to priority queue. 

	Event finish = create_event(sys.FIN_TIME, SYS_FIN, 0);

	events.push(finish);

	// Create first job.

	int next_arrival_time = gen_rand(sys.ARRIVE_MAX, sys.ARRIVE_MIN);
	Event next_event;

	next_event = create_event(next_arrival_time, PROCESS_ARRV, job_num);
	events.push(next_event);
	job_num++;

	// While the priority queue is not empty and the system time is not equal to FIN_TIME.
	while(!events.empty() && sys_time != sys.FIN_TIME){
		
		next_event = events.top();
		events.pop();

		switch(next_event.type){
			case PROCESS_ARRV:
				print_log(fp, next_event);
				sys_time = next_event.time;
				job_arrival_handler(sys, events, next_event);
				break;
			case PROCESS_EXIT:
				print_log(fp, next_event);
				sys_time = next_event.time;
				job_finish_handler(next_event);
				break;
			case CPU_ARRV:
				print_log(fp, next_event);
				sys_time = next_event.time;
				cpu_arrival_handler(sys, events, next_event);
				break;
			case CPU_FIN:
				print_log(fp, next_event);
				sys_time = next_event.time;
				cpu_finish_handler(sys, events, next_event);
				break;
			case DISK1_ARRV:
				print_log(fp, next_event);
				sys_time = next_event.time;
				disk1_arrival_handler(sys, events, next_event);
				break;
			case DISK1_FIN:
				print_log(fp, next_event);
				sys_time = next_event.time;
				disk1_finish_handler(events, next_event);
				break;
			case DISK2_ARRV:
				print_log(fp, next_event);
				sys_time = next_event.time;
				disk2_arrival_handler(sys, events, next_event);
				break;
			case DISK2_FIN:
				print_log(fp, next_event);
				sys_time = next_event.time;
				disk2_finish_handler(events, next_event);
				break;
			case NETWORK_ARRV:
				print_log(fp, next_event);
				sys_time = next_event.time;
				network_arrival_handler(sys, events, next_event);
				break;
			case NETWORK_FIN:
				print_log(fp, next_event);
				sys_time = next_event.time;
				network_finish_handler(events, next_event);
				break;
			case SYS_FIN:
				print_log(fp, next_event);
				sys_time = next_event.time;
				system_finish_handler(sys, next_event);
				break;
			default:
				break;
		}		
	}

	cout << "Network Remaining: " << NETWORK.size() << "\n";
	cout << "CPU Remaining: " << CPU.size() << "\n";
	cout << "DISK1 Remaining: " << DISK1.size() << "\n";
	cout << "DISK2 Remaining: " << DISK2.size() << "\n";

	cout << "\n" << "Total Jobs: " << job_num << " Total Jobs Left: " << job_left << " Total Jobs Enter Network: " << job_enter_network << "\n";
	
	fclose(fp);

	return 0;
}

void job_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

	Event new_event;
	int next_arrival_time;

	// Determine time for next arrival, push onto queue.
	next_arrival_time = gen_rand(sys.ARRIVE_MAX, sys.ARRIVE_MIN) + sys_time;
	new_event = create_event(next_arrival_time, PROCESS_ARRV, job_num);
	events.push(new_event);
	job_num++;

	// Set the event type to CPU_ARRV.
	next_event.type = CPU_ARRV;
	
	// If the CPU is not occupied.
	if(!CPU_BUSY && CPU.empty()){
		// Push the event onto the priority queue.
		events.push(next_event);	
	}else{
		// Else push the event onto the CPU queue to wait.
		CPU.push(next_event);
	}
}

void job_finish_handler(Event next_event){
	job_left++;
}

void cpu_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

	// When new job arrives in CPU, CPU is busy.
	CPU_BUSY = true;
	
	int cpu_time;
	Event new_event;

	cpu_time = gen_rand(sys.CPU_MAX, sys.CPU_MIN) + sys_time;
	new_event = create_event(cpu_time, CPU_FIN, next_event.pid);
	events.push(new_event);
}

void cpu_finish_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

	CPU_BUSY = false;
	Event next_cpu_event;

	double prob = rand_prob();

	if(prob < sys.QUIT_PROB){
		next_event.type = PROCESS_EXIT;
		events.push(next_event);
	}
	else{	
		if((prob = rand_prob()) < sys.NETWORK_PROB){
			next_event.type = NETWORK_ARRV;
			if(!NETWORK_BUSY && NETWORK.empty()){
				events.push(next_event);
			}
			else NETWORK.push(next_event);
		}	
		else{
			
			if(DISK1.size() < DISK2.size()){
				cout << "At time " << next_event.time << ": Job " << next_event.pid << " went to DISK1 because size is " << DISK1.size() << "\n";
				next_event.type = DISK1_ARRV;
				if(!DISK1_BUSY && DISK1.empty()){
					events.push(next_event);	
				}else DISK1.push(next_event);
			}	
			else if(DISK2.size() < DISK1.size()){
				 cout << "At time " << next_event.time << ": Job " << next_event.pid << " went to DISK2 because size is " << DISK2.size() << "\n";
				next_event.type = DISK2_ARRV;
				if(!DISK2_BUSY && DISK2.empty()){
					events.push(next_event);
				}else DISK2.push(next_event);
			}
			else{
				if((prob = rand_prob()) < 0.5){
					cout << "At time " << next_event.time << ": Job " << next_event.pid << " went to DISK1 because size are equal " << DISK1.size() << " " <<
                                                DISK2.size() << "\n";
					next_event.type = DISK1_ARRV;
					if(!DISK1_BUSY && DISK1.empty()){
                                        	events.push(next_event);
                                	}else DISK1.push(next_event);
				}
				else{
					cout << "At time " << next_event.time << ": Job " << next_event.pid << " went to DISK2 because size are equal " << DISK1.size() << " " <<
                                                DISK2.size() << "\n";
					next_event.type = DISK2_ARRV;
                                	if(!DISK2_BUSY && DISK2.empty()){
                                        	events.push(next_event);
                                	}else DISK2.push(next_event);
				}			
			}
		}
	}

	if(!CPU.empty()){
		next_cpu_event = CPU.front();
		next_cpu_event.time = sys_time;
		CPU.pop();
		events.push(next_cpu_event);
	}
}

void disk1_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event){
	
	DISK1_BUSY = true;

	int disk1_finish_time = gen_rand(sys.DISK1_MAX, sys.DISK1_MIN) + sys_time;

	Event new_event = create_event(disk1_finish_time, DISK1_FIN, next_event.pid);
	events.push(new_event);
}

void disk1_finish_handler(priority_queue<Event>& events, Event next_event){

	DISK1_BUSY = false;

	Event next_disk1_event;

	next_event.type = CPU_ARRV;

	if(!CPU_BUSY && CPU.empty()){
		events.push(next_event);
	}
	else CPU.push(next_event);

	if(!DISK1.empty()){
		next_disk1_event = DISK1.front();
		next_disk1_event.time = sys_time;
		DISK1.pop();
		events.push(next_disk1_event);
	}
}

void disk2_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

	DISK2_BUSY = true;

	int disk2_finish_time = gen_rand(sys.DISK2_MAX, sys.DISK2_MIN) + sys_time;

	Event new_event = create_event(disk2_finish_time, DISK2_FIN, next_event.pid);
	events.push(new_event);	
}

void disk2_finish_handler(priority_queue<Event>& events, Event next_event){
	
	DISK2_BUSY = false;

	Event next_disk2_event;

	next_event.type = CPU_ARRV;

	if(!CPU_BUSY && CPU.empty()){
		events.push(next_event);
	}
	else CPU.push(next_event);

	if(!DISK2.empty()){
		next_disk2_event = DISK2.front();
		next_disk2_event.time = sys_time;
		DISK2.pop();
		events.push(next_disk2_event);
	}
}

void network_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event){
	
	NETWORK_BUSY = true;
	job_enter_network++;

	int network_finish_time = gen_rand(sys.NETWORK_MAX, sys.NETWORK_MIN) + sys_time;
	
	Event new_event = create_event(network_finish_time, NETWORK_FIN, next_event.pid);

	events.push(new_event);
}

void network_finish_handler(priority_queue<Event>& events, Event next_event){

	// When a job finishes, the Network is now free.
	NETWORK_BUSY = false;
	// This event dictates the next job entering the network.
	Event next_network_event;

	// For the current event, send back to CPU.
	next_event.type = CPU_ARRV;
	// Check is CPU is free to enter, otherwise put into CPU queue.
	if(!CPU_BUSY && CPU.empty()){
		events.push(next_event);
	}else CPU.push(next_event);

	// If the NETWORK queue is not empty, pop the next event and add to priority queue.
	if(!NETWORK.empty()){
		next_network_event = NETWORK.front();
		next_network_event.time = sys_time;
		NETWORK.pop();
		events.push(next_network_event);
	}
}

void system_finish_handler(FileParam sys, Event next_event){
	
}

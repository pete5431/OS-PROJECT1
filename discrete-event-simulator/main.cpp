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
// The jobs that exited the system.
int job_left = 0;

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
queue<Event> Network;

void job_arrival_handler(FileParam, priority_queue<Event>&, Event);
void job_finish_handler(Event);
void system_finish_handler(FileParam, Event);
void cpu_arrival_handler(FileParam, priority_queue<Event>&, Event);
void cpu_finish_handler(FileParam, priority_queue<Event>&, Event);
void disk1_arrival_handler(FileParam, priority_queue<Event>&, Event);

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

	while(i < 50){
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
	while(!events.empty() && sys_time != sys.FIN_TIME){
		
		next_event = events.top();
		events.pop();

		switch(next_event.type){
			case PROCESS_ARRV:
				print_log(next_event);
				sys_time = next_event.time;
				job_arrival_handler(sys, events, next_event);
				break;
			case PROCESS_EXIT:
				print_log(next_event);
				sys_time = next_event.time;
				job_finish_handler(next_event);
				break;
			case CPU_ARRV:
				print_log(next_event);
				sys_time = next_event.time;
				cpu_arrival_handler(sys, events, next_event);
				break;
			case CPU_FIN:
				print_log(next_event);
				sys_time = next_event.time;
				cpu_finish_handler(sys, events, next_event);
				break;
			case DISK1_ARRV:
				break;
			case DISK1_FIN:
				break;
			case DISK2_ARRV:
				break;
			case DISK2_FIN:
				break;
			case NETWORK_ARRV:
				break;
			case NETWORK_FIN:
				break;
			case SYS_FIN:
				print_log(next_event);
				sys_time = next_event.time;
				system_finish_handler(sys, next_event);
				break;
			default:
				break;
		}		
	}

	
	while(!CPU.empty()){
		Event e = CPU.front();
		CPU.pop();
		cout << e.pid << "\n";
	}

	cout << "\n" << "Total Jobs: " << job_num << " Total Jobs Left: " << job_left << "\n";
	

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

	// Create new event for when the event would arrive in CPU.
	new_event = create_event(next_event.time, CPU_ARRV, next_event.pid);
	
	// If the CPU is not occupied.
	if(!CPU_BUSY && CPU.empty()){
		// Push the new event onto the priority queue.
		events.push(new_event);	
	}else{
		// Else push the event onto the CPU queue to wait.
		CPU.push(new_event);
	}
}

void job_finish_handler(Event next_event){
	 cout << "At time " << next_event.time << ": Job " << next_event.pid << " has left the system\n";
	job_left++;
}

void cpu_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

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
	Event job_finished;

	double prob = rand_prob();

	if(prob < sys.QUIT_PROB){
		job_finished = create_event(next_event.time, PROCESS_EXIT, next_event.pid);
		events.push(job_finished);
	}
	else{	
		if((prob = rand_prob()) < sys.NETWORK_PROB){
			cout << "At time " << next_event.time << ": Job " << next_event.pid << " went to NETWORK\n";
		}	
		else{
			if(DISK1.size() < DISK2.size()){
				 cout << "At time " << next_event.time << ": Job " << next_event.pid << " went to DISK1\n";
			}	
			else if(DISK2.size() < DISK1.size()){
				 cout << "At time " << next_event.time << ": Job " << next_event.pid << " went to DISK2\n";
			}
			else{
				if((prob = rand_prob()) < 0.5)
					 cout << "At time " << next_event.time << ": Job " << next_event.pid << " went to DISK1\n";
				else  cout << "At time " << next_event.time << ": Job " << next_event.pid << " went to DISK2\n";
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
	int disk1_arrival_time = gen_rand(sys.DISK1_MAX, sys.DISK1_MIN) + sys_time;

	Event new_event = create_event(disk1_arrival_time, DISK1_ARRV, next_event.pid);
	events.push(new_event);
	
}

void system_finish_handler(FileParam sys, Event next_event){
	
}

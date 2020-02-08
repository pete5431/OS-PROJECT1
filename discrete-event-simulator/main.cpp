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
queue<Event> Disk1;
queue<Event> Disk2;
queue<Event> Network;

void job_arrival_handler(FileParam, priority_queue<Event>&, Event);
void job_finish_handler(Event);
void system_finish_handler(FileParam, Event);
void cpu_arrival_handler(FileParam, priority_queue<Event>&, Event);
void cpu_finish_handler(FileParam, priority_queue<Event>&, Event);

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
			case CPU_FIN:
				cpu_finish_handler(sys, events, next_event);
				break;
			case SYS_FIN:
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
	

	return 0;
}

void job_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

	// Print the event when job arrives.
	print_log(next_event);
	// Set system time to the event arrival time.
	sys_time = next_event.time;

	// Determine time for next arrival, push onto queue.
	int next_arrival_time = gen_rand(sys.ARRIVE_MAX, sys.ARRIVE_MIN) + sys_time;
	Event new_event = create_event(next_arrival_time, PROCESS_ARRV, job_num);
	events.push(new_event);
	job_num++;
	
	// Pass the current arrival event onto the cpu arrival handler.
	cpu_arrival_handler(sys, events, next_event);
}

void job_finish_handler(Event next_event){
	
	cout << "Job " << next_event.pid << " has left the system.\n";

}

void cpu_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event){
	
	int cpu_time;
	Event new_event;

	// If the CPU queue is not busy.
	if(!CPU_BUSY){
		CPU_BUSY = true;
		new_event = create_event(next_event.time, CPU_ARRV, next_event.pid);
		print_log(new_event);
		cpu_time = gen_rand(sys.CPU_MAX, sys.CPU_MIN) + sys_time;
		new_event = create_event(cpu_time, CPU_FIN, next_event.pid);
		events.push(new_event);
	}
	else CPU.push(next_event);

}

void cpu_finish_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

	print_log(next_event);
	CPU_BUSY = false;
	Event next_cpu_event;

	job_finish_handler(next_event);

	if(!CPU.empty()){
		next_cpu_event = CPU.front();
		CPU.pop();
		cpu_arrival_handler(sys, events, next_cpu_event);
	}
}

void system_finish_handler(FileParam sys, Event next_event){
	sys_time = sys.FIN_TIME;
	print_log(next_event);
}

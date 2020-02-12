#include <iostream>
#include <queue>
#include <cstdlib>
#include <stdio.h>
#include "FileParam.h"
#include "Event.h"
using namespace std;

// The System time.
int sys_time;

// The component occupied status.
bool CPU_BUSY = false;
bool DISK1_BUSY = false;
bool DISK2_BUSY = false;
bool NETWORK_BUSY = false;

// enum for the types of events.
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

// Global queues for each component.
queue<Event> CPU;
queue<Event> DISK1;
queue<Event> DISK2;
queue<Event> NETWORK;

// All functions behave as their name dictates.
void job_arrival_handler(FileParam, priority_queue<Event>&, Event, int job_num);
void cpu_arrival_handler(FileParam&, priority_queue<Event>&, Event);
void cpu_finish_handler(FileParam, priority_queue<Event>&, Event);
void disk1_arrival_handler(FileParam&, priority_queue<Event>&, Event);
void disk1_finish_handler(FileParam, priority_queue<Event>&, Event);
void disk2_arrival_handler(FileParam&, priority_queue<Event>&, Event);
void disk2_finish_handler(FileParam, priority_queue<Event>&, Event);
void network_arrival_handler(FileParam&, priority_queue<Event>&, Event);
void network_finish_handler(FileParam, priority_queue<Event>&, Event);
void calculate_statistics(FileParam&, int previous_time, int current_time);
void write_constants(FILE* fp, FileParam);

int main(){

	// Object FileParam that holds all the constants and statistics values.
	FileParam sys;

	// Read in the constants from file.
	sys.read_file((char*)"config.txt");

	// Set the system time to INIT_TIME.
	sys_time = sys.INIT_TIME;	

	// Set the seed to SEED.
	srand(sys.SEED);

	// Declare priority queue.
	priority_queue<Event> events;	

	// Initialize file pointer to file for writing to log.
	FILE* fp = fopen("log.txt", "w");
	// Writes the constants from FileParam to log.
	write_constants(fp, sys);

	if(fp == NULL){
		perror("Error: ");
		exit(1);
	}

	// Creates the finish system event and pushes to priority queue. 

	Event finish = create_event(sys.FIN_TIME, SYS_FIN, 0);

	events.push(finish);

	// The job number.
	int job_num = 1;
	// Create first job.
	int next_arrival_time = gen_rand(sys.ARRIVE_MAX, sys.ARRIVE_MIN);
	Event next_event;

	next_event = create_event(next_arrival_time, PROCESS_ARRV, job_num);
	events.push(next_event);
	job_num++;

	// Used to the total time passed by saving time of previous event.
	int previous_time = 0;

	// While the priority queue is not empty and the system time is not equal to FIN_TIME.
	while(!events.empty() && sys_time != sys.FIN_TIME){
		
		// Pop next event.	
		next_event = events.top();
		events.pop();
		// Calculate statistics for the queues.
		calculate_statistics(sys, previous_time, next_event.time);
		previous_time = next_event.time;

		// Handle based off event type. Increments jobs completed upon getting FIN type. 
		// Sets the system time to the popped event time. Print the event to log.
		switch(next_event.type){
			case PROCESS_ARRV:
				print_log(fp, next_event);
				sys_time = next_event.time;
				job_arrival_handler(sys, events, next_event, job_num);
				job_num++;
				break;
			case PROCESS_EXIT:
				print_log(fp, next_event);
				sys_time = next_event.time;
				(sys.CPU_JOBS_COMPLETE)++;
				break;
			case CPU_ARRV:
				print_log(fp, next_event);
				sys_time = next_event.time;
				cpu_arrival_handler(sys, events, next_event);
				break;
			case CPU_FIN:
				print_log(fp, next_event);
				sys_time = next_event.time;
				(sys.CPU_JOBS_COMPLETE)++;
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
				(sys.DISK1_JOBS_COMPLETE)++;
				disk1_finish_handler(sys, events, next_event);
				break;
			case DISK2_ARRV:
				print_log(fp, next_event);
				sys_time = next_event.time;
				disk2_arrival_handler(sys, events, next_event);
				break;
			case DISK2_FIN:
				print_log(fp, next_event);
				sys_time = next_event.time;
				(sys.DISK2_JOBS_COMPLETE)++;
				disk2_finish_handler(sys, events, next_event);
				break;
			case NETWORK_ARRV:
				print_log(fp, next_event);
				sys_time = next_event.time;
				network_arrival_handler(sys, events, next_event);
				break;
			case NETWORK_FIN:
				print_log(fp, next_event);
				sys_time = next_event.time;
				(sys.NETWORK_JOBS_COMPLETE)++;
				network_finish_handler(sys, events, next_event);
				break;
			case SYS_FIN:
				print_log(fp, next_event);
				sys_time = next_event.time;
				break;
			default:
				break;
		}		
	}

	// Write all finalized statistics to "statistics.txt".
	write_statistic_file(sys);

	cout << "Job_num: " << job_num << "\n";

	fclose(fp);

	return 0;
}

void job_arrival_handler(FileParam sys, priority_queue<Event>& events, Event next_event, int job_num){

	Event new_event;
	int next_arrival_time;

	// Determine time for next arrival, push onto queue.
	next_arrival_time = gen_rand(sys.ARRIVE_MAX, sys.ARRIVE_MIN) + sys_time;
	new_event = create_event(next_arrival_time, PROCESS_ARRV, job_num);
	events.push(new_event);

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

void cpu_arrival_handler(FileParam& sys, priority_queue<Event>& events, Event next_event){

	// When new job arrives in CPU, CPU is busy.
	CPU_BUSY = true;
	
	int cpu_time;
	int response_time;
	Event new_event;

	cpu_time = gen_rand(sys.CPU_MAX, sys.CPU_MIN) + sys_time;
	new_event = create_event(cpu_time, CPU_FIN, next_event.pid);
	events.push(new_event);

	//Increment CPU job arrival count.
	(sys.CPU_ARRV_COUNT)++;
	// Calculate determined reponse time for each job entering CPU.
	response_time = cpu_time - next_event.time;
	sys.CPU_RESPONSE_SUM += response_time;
	if(sys.CPU_MAX_RESPONSE < response_time){
		sys.CPU_MAX_RESPONSE = response_time;
	}
}

void cpu_finish_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

	// Set the CPU to not occupied.
	CPU_BUSY = false;
	// The next event popped from CPU queue if queue not empty.
	Event next_cpu_event;

	// The probability to handler quit prob and network prob.
	double prob = rand_prob();

	// If process exits, then push the event onto queue.
	if(prob < sys.QUIT_PROB){
		next_event.type = PROCESS_EXIT;
		events.push(next_event);
	}
	else{
		prob = rand_prob();	
		// else check if it goes to network or either of the disks based off shorter queue size.
		if(prob < sys.NETWORK_PROB){
			next_event.type = NETWORK_ARRV;
			if(!NETWORK_BUSY && NETWORK.empty()){
				events.push(next_event);
			}
			else NETWORK.push(next_event);
		}	
		else if(DISK1.size() < DISK2.size()){
			next_event.type = DISK1_ARRV;
		}	
		else if(DISK2.size() < DISK1.size()){
			next_event.type = DISK2_ARRV;
		}
		else{
			// If disk sizes are equal, then picks one at pseudo-randomness 50/50.
			prob = rand_prob();
			if(prob < 0.5){
				next_event.type = DISK1_ARRV;
			}
			else next_event.type = DISK2_ARRV;			
		}
	}

	// If type was changed to DISK1 or DISK2, push event onto queue if not component is not busy.
	if(next_event.type == DISK1_ARRV){
		if(!DISK1_BUSY && DISK1.empty()){
			events.push(next_event);
		}else{
			DISK1.push(next_event);
		}
	}
	else if(next_event.type == DISK2_ARRV){
		if(!DISK2_BUSY && DISK2.empty()){
			events.push(next_event);
		}else{
			DISK2.push(next_event);
		}
	}

	// If CPU queue is not empty, pop the next event, and set time to current and push to queue.
	if(!CPU.empty()){
		next_cpu_event = CPU.front();
		next_cpu_event.time = sys_time;
		CPU.pop();
		events.push(next_cpu_event);
	}
}

void disk1_arrival_handler(FileParam& sys, priority_queue<Event>& events, Event next_event){
	
	int response_time;
	DISK1_BUSY = true;

	int disk1_finish_time = gen_rand(sys.DISK1_MAX, sys.DISK1_MIN) + sys_time;

	Event new_event = create_event(disk1_finish_time, DISK1_FIN, next_event.pid);
	events.push(new_event);

	//Increment DISK1 job arrival count.
        (sys.DISK1_ARRV_COUNT)++;
        // Calculate determined reponse time for each job entering DISK1.
        response_time = disk1_finish_time - next_event.time;
        sys.DISK1_RESPONSE_SUM += response_time;
        if(sys.DISK1_MAX_RESPONSE < response_time){
                sys.DISK1_MAX_RESPONSE = response_time;
        }

}

void disk1_finish_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

	DISK1_BUSY = false;

	Event next_disk1_event;

	next_event.type = CPU_ARRV;

	if(!CPU_BUSY && CPU.empty()){
		events.push(next_event);
	}
	else{
		CPU.push(next_event);
	}

	if(!DISK1.empty()){
		next_disk1_event = DISK1.front();
		next_disk1_event.time = sys_time;
		DISK1.pop();
		events.push(next_disk1_event);
	}
}

void disk2_arrival_handler(FileParam& sys, priority_queue<Event>& events, Event next_event){

	int response_time;
	DISK2_BUSY = true;

	int disk2_finish_time = gen_rand(sys.DISK2_MAX, sys.DISK2_MIN) + sys_time;

	Event new_event = create_event(disk2_finish_time, DISK2_FIN, next_event.pid);
	events.push(new_event);	

	//Increment DISK2 job arrival count.
        (sys.DISK2_ARRV_COUNT)++;
        // Calculate determined reponse time for each job entering DISK2.
        response_time = disk2_finish_time - next_event.time;
        sys.DISK2_RESPONSE_SUM += response_time;
        if(sys.DISK2_MAX_RESPONSE < response_time){
                sys.DISK2_MAX_RESPONSE = response_time;
        }
}

void disk2_finish_handler(FileParam sys, priority_queue<Event>& events, Event next_event){
	
	DISK2_BUSY = false;

	Event next_disk2_event;

	next_event.type = CPU_ARRV;

	if(!CPU_BUSY && CPU.empty()){
		events.push(next_event);
	}
	else{
		CPU.push(next_event);
	}

	if(!DISK2.empty()){
		next_disk2_event = DISK2.front();
		next_disk2_event.time = sys_time;
		DISK2.pop();
		events.push(next_disk2_event);
	}
}

void network_arrival_handler(FileParam& sys, priority_queue<Event>& events, Event next_event){
	
	int response_time;
	NETWORK_BUSY = true;

	int network_finish_time = gen_rand(sys.NETWORK_MAX, sys.NETWORK_MIN) + sys_time;
	
	Event new_event = create_event(network_finish_time, NETWORK_FIN, next_event.pid);
	events.push(new_event);

	//Increment NETWORK job arrival count.
        (sys.NETWORK_ARRV_COUNT)++;
        // Calculate determined reponse time for each job entering NETWORK.
        response_time = network_finish_time - next_event.time;
        sys.NETWORK_RESPONSE_SUM += response_time;
        if(sys.NETWORK_MAX_RESPONSE < response_time){
                sys.NETWORK_MAX_RESPONSE = response_time;
        }
}

void network_finish_handler(FileParam sys, priority_queue<Event>& events, Event next_event){

	// When a job finishes, the Network is now free.
	NETWORK_BUSY = false;
	// This event dictates the next job entering the network.
	Event next_network_event;

	// For the current event, send back to CPU.
	next_event.type = CPU_ARRV;
	// Check is CPU is free to enter, otherwise put into CPU queue.
	if(!CPU_BUSY && CPU.empty()){
		events.push(next_event);
	}
	else{
		CPU.push(next_event);
	}

	// If the NETWORK queue is not empty, pop the next event and add to priority queue.
	if(!NETWORK.empty()){
		next_network_event = NETWORK.front();
		next_network_event.time = sys_time;
		NETWORK.pop();
		events.push(next_network_event);
	}
}

void calculate_statistics(FileParam& sys, int previous_time, int current_time){

	// The amount of time passed from the last event to the current.	
	int time_passed = current_time - previous_time;

	// Calculate max queue sizes;
	if(sys.CPU_MAX_SIZE < CPU.size()){
		sys.CPU_MAX_SIZE = CPU.size();
	}
	if(sys.DISK1_MAX_SIZE < DISK1.size()){
                sys.DISK1_MAX_SIZE = DISK1.size();
        }
	if(sys.DISK2_MAX_SIZE < DISK2.size()){
		sys.DISK2_MAX_SIZE = DISK2.size();
	}
	if(sys.NETWORK_MAX_SIZE < NETWORK.size()){
		sys.NETWORK_MAX_SIZE = NETWORK.size();
	}

	// Calculate the sum of queue sizes at each unit of time.
	// Will be used to calculate average.
	sys.CPU_QUEUE_SUM += (CPU.size() * time_passed);
        sys.DISK1_QUEUE_SUM += (DISK1.size() * time_passed);
        sys.DISK2_QUEUE_SUM += (DISK2.size() * time_passed);
        sys.NETWORK_QUEUE_SUM += (NETWORK.size() * time_passed);

	// Calculate the time the queues were busy.
	if(CPU_BUSY){
		sys.CPU_BUSY_TOTAL += time_passed;
	}
	if(DISK1_BUSY){
		sys.DISK1_BUSY_TOTAL += time_passed;
	}
	if(DISK2_BUSY){
		sys.DISK2_BUSY_TOTAL += time_passed;
	}
	if(NETWORK_BUSY){
		sys.NETWORK_BUSY_TOTAL += time_passed;
	}
}

void write_constants(FILE* fp, FileParam sys){
	// Print constants into log file.
	fprintf(fp, "SEED %d\n", sys.SEED);
	fprintf(fp, "INIT_TIME %d\n", sys.INIT_TIME);
	fprintf(fp, "FIN_TIME %d\n", sys.FIN_TIME);
	fprintf(fp, "ARRIVE_MIN %d\n", sys.ARRIVE_MIN);
	fprintf(fp, "ARRIVE_MAX %d\n", sys.ARRIVE_MAX);
	fprintf(fp, "QUIT_PROB %f\n", sys.QUIT_PROB);
	fprintf(fp, "NETWORK_PROB %f\n", sys.NETWORK_PROB);
	fprintf(fp, "CPU_MIN %d\n", sys.CPU_MIN);
	fprintf(fp, "CPU_MAX %d\n", sys.CPU_MAX);
	fprintf(fp, "DISK1_MIN %d\n", sys.DISK1_MIN);
	fprintf(fp, "DISK1_MAX %d\n", sys.DISK1_MAX);
	fprintf(fp, "DISK2_MIN %d\n", sys.DISK2_MIN);
	fprintf(fp, "DISK2_MAX %d\n", sys.DISK2_MAX);
	fprintf(fp, "NETWORK_MIN %d\n", sys.NETWORK_MIN);
	fprintf(fp, "NETWORK_MAX %d\n", sys.NETWORK_MAX);
}

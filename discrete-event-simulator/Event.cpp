#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include "Event.h"
using namespace std;

// enum containing the event types.
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

// The < is the less-relationship on the priority, so by default it would order it in descending order.
// The override tells it to instead to sort by time in ascending order.
bool Event::operator < (const Event& e) const{
	return time > e.time; 
}

// Returns a random double between 0 and 1.
double rand_prob(){
	return ((double)rand() / (double)RAND_MAX);
}

// Returns random number in the range max to min.
int gen_rand(int max, int min){
	return rand() % ((max + 1) - min) + min;
}

// Creates new events.
Event create_event(int time, int type, int id){
	Event new_job = {time, type, id};
	return new_job;
}

// Prints the events to the log file.
void print_log(FILE* fp, Event e){

	switch(e.type){
		case PROCESS_ARRV:
			fprintf(fp, "At time %d, Job %d arrives.\n", e.time, e.pid);
			break;
		case PROCESS_EXIT:
			fprintf(fp, "At time %d, Job %d has exited the system.\n", e.time, e.pid);
			break;
		case CPU_ARRV:
			fprintf(fp, "At time %d, Job %d entered CPU.\n", e.time, e.pid);
			break;
		case CPU_FIN:
			fprintf(fp, "At time %d, Job %d finished in CPU.\n", e.time, e.pid);
			break;
		case DISK1_ARRV:
			fprintf(fp, "At time %d, Job %d entered DISK1.\n", e.time, e.pid);
			break;
		case DISK1_FIN:
			fprintf(fp, "At time %d, Job %d finished in DISK1.\n", e.time, e.pid);
			break;
		case DISK2_ARRV:
			fprintf(fp, "At time %d, Job %d entered DISK2.\n", e.time, e.pid);
			break;
		case DISK2_FIN:
			fprintf(fp, "At time %d, Job %d finished in DISK2.\n", e.time, e.pid);
			break;
		case NETWORK_ARRV:
			fprintf(fp, "At time %d, Job %d entered NETWORK.\n", e.time, e.pid);
			break;
		case NETWORK_FIN:
			fprintf(fp, "At time %d, Job %d finished in NETWORK.\n", e.time, e.pid);
			break;
		case SYS_FIN:
			fprintf(fp, "At time %d, Simulation Finished.\n", e.time);
			break;
		default:
			break;
	}	
}


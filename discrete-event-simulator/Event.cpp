#include <iostream>
#include <cstdlib>
#include "Event.h"
using namespace std;

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

int rand_prob(double prob){

	int range = 100 * prob;

	int result = rand() % 100 + 1;

	if(result > range)
		return 0;
	else return 1;
}

int gen_rand(int max, int min){

	return rand() % (max + 1 - min) + min;
}

Event create_event(int time, int type, int id){

	Event new_job = {time, type, id};

	return new_job;
}

void print_log(Event e){
	switch(e.type){
		case PROCESS_ARRV:
			cout << "At time " << e.time << ": Job " << e.pid << " arrives.\n";
			break;
		case CPU_ARRV:
			cout << "At time " << e.time << ": Job " << e.pid << " arrives in CPU.\n";
			break;
		case CPU_FIN:
			cout << "At time " << e.time << ": Job " << e.pid << " finished in CPU.\n";
			break;
		case SYS_FIN:
			cout << "System exit\n";
			break;
		default:
			break;
	}	
}


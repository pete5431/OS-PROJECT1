#ifndef EVENT_H
#define EVENT_H

// Holds the Event struct and functions relating to Event.

struct Event{
	
	// The time of the event.
	int time;
	// The type of the event.
	int type;
	// The process id.
	int pid;
	// Operator override for priority queue.
	bool operator < (const Event& e) const;
};

double rand_prob();

int gen_rand(int max, int min);

Event create_event(int time, int type, int id);

void print_log(FILE* fp, Event e);

#endif

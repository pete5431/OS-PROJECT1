#ifndef EVENT_H
#define EVENT_H

struct Event{

	int time;
	int type;
	int pid;

	bool operator < (const Event& e) const;
};

double rand_prob();

int gen_rand(int max, int min);

Event create_event(int time, int type, int id);

void print_log(Event e);

#endif

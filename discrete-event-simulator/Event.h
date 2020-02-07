#ifndef EVENT_H
#define EVENT_H

struct Event{

	int time;
	int type;
	int pid;

	bool operator < (const Event& e) const;
};

int rand_prob(double prob);

int gen_rand(int max, int min);

Event create_event(int time, int type, int id);

void print_log(Event e);

#endif

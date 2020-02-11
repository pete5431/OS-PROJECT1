#ifndef FILEPARAM_H
#define FILEPARAM_H

struct FileParam{
	
	int SEED;
	int INIT_TIME;
	int FIN_TIME;
	int ARRIVE_MIN;
	int ARRIVE_MAX;
	double QUIT_PROB;
	double NETWORK_PROB;
	int CPU_MIN;
	int CPU_MAX;
	int DISK1_MIN;
	int DISK1_MAX;
	int DISK2_MIN;
	int DISK2_MAX;
	int NETWORK_MIN;
	int NETWORK_MAX;

	double CPU_AVERAGE_SIZE;
	double DISK1_AVERAGE_SIZE;
	double DISK2_AVERAGE_SIZE;
	double NETWORK_AVERAGE_SIZE;

	double CPU_QUEUE_SUM;
	double DISK1_QUEUE_SUM;
	double DISK2_QUEUE_SUM;
	double NETWORK_QUEUE_SUM;

	int CPU_CHANGED;
	int DISK1_CHANGED;
	int DISK2_CHANGED;	
	int NETWORK_CHANGED;

	int CPU_MAX_SIZE;
	int DISK1_MAX_SIZE;
	int DISK2_MAX_SIZE;
	int NETWORK_MAX_SIZE;

	FileParam();

	void read_file(char* name);
	double calculate_queue_average();
};

#endif

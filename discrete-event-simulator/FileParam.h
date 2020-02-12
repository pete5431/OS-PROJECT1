#ifndef FILEPARAM_H
#define FILEPARAM_H

struct FileParam{
	
	// Constants
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

	// Statistic values
	double CPU_QUEUE_SUM;
	double DISK1_QUEUE_SUM;
	double DISK2_QUEUE_SUM;
	double NETWORK_QUEUE_SUM;

	int CPU_MAX_SIZE;
	int DISK1_MAX_SIZE;
	int DISK2_MAX_SIZE;
	int NETWORK_MAX_SIZE;

	int CPU_MAX_RESPONSE;
	int DISK1_MAX_RESPONSE;
	int DISK2_MAX_RESPONSE;
	int NETWORK_MAX_RESPONSE;

	int CPU_ARRV_COUNT;
	int DISK1_ARRV_COUNT;
	int DISK2_ARRV_COUNT;
	int NETWORK_ARRV_COUNT;

	double CPU_RESPONSE_SUM;
	double DISK1_RESPONSE_SUM;
	double DISK2_RESPONSE_SUM;
	double NETWORK_RESPONSE_SUM;

	double CPU_BUSY_TOTAL;
	double DISK1_BUSY_TOTAL;
	double DISK2_BUSY_TOTAL;
	double NETWORK_BUSY_TOTAL;
	
	int CPU_JOBS_COMPLETE;
	int DISK1_JOBS_COMPLETE;
	int DISK2_JOBS_COMPLETE;
	int NETWORK_JOBS_COMPLETE;

	// Constructor for FileParam.
	FileParam();
	// Reads file for constants.
	void read_file(char* name);
};

void write_statistic_file(FileParam sys);

#endif

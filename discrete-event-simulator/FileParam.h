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

	void read_file(char* name);
	
};

#endif

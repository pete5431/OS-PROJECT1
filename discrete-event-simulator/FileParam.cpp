#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "FileParam.h"
using namespace std;

FileParam::FileParam(){

	CPU_QUEUE_SUM = 0.0;
        DISK1_QUEUE_SUM = 0.0;
        DISK2_QUEUE_SUM = 0.0;
        NETWORK_QUEUE_SUM = 0.0;

        CPU_MAX_SIZE = 0;
        DISK1_MAX_SIZE = 0;
        DISK2_MAX_SIZE = 0;
        NETWORK_MAX_SIZE = 0;

        CPU_MAX_RESPONSE = 0;
        DISK1_MAX_RESPONSE = 0;
        DISK2_MAX_RESPONSE = 0;
        NETWORK_MAX_RESPONSE = 0;

        CPU_RESPONSE_SUM = 0.0;
        DISK1_RESPONSE_SUM = 0.0;
        DISK2_RESPONSE_SUM = 0.0;
        NETWORK_RESPONSE_SUM = 0.0;

	CPU_ARRV_COUNT = 0;
	DISK1_ARRV_COUNT = 0;
	DISK2_ARRV_COUNT = 0;
	NETWORK_ARRV_COUNT = 0;

        CPU_BUSY_TOTAL = 0;
        DISK1_BUSY_TOTAL = 0;
        DISK2_BUSY_TOTAL = 0;
        NETWORK_BUSY_TOTAL = 0;

        CPU_JOBS_COMPLETE = 0;
        DISK1_JOBS_COMPLETE = 0;
        DISK2_JOBS_COMPLETE = 0;
        NETWORK_JOBS_COMPLETE = 0;

	TOTAL = 0;
}

void FileParam::read_file(char* name){

	FILE* fp;

	fp = fopen(name, "r");
	
	if(fp == NULL){
		perror("Error: ");
		exit(1);
	}

	char temp[50];
	int read_int;
	double read_double;

	fscanf(fp, "%s %d", temp, &read_int);
	this->SEED = read_int;

	fscanf(fp, "%s %d", temp, &read_int);
	this->INIT_TIME = read_int;

	fscanf(fp, "%s %d", temp, &read_int);
	this->FIN_TIME = read_int;

	fscanf(fp, "%s %d", temp, &read_int);
	this->ARRIVE_MIN = read_int;

	fscanf(fp, "%s %d", temp, &read_int);
	this->ARRIVE_MAX = read_int;

	fscanf(fp, "%s %lf", temp, &read_double);
	this->QUIT_PROB = read_double;

	fscanf(fp, "%s %lf", temp, &read_double);

	this->NETWORK_PROB = read_double;

	fscanf(fp, "%s %d", temp, &read_int);
	this->CPU_MIN = read_int;

	fscanf(fp, "%s %d", temp, &read_int);
	this->CPU_MAX = read_int;

	fscanf(fp, "%s %d", temp, &read_int);
	this->DISK1_MIN = read_int;

	fscanf(fp, "%s %d", temp, &read_int);
	this->DISK1_MAX = read_int;

	fscanf(fp, "%s %d", temp, &read_int);
	this->DISK2_MIN = read_int;

	fscanf(fp, "%s %d", temp, &read_int);

	this->DISK2_MAX = read_int;

	fscanf(fp, "%s %d", temp, &read_int);
	
	this->NETWORK_MIN = read_int;

	fscanf(fp, "%s %d", temp, &read_int);

	this->NETWORK_MAX = read_int;	

	fclose(fp);
}

void write_statistic_file(FileParam sys){
	
	FILE* fp = fopen("statistics.txt", "w");

	if(fp == NULL){
		perror("Error: ");
		exit(1);
	} 

	fprintf(fp, "CPU busy time: %f\n", sys.CPU_BUSY_TOTAL);
	fprintf(fp, "Max CPU Queue Size: %d\n", sys.CPU_MAX_SIZE);
	fprintf(fp, "Average CPU Queue Size: %f\n", (sys.CPU_QUEUE_SUM / sys.FIN_TIME));	
	fprintf(fp, "Max CPU Response time: %d\n", sys.CPU_MAX_RESPONSE);
	fprintf(fp, "Average CPU Response time: %f\n", (sys.CPU_RESPONSE_SUM / sys.CPU_ARRV_COUNT));
	fprintf(fp, "Number of jobs CPU completed: %d\n", sys.CPU_JOBS_COMPLETE);
	fclose(fp);
}	


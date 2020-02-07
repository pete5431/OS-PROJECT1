#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "FileParam.h"
using namespace std;

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


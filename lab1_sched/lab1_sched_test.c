/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32161796
*	    Student name : Park_Jung_Sun
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/
#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

int main(int argc, char *argv[]){
	init_workload();
	print_workload();
	scheduling(FCFS_MODE);
	print_result("FCFS");
	scheduling(RR_MODE);
	print_result("RR");
	scheduling(SJF_MODE);
	print_result("SJF");
	scheduling(STCF_MODE);
	print_result("STCF");
	scheduling(MLFQ_MODE);
	print_result("MLFQ");
	scheduling(LOTTERY_MODE);
	print_result("LOTTERY");
	return 0;
}

/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32141183
*	    Student name : Ji Hyung Kim
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

int main(){
	// Initialize workload
	init_workload();
	print_workload();

	// FCFS
	scheduling(FCFS_MODE);
	print_result("FCFS");

	// Robin Round
	scheduling(RR_MODE);
	print_result("RR");

	// SJF
	scheduling(SJF_MODE);
	print_result("SJF");

	// STCF
	scheduling(STCF_MODE);
	print_result("STCF");

	// MLFQ
	scheduling(MLFQ_MODE);
	print_result("MLFQ");

	// Lottery
	scheduling(LOTTERY_MODE);
	print_result("LOTTERY");

	return 0;
}

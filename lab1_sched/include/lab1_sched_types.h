/*
* DKU Operating System Lab
*	  Lab1 (Scheduler Algorithm Simulator)
*	  Student id : 32141183, 32131704
*	  Student name : Ji Hyung Kim, Hong Je Park
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

// lab1_sched_types.h
#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H

// Scheduling mode
#define FCFS_MODE 0
#define RR_MODE 5
#define SJF_MODE 1
#define STCF_MODE 6
#define MLFQ_MODE 7
// Lottery (+a)
#define LOTTERY_MODE 8 // Lottery**

// Queue size
#define QUEUE_SIZE 5

// MLFQ level define
#define MLFQ_LEVEL 5
#define SINGLE_LEVEL 1


// Queue structure
typedef struct QUEUE {
	int front;
	int rear;
	int size;
	int *index;
} Queue	;

// Process structure
typedef struct PROCESS {
	int arrival_time;
	int service_time;
	int remain_time;
	int turnarround_time;
	int response_time;
	int is_complete;
	int level;
	int ticket;
} Process;


// Scheduling function
void scheduling(int);

// Initialization function
void init_workload();
void init_process();
void init_queue(int);
void init_result();

// Queue function
void input_queue(int);
int output_queue();
void del_queue();
int is_queue_empty(int);
int empty_queue();

// Check if the process is new
int is_new_process();
// Input texture for scheudling
void input_result(int);
// Check if the process is finished
int is_finished_process();
// Check if the process is alone
int is_process_alone();

// Print function
void print_workload();
void print_result(char*);
void print_picture();

#endif
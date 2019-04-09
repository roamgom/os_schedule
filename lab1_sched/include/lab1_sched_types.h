/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 3216196
*	    Student name : Park Jung Sun
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
#define FCFS_MODE 0 // FCFS, RR
#define RR_MODE 10 //RR
#define SJF_MODE 1  // SJF SRTF
#define STCF_MODE 11 // SRTF
#define MLFQ_MODE 12 // MLFQ
// 
#define MLFQ_LEVEL 5	// the number of queue level
#define SINGLE_LEVEL 1
// Lottery (+a)
#define LOTTERY_MODE 13 // Lottery**

// Queue size
#define QUEUE_SIZE 5

/* type of PCB, QUEUE */
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
typedef struct QUEUE {
	int front;
	int rear;
	int size;
	int *index;
} Queue	;
/* Schedul_func */
void scheduling(int);
/* init_func */
void init_workload();
void init_process();
void init_queue(int); // init by schedul_mode
void init_result(); // result picture
/* queue_func*/
void input_queue(int);
int output_queue();
void del_queue();
int is_queue_empty(int); // if ready_queue is empty
int empty_queue(); // if every thing is not run (not schedul)
/* print_func */
void print_workload();
void print_result(char*); // result PCB print
void print_picture(); // result schduling picture print
/* others_func */
int is_new_process(); // if new schduling entity come
void input_result(int); // scheduling picture draw
int is_finished_process(); // if all scheduling entity finished
int is_process_alone(); // scheduling entitiy is alone
#endif /* LAB1_HEADER_H*/

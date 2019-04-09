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
typedef struct PROCESS {
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
void init_ready_queue(int); // init by schedul_mode
void init_result(); // result picture
/* queue_func*/
void input_ready_queue(int);
int output_ready_queue();
void del_ready_queue();
int queue_is_empty_full(int); // if ready_queue is empty
int queue_empty(); // if every thing is not run (not schedul)
/* print_func */
void workload_print();
void result_print(char*); // result PCB print
void picture_print(); // result schduling picture print
/* others_func */
int is_arrival_new(); // if new schduling entity come
void input_result(int); // scheduling picture draw
int is_all_finished(); // if all scheduling entity finished
int process_is_alone(); // scheduling entitiy is alone
#endif /* LAB1_HEADER_H*/

/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32161796
*	    Student name : Park_Jung_Sun
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
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

// Process
Process* process;
int process_number = 0;

// Queue
Queue* ready_queue;
int queue_level;

// Schedule setting
int schedule_time = 0;
int schedule_mode;

// 
char **result_picture;
int character_location = -1;

// Set total time for process
const int total_time = 100;
const int time_slice = 1;


void init_workload(){
	/*
	init_workload: Initialize queue to manage process

	:return: Void
	*/
	int i=0;
	printf("process_number your time (maximum: 100)\n");
	printf("number of processes : ");
	scanf("%d", &process_number);
	process = malloc(sizeof(Process)*process_number);
	result_picture = (char**)malloc(sizeof(char*)*process_number);
	for(i=0;i<process_number;i++){
		printf("input %c process workload(arrival_time, service_time, ticket) : ", i+65);
		scanf("%d %d %d", &process[i].arrival_time,&process[i].service_time,&process[i].ticket);
	}

	for(i=0;i<process_number;i++){
		result_picture[i] = (char*)malloc(sizeof(char)*total_time);
	}
}
void init_queue(int mode){
	/*
	init_queue: Initialize queue to manage process
	:mode: Schedule mode

	:return: Void
	*/
	int i =0,k;
	schedule_time = 0;
	schedule_mode = mode;
	if(schedule_mode == MLFQ_MODE){
		// If schedule mode is MLFQ
		queue_level = MLFQ_LEVEL;
	}else{
		queue_level = SINGLE_LEVEL;
	}
	ready_queue = malloc(sizeof(Queue)*queue_level);
	for(i =0;i<queue_level;i++){
		// Initialize queue
		ready_queue[i].front = ready_queue[i].rear = 0;
		ready_queue[i].size = process_number+1;
		ready_queue[i].index = malloc(sizeof(int)*ready_queue[i].size);
		for(k=0;k<QUEUE_SIZE+1;k++){
			ready_queue[i].index[k] = -1;
		}
	}
}
void init_process(){
	/*
	init_process: Initialize queue to manage process

	:return: Void
	*/
	int i =0;
	for(i=0;i<process_number;i++){
		process[i].remain_time = process[i].service_time;
		process[i].turnarround_time = 0;
		process[i].response_time = -1;
		process[i].is_complete =0;
		process[i].level = 0;
	}
}
void init_result(){
	/*
	init_queue: Initialize result

	:return: Void
	*/
	int i =0,k=0;
	character_location =-1;
	for(k=0;k<process_number;k++){
		for(i=0;i<total_time;i++){
			result_picture[k][i]=45;
		}
	}
}


void scheduling(int mode){
	/*
	scheduling: Start scheduling
	:mode: Schedule mode

	:return: Void
	*/

	// run: schedule entity
	// count: time check
	int run = 0, count = 0;

	// Initialize queue and process, result
	init_queue(mode);
	init_process();
	init_result();

	// first schedule
	while(!is_new_process()) schedule_time++;
	run = output_queue();
	while(1){
    input_result(run);
		if(process[run].response_time == -1){
			// arrival_time check
			process[run].response_time = schedule_time - process[run].arrival_time;
		}
		process[run].remain_time--;
		schedule_time++; count++;
		is_new_process();
		if(process[run].remain_time ==0){
			// when process is complete
			process[run].turnarround_time = schedule_time - process[run].arrival_time;
			process[run].is_complete = 1;
			if(!is_finished_process()){ 
				// when all process is complete
				break;
			}
		  run = output_queue();
			while(run==-1) run = empty_queue();
			count = 0;
		}else if(schedule_mode == MLFQ_MODE && count == time_slice){
			// if scheduling is MLFQ, increase the level
			if((!is_process_alone()) && process[run].level<MLFQ_LEVEL-1)
				process[run].level++;
		}
		if((schedule_mode >= 10)&&count == time_slice){
			// if scheduling in [RR, STCF, MLFQ, Lottery], use time slice
			input_queue(run);
			run = output_queue();
			while(run==-1) run = empty_queue();
			count = 0;
		}
	}
	del_queue();
}


void input_result(int i){
	/*
	input_result: Input value as alphabet
	:i: value as a number

	:return: Void
	*/
	result_picture[i][character_location]=i+65;
}
void input_queue(int index){
	/*
	input_queue: Input value into queue by index
	:index: Index of process

	:return: Void
	*/
	int i, in_index;
	i = process[index].level;
	ready_queue[i].rear = (ready_queue[i].rear+1)%ready_queue[i].size;
	in_index = ready_queue[i].rear;
	ready_queue[i].index[in_index] = index;
}
int output_queue(){
	/*
	output_queue: Get the output for queue

	:return: Index of queue
	*/
	int i = 0, out_index;
	if(schedule_mode==MLFQ_MODE){
		// If schedule mode is MLFQ
		for(i=0;i<MLFQ_LEVEL;i++){
			if(!is_queue_empty(i)){
				break;
			}
		}
		if(i==MLFQ_LEVEL){
			i--;
		}
	}
	if(is_queue_empty(i)){
		// If the queue is empty
		return -1;
	}
	ready_queue[i].front = (ready_queue[i].front+1)%ready_queue[i].size;
	if((schedule_mode%10 == 1)&&(!is_queue_empty(i))){
		// If the schedule mode is SJF or STCF
		// swap schedule entity
		int start, end, shortest,time1,time2,temp;
		start = shortest = ready_queue[i].front;
		end = ready_queue[i].rear;
		do{
			start=(start+1)%ready_queue[i].size;
			time1 = ready_queue[i].index[shortest];
			time2 = ready_queue[i].index[start];
			if(process[time1].remain_time > process[time2].remain_time){
				shortest = start;
			}
		}while(start!=end);
		temp = ready_queue[i].index[shortest];
		// Change the shortest and front process
		ready_queue[i].index[shortest] = ready_queue[i].index[ready_queue[i].front];
		ready_queue[i].index[ready_queue[i].front] = temp;
	}
	if((schedule_mode==LOTTERY_MODE)&&(!is_queue_empty(i))){
		// If schedule mode is Lottery
		int total_ticket,select =0,start,end,index,temp;
		total_ticket = 0;
		start = ready_queue[i].front;
		end = (ready_queue[i].rear+1)%ready_queue[i].size;
		for(;start!=end;start=(start+1)%ready_queue[i].size){
			// Get total sum of ticket
			index = ready_queue[i].index[start];
			total_ticket += process[index].ticket;
		}
		srand((unsigned)time(NULL));

		// Random select with total ticket
		select = rand()%total_ticket;
		total_ticket = 0;
		start = ready_queue[i].front;
		end = (ready_queue[i].rear+1)%ready_queue[i].size;
		for(;start!=end;start=(start+1)%ready_queue[i].size){
			// Lottery scheduling
			index = ready_queue[i].index[start];
			total_ticket += process[index].ticket;
			if(select < total_ticket){
				select = start;
				break;
			}
		}
		temp = ready_queue[i].index[select];
		ready_queue[i].index[select] = ready_queue[i].index[ready_queue[i].front];
		ready_queue[i].index[ready_queue[i].front] = temp;
		select = 0;
	}
	out_index = ready_queue[i].front;
	return ready_queue[i].index[out_index];
}
void del_queue(){
	/*
	del_queue: Delete process on queue

	:return: Void
	*/
	int i =0;
	for(i=0;i<queue_level;i++){
		free(ready_queue[i].index);
	}
}

int is_new_process(){
	/*
	is_new_process: Check if the process is new

	:return: Execution time of the process
	*/
	int i = 0, count =0;
	for(i = 0;i<process_number;i++){
		if(process[i].arrival_time == schedule_time){
			input_queue(i);
			count++;
		}
	}
	character_location++;
	return count;
}
int is_finished_process(){
	/*
	init_queue: Check if the process if finished

	:return: Status of the process by finish
	*/
	int i=0;
	int count = 0;
	for(i=0;i<process_number;i++){
		if(process[i].is_complete == 1)
			count++;
	}
	if(count == process_number)
		return 0;
	return 1;
}
int is_queue_empty(int i){
	/*
	is_queue_empty: Check if the queue is empty
	:i: index of queue

	:return: Status of the queue if it's empty
	*/
	if(ready_queue[i].front == ready_queue[i].rear){
		return 1;
	}else{
		return 0;
	}
}
int is_process_alone(){
	/*
	is_process_alone: Check if the process is alone

	:return: Status of the process if it's alone
	*/
	int i, count =0;
	for(i=0;i<queue_level;i++){
		if(is_queue_empty(i)){
			count++;
		}
	}
	if(count == i){
		return 1;
	}
	return 0;
}


void print_workload(){
	/*
	print_workload: Print the workload as text

	:return: Void
	*/
	int i = 0;
	printf("|------------workload-------------|\n");
	printf("|---|-arrival_time-|-service_time-|\n");
	for(i=0;i<process_number;i++){
		printf("|_%c_|_%12d_|_%12d_|\n", i+65, process[i].arrival_time, process[i].service_time);
	}
}
void print_result(char* text){
	/*
	print_result: Print the result as text
	:text: Each process result

	:return: Void
	*/
	int i=0;
	printf("\n\n\n|%-41s|\n",text);
	printf("|---|-turnarround_time-|---respose_time---|\n");
	for(i=0;i<process_number;i++){
		printf("|_%c_|_%16d_|_%16d_|\n", i+65, process[i].turnarround_time, process[i].response_time);
	}
	print_picture();
}
void print_picture(){
	/*
	print_picture: Print scheduling picture

	:return: Void
	*/
	int i=0;
	printf("|Scheduling Picture|\n");
	for(i=0;i<process_number;i++){
		result_picture[i][schedule_time] = '\0';
		printf("|_%c_| %s \n",i+65,result_picture[i]);
	}
}


int empty_queue(){
	/*
	init_queue: Initialize queue to manage process
	:mode: Schedule mode

	:return: Void
	*/
	schedule_time++;
	is_new_process();
	return output_queue();
}

/*
 * Q.4.20 : Operating Systems Concepts, Abraham Siberschatz, 9th Edition
 * ---------------------------------------------------------------------
 * Modify programming problem Exercise 3.20 from Chapter 3, which asks
 * you to design a pid manager. This modification will consist of writing
 * a multithreaded program that tests your solution to Exercise 3.20. You
 * will create a number of threads—for example, 100—and each threadwill
 * request a pid, sleep for a random period of time, and then release the pid.
 * (Sleeping for a random period of time approximates the typical pid usage
 * in which a pid is assigned to a new process, the process executes and
 * then terminates, and the pid is released on the process’s termination.) On
 * UNIX and Linux systems, sleeping is accomplished through the sleep()
 * function, which is passed an integer value representing the number of
 * seconds to sleep. This problem will be modified in Chapter 5.
 */

/*
 * Compile as: >> gcc 02_pid_manager_multithreading.c -l pthread
 * Run as: >> ./a.out
 */

#include <stdio.h> // for printf
#include <stdlib.h> // for malloc,exit,rand,srand
#include <string.h> // for memset
#include <unistd.h> // for sleep
#include <time.h> // for time
#include <pthread.h> // for pthread_create,pthread_join,pthread_attr_init,pthread_attr_destroy

#define MIN_PID 300
#define MAX_PID 5000

// The pids will be stored in an integer array
int	*pid_table; 

/* 
 * Function: allocate_map
 * ----------------------
 * Description: Creates and initializes an array to store the pid's alloted and not alloted.
 *              Returns 1 if successful and -1 if unsuccessful. 
 * args: (void)
 * returns: (int)
 */
int allocate_map(void)
{
	pid_table = malloc((MAX_PID-MIN_PID)*sizeof(int));
	memset(pid_table,0,MAX_PID-MIN_PID);

	if(pid_table==NULL)
	{
		return -1;
	}

	return 1;
}

/* 
 * Function: allocate_pid
 * ----------------------
 * Description: Allocates and returns a pid. Returns 1 if unsuccessful or full. 
 *              The lowest unused pid is set always.
 * args: (void)
 * returns: (int)
 */
int allocate_pid(void)
{
	for(int i=0;i<MAX_PID-MIN_PID;i++)
	{
		if(pid_table[i]==0)
		{
			pid_table[i] = 1;
			return i+MIN_PID;
		}
	}
	return 1;
}

/* 
 * Function: release_pid
 * ----------------------
 * Description: Releases the pid value given to the function if it is a valid pid value.
 * args: (int)
 * returns: (void)
 */
void release_pid(int pid)
{
	if(pid>=MIN_PID && pid<MAX_PID)
	{
		pid_table[pid-MIN_PID] = 0;
	}
	return;
}

void display_pid_table()
{
	printf("PID        STATUS\n");
	printf("---        ------\n");
	for(int i=0;i<MAX_PID-MIN_PID;i++)
	{
		printf("%d          %d\n",i+MIN_PID,pid_table[i]);
	}
	return;
}

/* 
 * Function: run_thread
 * --------------------
 * Description: Runs a thread. Allocates a pid for the thread. Sleeps for a specified amount of time.
 				Deallocates the pid when the thread finishes execution. Mimicks an actual thread execution.
 * Args: void *args
 * Returns: void
 */
void *run_thread(void *args)
{
	// Converting void->int
	int *max_exec_time = args;
	// Allocating a pid to the thread
	int pid = allocate_pid();
	
	// Cheacking if pid allocation was successful.
	if(pid==1)
	{
		printf("PID allocation failed.\n");
		exit(EXIT_FAILURE);		
	}

	// Each thread will run for a maximum of 'max_exec_time' seconds.	
	sleep(rand()%(*max_exec_time)+1);
	// The allocated pid is released just before the thread finishes its execution.
	release_pid(pid);
}

/* 
 * Function: create_threads
 * ----------------------
 * Description: Creates 'num_threads' threads and executes them.
 * Args: int num_threads, int max_exec_time
 * Returns: void
 */
void create_threads(int num_threads,int max_exec_time)
{
	// Array to store the ids of all the threads.s
	pthread_t tid[num_threads];
	// Attribute of the threads. All the threads have the same attributes.
	pthread_attr_t attr;

	// Setting the attribute of the thread to default.
	pthread_attr_init(&attr);

	// Creating 'num_threads' threads.
	for(int i=0;i<num_threads;i++)
	{
		if(pthread_create(&tid[i],&attr,run_thread,(void *)&max_exec_time)!=0)
		{
			printf("pthread_create() failed.\n");
			exit(EXIT_FAILURE);
		}
	}

	// Deallocating attr.
	pthread_attr_destroy(&attr);
	
	// Visualizing the status.
	// sleep(2);
	// display_pid_table();

	// The parent thread is waiting for all the child thread to finish execution.
	for(int i=0;i<num_threads;i++)
	{
		pthread_join(tid[i],NULL);
	}

	printf("All threads have executed successfully.\n");
}

int main()
{
	int num_threads = 100;
	int max_exec_time = 3;
	allocate_map();
	srand(time(0));
	
	create_threads(num_threads,max_exec_time);

	return 0;
}
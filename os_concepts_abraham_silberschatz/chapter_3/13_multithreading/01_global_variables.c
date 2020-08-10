/*
 * Compile as: $ gcc 01_global_variables.c -l pthread
 * Run as: $ ./a.out
 */

#include <pthread.h> // for pthread_exit,pthread_create,pthread_join,pthread_attr_t,pthread_attr_init
#include <unistd.h> // for fork, wait
#include <stdlib.h> // for exit
#include <stdio.h> // for printf

// global variable shared by all the threads.
// processes create seperate copies.
int value = 100;

void *fun(void *args)
{
	value += 100;
	pthread_exit(0);
}

int main(int argc, char const *argv[])
{
	pid_t pid;
	pthread_t tid;
	pthread_attr_t attr;

	pid = fork();

	if(pid<0) // Creation of child process failed
	{
		printf("fork() faile.\n");
		exit(EXIT_FAILURE);
	}
	else if(pid==0) // Child process
	{
		// defines the attributes of the thread, sets it to default
		pthread_attr_init(&attr);
		// creates the thread
		pthread_create(&tid,&attr,fun,NULL);
		// waits for the child thread to finish execution
		pthread_join(tid,NULL);
		// outputs the results
		printf("Child Process: value = %d\n",value);
	}
	else // Parent process
	{
		// waits for the completion of the child process
		wait(NULL);
		// outputs the results
		printf("Parent Process: value = %d\n",value);
	}
	
	return 0;
}
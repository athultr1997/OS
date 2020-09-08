#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

/*
 * This program creates a zombie process.
 * To visualize, enter the command - "./a.out & ps -l" in the terminal.
 * The process having the S column set to 'Z' is the zombie process.
 * If the parent process is taking too much time to execute, it can be 
 * terminted using the command - "kill -9 <pid of parent>".
 */

int main()
{
	pid_t pid = fork();
	
	if(pid<0) // If the fork() resulted in an error
	{
		printf("Error occured. Process not created.\n");
	}
	else if(pid==0) // Child process
	{
		printf("Child process completed.\n");
		exit(0); // The child process completed execution quickly
				 // but its return data is stored in the process table 
		         // since wait() was not called by its parent process
	}
	else // Parent process
	{
		sleep(10);
		printf("Parent process completed.\n"); // The parent process does not call wait()
	}

	return 0;
}
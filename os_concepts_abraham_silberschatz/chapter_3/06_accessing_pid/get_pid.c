#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid1 = fork();

	if(pid1<0) // If the fork() resulted in an error
	{
		printf("Error occured. Process not created.\n");
	}
	else if(pid1==0) // Child process
	{
		pid_t pid2 = getpid();
		pid_t pid3 = getppid(); // Returns the pid of the parent process
		printf("Inside Child process:\npid1 = %d\n",pid1);
		printf("pid2 = %d\n",pid2);
		printf("parent pid = %d\n",pid3);
	}
	else // Parent process
	{
		wait(NULL); // Parent process waits for the child process to get over
		pid_t pid2 = getpid();
		pid_t pid3 = getppid(); // Returns the pid of the parent process
		printf("Inside Parent process:\npid1 = %d\n",pid1);
		printf("pid2 = %d\n",pid2); 
		printf("parent pid = %d\n",pid3);
	}

	return 0;
}
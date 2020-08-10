#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid = fork();

	if(pid<0) // If the fork() resulted in an error
	{
		printf("Error occured. Process not created.\n");
	}
	else if(pid==0) // Child process
	{
		execlp("/bin/ls","ls",NULL);
		printf("Error occured with execlp().\n"); //control comes back to this line only if and only if the execlp() ends up in an error
	}
	else // Parent process
	{
		wait(NULL); // The parent process waits for the child process to complete
		printf("Child process completed.\n");
	}
	
	return 0;
}
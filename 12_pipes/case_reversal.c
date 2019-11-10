/*
 * Question 3.26 - Operating System Concepts, 9th Edition
 * ------------------------------------------------------
 * Design a program using ordinary pipes in which one process sends a
 * string message to a second process, and the second process reverses
 * the case of each character in the message and sends it back to the first
 * process. For example, if the first process sends the message Hi There, the
 * second process will return hI tHERE. This will require using two pipes,
 * one for sending the original message from the first to the second process
 * and the other for sending the modified message from the second to the
 * first process. You can write this program using either UNIX or Windows
 * pipes.
 */

/*
 * Compile as - 'gcc case_reversal.c'.
 * Run as - '.\a.out'
 */
#include <string.h> // for strlen
#include <unistd.h> //  for pipe, fork
#include <stdio.h> // for printf
#include <stdlib.h> // for exit

/*
 * Function: invert_case
 * ---------------------
 * args: char *array
 * returns: void
 * 
 * It converts lowercase letters to uppercase and vice-versa.
 * Special characters are ignored - '1','*',' ', etc.
 */
void invert_case(char *array)
{
	for(int i=0;array[i]!='\0';i++)
	{
		if(array[i]>='a' && array[i]<='z')
		{
			array[i] -= 32;
		}
		else if(array[i]>='A' && array[i]<='Z')
		{
			array[i] += 32;
		}
	}
}

int main()
{
	// ordinary pipe 1 file descriptors for passing message to the child process from the parent process
	int pipefd1[2];
	// ordinary pipe 2 file descriptors for passing message to the parent process from the child process
	int pipefd2[2]; 
	// message to be passed from parent process to child process
	char *message = "Hiiii      There";
	// storing the message size
	const int MSG_SIZE = strlen(message);

	// creating ordinary pipe 1
	if(pipe(pipefd1)==-1)
	{
		printf("pipe() failed.\n");
		exit(EXIT_FAILURE);
	}
	// storing the message in pipe-1
	write(pipefd1[1],message,MSG_SIZE);

	// creating ordinary pipe-2
	if(pipe(pipefd2)==-1)
	{
		printf("pipe() failed.\n");
		exit(EXIT_FAILURE);
	}

	// creating the chil process
	pid_t pid = fork();	

	if(pid<0) // fork() resulted in an error
	{
		printf("fork() failed.\n");
		exit(EXIT_FAILURE);
	}
	else if(pid==0) // Child Process
	{
		// array to store the message from the parent process
		char message_from_parent[MSG_SIZE];
		// accessing the message from the parent process
		if(read(pipefd1[0],message_from_parent,MSG_SIZE)<MSG_SIZE) // guard for read
		{
			printf("read() failed.\n");
			exit(EXIT_FAILURE);
		}
		// the char array has to be terminated with '\0' for case inversion, 
		// else the arrays end won't be found, resulting in an infinite loop.
		message_from_parent[MSG_SIZE] = '\0';
		
		// showing the message from parent process
		printf("Message from Parent: %s\n",message_from_parent);
		
		// inverting the case of the message
		invert_case(message_from_parent);
		// storing the case inverted message in pipe-2
		if(write(pipefd2[1],message_from_parent,MSG_SIZE)<MSG_SIZE) // guard for write
		{
			printf("write() failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	else // Parent Process
	{
		// waiting for the child process to finish execution
		wait();
		
		// array to store the message from the child process		
		char message_from_child[MSG_SIZE];		
		// accessing the message from the child process
		if(read(pipefd2[0],message_from_child,MSG_SIZE)<MSG_SIZE) // guard for read
		{
			printf("read() failed.\n");
			exit(EXIT_FAILURE);
		}
		// the char array has to be terminated with '\0',
		// else the char array's end won't be found.
		message_from_child[MSG_SIZE] = '\0';

		// showing the results...
		printf("Message from Child: %s\n",message_from_child);
	}

	return 0;
}
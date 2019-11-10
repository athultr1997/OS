/* Question - 3.21 in Operating System Concepts, 9th edition
 * ---------------------------------------------------------
 * The Collatz conjecture concerns what happens when we take any
 * positive integer n and apply the following algorithm:
 * n = n/2, if n is even
 *      3 × n + 1, if n is odd
 * The conjecture states that when this algorithm is continually applied,
 * all positive integers will eventually reach 1. For example, if n = 35, the
 * sequence is
 * 35, 106, 53, 160, 80, 40, 20, 10, 5, 16, 8, 4, 2, 1
 * Write a C program using the fork() system call that generates this
 * sequence in the child process. The starting number will be provided
 * from the command line. For example, if 8 is passed as a parameter on
 * the command line, the child process will output 8, 4, 2, 1. Because the
 * parent and child processes have their own copies of the data, it will be
 * necessary for the child to output the sequence. Have the parent invoke
 * the wait() call to wait for the child process to complete before exiting
 * the program. Perform necessary error checking to ensure that a positive
 * integer is passed on the command line.
 *
 * Question - 3.22 in Operating System Concepts, 9th edition
 * ---------------------------------------------------------
 * In Exercise 3.21, the child process must output the sequence of numbers
 * generated fromthe algorithm specified by the Collatz conjecture because
 * the parent and child have their own copies of the data. Another
 * approach to designing this program is to establish a shared-memory
 * object between the parent and child processes. This technique allows the
 * child to write the contents of the sequence to the shared-memory object.
 * The parent can then output the sequence when the child completes.
 * Because the memory is shared, any changes the child makes will be
 * reflected in the parent process as well.
*/

#include <stdio.h> // for sprintf and printf
#include <string.h> // for strlen
#include <fcntl.h> // for mmap,O_CREAT,O_RDWR,PROT_WRITE,MAP_SHARED
#include <sys/mman.h> // for O_CREAT,O_RDWR,PROT_WRITE,MAP_SHARED

/*
 * The child and the parent have their own copies of the data.
 * Hence, the child has to output the sequence, the parent cannot.
 */
void generate_collatz_seqeunce(int N)
{
	if(N<=0)
	{
		return;
	}

	pid_t pid = fork();

	if(pid<0) // If the fork() resulted in an error
	{
		printf("Error occured. Process not created.\n");
	}
	else if(pid==0) // Child process
	{
		printf("%d, ",N);
		while(N!=1)
		{
			if(N%2==0)
			{
				N = N/2;
			}
			else
			{
				N = 3*N + 1;
			}
			printf("%d, ",N);
		}
		printf("\n");
	}
	else // Parent process
	{
		wait(0);
		printf("Child process completed.\n");
	}

	return;
}

/*
 * Function to find the number of digits in a number.
 */
int find_num_of_dig(int N)
{
	int digits = 0;
	while(N!=0)
	{
		N = N/10;
		digits += 1;
	}

	return digits;
}

/*
 * The child creates the sequence and stores it in a shared memory object.
 * The parent uses the shared memory object to display the sequence.
 */
void generate_collatz_seqeunce_shared_memory(int N)
{
	if(N<=0)
	{
		return;
	}
	// name of the shared-memory-object
	const char *name = "shared_object";	
	// size in bytes of the memory object
	const int SIZE = 4096;
	// file descriptor for the shared-memory-object
	int shm_fd;
	// pointer to the memory mapped shared-memory-object
	void *ptr_shm;

	// creating a shared-memory-object
	shm_fd = shm_open(name,O_CREAT|O_RDWR, 0666);
	// specifying the size of the shared-memory-object
	ftruncate(shm_fd,SIZE);
	// memory mapping the shared-memory-object
	ptr_shm = mmap(0,SIZE,PROT_WRITE,MAP_SHARED,shm_fd,0);
	// creating the child process
	pid_t pid = fork();

	if(pid<0) // If the fork() resulted in an error
	{
		printf("Error occured. Process not created.\n");
	}
	else if(pid==0) // Child process
	{
		sprintf(ptr_shm,"%d, ",N);
		ptr_shm += find_num_of_dig(N)+2;
		while(N!=1)
		{
			if(N%2==0)
			{
				N = N/2;
			}
			else
			{
				N = 3*N + 1;
			}
			sprintf(ptr_shm,"%d, ",N);
			ptr_shm += find_num_of_dig(N)+2;
		}
		sprintf(ptr_shm,"\n");
	}
	else // Parent process
	{	
		wait(0);
		printf("Child process completed.\n");

		// opening the shared-memory-object created by the child process
		shm_fd = shm_open(name,O_RDONLY,0666);
		// memory mapping the shared-memory-object
		ptr_shm = mmap(0,SIZE,PROT_READ, MAP_SHARED,shm_fd,0);
		// outputing the data inside the shared-memory-object
		printf("%s",(char *)ptr_shm);

		// releasing the shared-memory-object
		shm_unlink(name);
	}

	return;
}

int main(int argc, char const *argv[])
{
	generate_collatz_seqeunce(atoi(argv[1]));
	generate_collatz_seqeunce_shared_memory(atoi(argv[1]));
	return 0;
}
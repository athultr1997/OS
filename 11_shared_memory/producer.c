/*
 * Compile as - 'gcc producer.c -o producer -lrt'.
 * Run before consumer.c
 */

#include <stdio.h> // for sprintf and printf
#include <string.h> // for strlen
#include <fcntl.h> // for mmap,O_CREAT,O_RDWR,PROT_WRITE,MAP_SHARED
#include <sys/mman.h> // for O_CREAT,O_RDWR,PROT_WRITE,MAP_SHARED

int main()
{
	// name of the shared-memory-object
	const char *name = "shared_object";	
	// strings to be written into the memory-object
	const char *str1 = "Hello World!";
	const char *str2 = "Hello World 2!";
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

	// storing the strings in the shared-memory-object
	sprintf(ptr_shm,str1);
	ptr_shm += strlen(str1);
	sprintf(ptr_shm,str2);
	ptr_shm += strlen(str2);

	return 0;
}
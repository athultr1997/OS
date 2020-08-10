/* 
 * Compile as - 'gcc consumer.c -o consumer -lrt'.
 * Run after running producer each time.
 */
#include <stdio.h> // for printf
#include <fcntl.h> // for mmap,O_RDONLY,MAP_SHARED,PROT_READ
#include <sys/mman.h> // for O_CREAT,O_RDWR,PROT_WRITE,MAP_SHARED

int main()
{	
	// name of the shared-memory-object
	// should be same as the one specified in producer.c
	const char *name = "shared_object";
    // size in bytes of the shared-memory-object
    const int SIZE  = 4096;
    // file descriptor for the shared-memory-object
    int shm_fd;
    // pointer to the memory mapped shared-memory-object
    void *shm_ptr;    

	// opening the shared-memory-object created by the producer process
	shm_fd = shm_open(name,O_RDONLY,0666);
	// memory mapping the shared-memory-object
	shm_ptr = mmap(0,SIZE,PROT_READ, MAP_SHARED,shm_fd,0);
	// outputing the data inside the shared-memory-object
	printf("%s\n",(char *)shm_ptr);

	// releasing the shared-memory-object
	shm_unlink(name);
	return 0;
}
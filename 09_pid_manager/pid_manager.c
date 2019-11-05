/* Question - 3.20 in Operating System Concepts, 9th edition
 * ---------------------------------------------------------
 * An operating system’s pid manager is responsible for managing process
 * identifiers. When a process is first created, it is assigned a unique pid
 * by the pid manager. The pid is returned to the pid manager when the
 * process completes execution, and the manager may later reassign this
 * pid. Process identifiers are discussed more fully in Section 3.3.1. What
 * is most important here is to recognize that process identifiers must be
 * unique; no two active processes can have the same pid.
 * Use the following constants to identify the range of possible pid values:
 * #define MIN PID 300
 * #define MAX PID 5000
 * You may use any data structure of your choice to represent the availability
 * of process identifiers. One strategy is to adopt what Linux has
 * done and use a bitmap in which a value of 0 at position i indicates that
 * a process id of value i is available and a value of 1 indicates that the
 * process id is currently in use.
 * Implement the following API for obtaining and releasing a pid:
 * int allocate map(void)—Creates and initializes a data structure
 * for representing pids; returns—1 if unsuccessful, 1 if successful
 * int allocate pid(void)—Allocates and returns a pid; returns -1 if unable to 
 * allocate a pid (all pids are in use)
 * void release pid(int pid)—Releases a pid
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main()
{
	allocate_map();
	
	for(int i=0;i<30;i++)
	{
		allocate_pid();
	}

	for(int i=MIN_PID+10;i<MIN_PID+25;i++)
	{
		release_pid(i);
	}

	display_pid_table();

	return 0;
}
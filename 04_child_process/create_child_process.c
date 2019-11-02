#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int value = 50;

int main()
{
  pid_t pid; /* Used to store the id of the child process */

  pid = fork(); /* A child process is created from the parent process */

  if(pid<0) /* If fork() did not create a child process, pid returned will be negative */
    {
      printf("Child was not creater. Error.\n");
      return 0;		       
    }  
  else if(pid==0) /* The child process has a pid of 0 in the child process */
    {
      value += 10;
      printf("Child created successfully\n");
      printf("value = %d\n",value);
      return 0;
    }
  else if(pid>0) /*In the parent process, the child process has a positive pid*/
    {
      wait(NULL);
      value += 20;
      printf("The parent process is running\n");
      printf("value = %d\n",value); /* value will be 70 and not 80 since the child process makes its own copy of data variables */
      return 0;
    }
}

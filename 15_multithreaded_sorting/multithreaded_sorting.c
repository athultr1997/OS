/*
 * Project: Multithreaded Sorting Application
 * ------------------------------------------
 * Write a multithreaded sorting program that works as follows: A list of integers
 * is divided into two smaller lists of equal size. Two separate threads (which we
 * will term sorting threads) sort each sublist using a sorting algorithm of your
 * choice. The two sublists are then merged by a third thread—a merging thread
 * —which merges the two sublists into a single sorted list.
 */

#include <pthread.h> // for pthread_create,pthread_join,pthread_attr_init,pthread_attr_destroy
#include <stdio.h> // for printf
#include <stdlib.h> // for free,malloc,realloc

// The data structure to store the elements to be sorted.
int *array;

// The data structure to store the elements after merging 
// the two sorted half arrays.
int *sorted_array;

// The variable to store the size of the data structure 
// before and after sorting.
int N;

// Structure for passing data to threads.
typedef struct
{
	// Range includes both start_index and end_index
	int start_index;
	int end_index;
} parameters;

/*
 * Function: sort_aux
 * Description: Used to sort elements of 'array' between the indices 
 *				'par->start_index' and 'par->end_index'.
 * Arguments: void *args
 * Returns: void *
 */
void *sort_aux(void *args)
{
	// args is converted back to parameters from void.
	parameters *par = args;

	// Bubble sort between start_index and end_index.
    int i,j; 
	for(i=par->start_index;i<par->end_index;i++)
	{
		for(j=par->start_index;j<par->end_index-i+par->start_index;j++)
		{
			if(array[j]>array[j+1])
			{
				int tmp = array[j];
				array[j] = array[j+1];
				array[j+1] = tmp;
			}	         
		}	      
	}
}

/*
 * Function: take_input
 * Description: Used to take input from the user. Dynamically sets 'array' and 'N'.
 				Dynamically allocates space for 'sorted_array'. 				
 * Arguments: void
 * Returns: void
 */
void take_input()
{
	char ch;
	char *str;
	int len,array_size,array_size_increment;

	// The maximum length of an element is assumed to be 10.
	str = (char *)malloc(sizeof(char)*10);
	// Counter for the length of an element.
	len = 0;
	// Initializing the length of 'array' to zero.	
	N = 0;
	// Initializing the length of 'array' to 16.
	array_size = 16;
	// Setting the increment to the size of 'array' to 16.
	array_size_increment = 16;
	array = realloc(NULL,sizeof(int)*array_size);

	while(1)
	{
		ch = getc(stdin);

        if(ch==' '||ch==EOF||ch=='\n')
        {
        	str[len++] = '\0';
        	array[N++] = atoi(str);
        	if(N==array_size)
        	{
        		array = realloc(array,sizeof(int)*(array_size+=array_size_increment));
        	}        	
        	free(str);
        	len = 0;
        	str = (char *)malloc(sizeof(char)*10);
        	
        	if(ch==' ')
        	{
        		continue;
        	}
        	else
        	{
        		break;
        	}
        }

        str[len++] = ch;
	}
	free(str);

	sorted_array = (int *)malloc(sizeof(int)*N);	
}

void sort()
{
	pthread_t tid[2];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	int mid_index = N/2 - 1;

	parameters *par1 = (parameters *)malloc(sizeof(parameters));	
	par1->start_index = 0;
	par1->end_index = mid_index;
	pthread_create(&tid[0],&attr,sort_aux,(void *)par1);

	parameters *par2 = (parameters *)malloc(sizeof(parameters));	
	par2->start_index = mid_index + 1;
	par2->end_index = N-1;
	pthread_create(&tid[1],&attr,sort_aux,(void *)par2);

	// Deallocating attr.
	pthread_attr_destroy(&attr);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	int i,j,k;

	for(i=0,j=mid_index+1,k=0;i<=mid_index && j<N;)
	{
		if(array[i]<array[j])
		{
			sorted_array[k++] = array[i++];
		}
		else
		{
			sorted_array[k++] = array[j++];	
		}
	}

	while(i<=mid_index)
	{
		sorted_array[k++] = array[i++];
	}

	while(j<N)
	{
		sorted_array[k++] = array[j++];	
	}

	free(par1);
	free(par2);
}

int main(int argc, char const *argv[])
{
	take_input();
	sort();

	for(int i=0;i<N;i++)
	{
		printf("%d ",sorted_array[i]);
	}	
	printf("\n");

	return 0;
}
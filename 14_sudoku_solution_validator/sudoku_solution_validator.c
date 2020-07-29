/* 
 * Project: Sudoku Solution Validator
 * ----------------------------------
 * A Sudoku puzzle uses a 9 × 9 grid in which each column and row, as well as
 * each of the nine 3 × 3 subgrids, must contain all of the digits 1 · · · 9. Figure
 * 4.19 presents an example of a valid Sudoku puzzle. This project consists of
 * designing a multithreaded application that determineswhether the solution to
 * a Sudoku puzzle is valid.
 */

/*
 * Compile as: >> gcc sudoku_solution_validator.c -l pthread
 * Run as: >> ./a.out
 * Sample correct input: (easy to copy-paste for testing)
	6 2 4 5 3 9 1 8 7
	5 1 9 7 2 8 6 3 4
	8 3 7 6 1 4 2 9 5
	1 4 3 8 6 5 7 2 9
	9 5 8 2 4 7 3 6 1 
	7 6 2 3 9 1 4 5 8
	3 7 1 9 5 6 8 4 2 
	4 9 6 1 8 2 5 7 3
	2 8 5 4 7 3 9 1 6
 */

#include <pthread.h> // for pthread_create,pthread_join,pthread_attr_init,pthread_attr_destroy
#include <stdio.h> // for printf,scanf
#include <stdlib.h> // for malloc

// The data structure to store the sudoku solution to validate.
// It is shared by all the threads, since it is a global variable.
int sudoku[9][9];

// The data structure to store the results from sudoku validation.
// The different rows are for: 0->row validation,1->column validation,2->subgrid validation.
// The values of the data structure are: 0->wrong,1->correct.
int sudoku_val_results[3][9];

// Structure for passing data to threads.
typedef struct
{
	int row;
	int column;
} parameters;

void *validate_aux(void *args)
{
	parameters *par = args;
	// sudoku[i][j]-1 -> present
	int present[9] = {0};
	int flag = 1;
	
	if(par->column==-1) // Validation for row.
	{
		for(int j=0;j<9;j++)
		{
			if(present[sudoku[par->row][j]-1] == 0)
			{
				present[sudoku[par->row][j]-1] = 1;
			}
			else
			{
				flag = 0;
				break;
			}			
		}

		sudoku_val_results[0][par->row] = (flag == 0)? 0 : 1;
	}
	else if(par->row==-1) // Validation for column.
	{
		for(int i=0;i<9;i++)
		{
			if(present[sudoku[i][par->column]-1] == 0)
			{
				present[sudoku[i][par->column]-1] = 1;
			}
			else
			{
				flag = 0;
				break;
			}
		}

		sudoku_val_results[1][par->column] = (flag == 0)? 0 : 1;
	}
	else if(par->row>=0 && par->row<9 && par->column>=0 && par->column<9) // Validation for subgrid.
	{
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				if(present[sudoku[par->row+i][par->column+j]-1] == 0)
				{
					present[sudoku[par->row+i][par->column+j]-1] = 1;	
				}
				else
				{
					flag = 0;
					break;
				}
			}

			if(flag==0)
			{
				break;
			}
		}

		sudoku_val_results[2][3*((par->row)/3)+((par->column)/3)] = (flag == 0)? 0 : 1;
	}
}

void *validate_cols(void *args)
{
	pthread_t tid[9];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	for(int i=0;i<9;i++)
	{
		parameters *par = (parameters *)malloc(sizeof(parameters));
		par->row = -1;
		par->column = i;
		pthread_create(&tid[i],&attr,validate_aux,(void *)par);
	}

	// Deallocating attr.
	pthread_attr_destroy(&attr);

	for(int i=0;i<9;i++)
	{
		pthread_join(tid[i],NULL);	
	}
}

void *validate_rows(void *args)
{
	pthread_t tid[9];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	for(int i=0;i<9;i++)
	{
		parameters *par = (parameters *)malloc(sizeof(parameters));
		par->row = i;
		par->column = -1;
		pthread_create(&tid[i],&attr,validate_aux,(void *)par);
	}

	// Deallocating attr.
	pthread_attr_destroy(&attr);

	for(int i=0;i<9;i++)
	{
		pthread_join(tid[i],NULL);	
	}
}

void *validate_subgrids(void *args)
{
	pthread_t tid[9];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	// Starting index of subgrid is passed.
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			parameters *par = (parameters *)malloc(sizeof(parameters));
			par->row = i*3;
			par->column = j*3;
			pthread_create(&tid[i],&attr,validate_aux,(void *)par);
		}		
	}

	// Deallocating attr.
	pthread_attr_destroy(&attr);

	for(int i=0;i<9;i++)
	{
		pthread_join(tid[i],NULL);	
	}
}

int validate(int sudoku[][9])
{
	pthread_t tid[3];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_create(&tid[0],&attr,validate_rows,NULL);
	pthread_create(&tid[1],&attr,validate_cols,NULL);
	pthread_create(&tid[2],&attr,validate_subgrids,NULL);

	// Deallocating attr.
	pthread_attr_destroy(&attr);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);

	int result = 1;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(sudoku_val_results[i][j]==0)
			{
				result = 0;
				break;
			}
		}

		if(result==0)
		{
			break;
		}
	}

	return result;
}

int main()
{
	printf(">> Please enter a sudoku solution for validation (9x9 matrix):\n"); 
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			scanf("%d",&sudoku[i][j]);
		}
	}

	if(validate(sudoku)==1)
	{
		printf(">> The sudoku solution is valid.\n");
	}
	else
	{
		printf(">> The sudoku solution is not valid.\n");
	}

	// for(int i=0;i<3;i++)
	// {
	// 	for(int j=0;j<9;j++)
	// 	{
	// 		printf("%d ",sudoku_val_results[i][j]);
	// 	}
	// 	printf("\n");
	// }

	return 0;
}
#include <stdio.h> /* for stdin and stdout */
#include <unistd.h>
#define MAX_LINE 80 /* The maximum length of command */

void create_args(char *buffer, char **args) {
     // use strsep

    int size = sizeof(buffer) / sizeof(buffer[0]);
    int args_row = 0;
    int args_col = 0;

    for (int i = 0; i < size; i = i + 1) {
        
    }

}

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
    char **args;
    char *buffer;
    while (should_run) {
        printf("osh>");
        fflush(stdout);
        read(stdin, buffer, MAX_LINE);

        create_args(buffer, args);

    }
    return 0;
}
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	char *str = (char *)malloc(8*sizeof(char));
	str[0] = 'b';
	str[1] = 'a';
	printf("%c\n",str[0]);
	free(str);
	printf("%c\n",str[0]);
	return 0;
}
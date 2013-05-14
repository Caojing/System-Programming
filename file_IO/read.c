#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	char buf[10];
	int n = 0;

	n = read(STDIN_FILENO, buf, 10);
//	printf("n = %d\n", n);

	if(n < 0)
	{
		perror("read STDIN_FILENO");
		exit(1);
	}

	write(STDOUT_FILENO, buf, n);

	return 0;
}

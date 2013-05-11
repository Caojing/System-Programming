#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int fd[2];
	char  str[10];
	pid_t pid;

	pipe(fd);
	pid = fork();

	if(pid == 0)
	{
		printf("this is child\n");
		close(fd[1]);
		read(fd[0], str, 5);
		printf("%s\n", str);
	}
	else
	{
		printf("this is parent\n");
		close(fd[0]);
		write(fd[1], "hello", 5);
		wait();
	}
	
	return 0;
}

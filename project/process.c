#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int fwin = 0;
int cwin = 0;

int get_rand(void)  // 0 is replace 剪刀   1 is replace 石头   2 is replace 布 
{
	return rand() % 3;
}

int cmp(int a, int b)
{
	if(a - b == 1 || a - b == -2)
		return 1;
	else if(a == b)
		return 0;
	else 
		return -1;
}

int judge(int father, int child)
{	
	printf("father: %d vs child: %d\n", father, child);
	
	if(cmp(father, child) > 0)
		fwin++;
	if(cmp(father, child) < 0)
		cwin++;

	printf("\t\t\t(%d vs %d)\n", fwin, cwin);
	
	if(fwin < 2 && cwin < 2)	
		return 0;
	if(fwin == 2)
		printf("father win\n");
	if(cwin == 2)
		printf("child win\n");

	return 1;
}

int main(void)
{
	pid_t pid;
	int fd[2];
	int n = 0;
	
	pipe(fd);
	pid = fork();

	if(pid == 0) //child
	{
		close(fd[1]);		
		srand(time(NULL));
		
		while(1)
		{
			read(fd[0], &n, 1);

			if(judge(n, get_rand()))
				break;
		}
	}
	else  //father
	{
		close(fd[0]);
		while(1)
		{
			n = get_rand();

			write(fd[1], &n, 1);
			sleep(1);
		}
	}
}

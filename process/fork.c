#include <stdio.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	int global = 200;

	pid = fork();
	if(pid == 0)
	{
		global = 50;
		printf("this is child\n");
		printf("%d(%p)\n", global, &global);
	}
	else
	{
		sleep(1);
		global = 100;
		printf("this is parent\n");
		printf("%d(%p)\n", global, &global);
		
	}

	return 0;
}

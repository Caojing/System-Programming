#include <stdio.h>
#include <signal.h>

void myhandler(int signo)
{
	printf("come in myhandler\n");
	return ;
}

int main(void)
{
	sigset_t s, p;
	struct sigaction newact;
	newact.sa_handler = myhandler;
	int counter = 0;

	sigaction(SIGINT, &newact, NULL);

	sigemptyset(&s);
	sigaddset(&s, SIGINT);
	sigprocmask(SIG_BLOCK, &s, NULL);

	while(1)
	{
		counter ++;
		
		if(counter > 5)
			sigprocmask(SIG_UNBLOCK, &s, NULL);

		printf("s = %x\n",*(int *)&s);
		sleep(1);
	}

	return 0;
}

#include <stdio.h>
#include <signal.h>

void myhandler(int signo)
{
	printf("this is myhandler\n");
	return;
}

int main(void)
{
	struct sigaction newact;

	newact.sa_handler = myhandler;
	sigaction(SIGINT, &newact, NULL);

	printf("begin pause \n");
	pause();
	printf("end pause\n");
	
	return 0;

}

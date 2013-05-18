#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int msg = 0;
int fwin = 0;
int cwin = 0;

int get_rand()
{
         return rand() % 3;
}

int cmp(int x, int y)
{
         if(x == y)
                 return 0;
         else if(x - y == 1 | x - y == -2)
                 return 1;
         return -1;
}

int judge(int father, int child)
{

         printf("father : %d vs child : %d\n", father, child);

         if(cmp(father, child) > 0)
                 fwin++;
         else if(cmp(father, child) < 0)
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


#define SIG_SET(x)	\
	void sig_set##x(int signo)	\
{\
	msg = x;	\
}

SIG_SET(0);
SIG_SET(1);
SIG_SET(2);

int signo[3] = {SIGQUIT, SIGUSR1, SIGUSR2};
void (*handler[3])(int signo) = {sig_set0, sig_set1, sig_set2};

void install_handler(int signo[], void (*handler[])(int signo))
{
	struct sigaction newact;
	int i = 0;

	for(i = 0; i < 3; i++)
	{
		newact.sa_handler = handler[i];
		sigemptyset(&newact.sa_mask);
		newact.sa_flags = 0;
		sigaction(signo[i], &newact, NULL);
	}
	
	return ;
}

void send(pid_t pid, int msg)
{
	kill(pid, signo[msg]);
}

void playing_with(pid_t opid, int id)
{
	while(1)
	{
		int tmp = 0;
		sigset_t newmask, oldmask, suspmask;

		tmp = get_rand();
		sigemptyset(&newmask);
		sigaddset(&newmask, signo[0]);
		sigaddset(&newmask, signo[1]);
		sigaddset(&newmask, signo[2]);
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);

		sleep(1);
		send(opid, tmp);

		sigdelset(&newmask, signo[0]);
		sigdelset(&newmask, signo[1]);
		sigdelset(&newmask, signo[2]);
		sigsuspend(&suspmask);
	
		sigprocmask(SIG_SETMASK, &oldmask, NULL);

#define SWAP(x, y)	{	x = x ^ y; y = y ^ x; x = x ^ y;	}

		if(id == 0)
			SWAP(tmp, msg);

		if(judge(tmp, msg))
		{
			wait();
			return ;
		}	
	}

	return ;
}

int main(void)
{
	pid_t pid;
	
	install_handler(signo, handler);	

	pid = fork();
	if(pid == 0)
	{
		int opid = getppid();
		playing_with(opid, 0);
	}
	else
	{
		srand(time(NULL));
		int opid = pid;
		playing_with(opid, 1);
	}

	return 0;
}

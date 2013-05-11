#include <stdio.h>
#include <pthread.h>

int global = 0;

void *mypthread(void *arg)
{
	while(1)
	{
		global++;
		printf("%s,%d\n", (char *)arg, global);
	}
	return ;
}

int main(void)
{
	pthread_t pthid1;
	pthread_t pthid2;
	void * tret;

	pthread_create(&pthid1, NULL, mypthread, "p1");

	pthread_create(&pthid2, NULL, mypthread, "p2");
	pthread_join(pthid1, &tret);
	pthread_join(pthid2, &tret);

	return 0;
}


#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int counter = 0;
//pthread_mutex_t mypth = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t *plock;

void *mypthread(void *arg)
{
	int i = 0;

	for(i = 0; i < 1000; i++)
	{
		int tmp;

		pthread_mutex_lock(plock);
		tmp = counter;
//		usleep(100);

		counter = tmp + 1;
		pthread_mutex_unlock(plock);
	
		printf("%s %d\n", (char *)arg, counter);
	}
//	printf("%s,%d,pthid = 0x%x\n", (char *)arg, getpid(), (unsigned int) pthread_self());
	
	return ;
}

int main(void)
{
	pthread_t mypid1;
	pthread_t mypid2;
	void * tret;
	pthread_mutex_t tmp;
	plock = &tmp;
//	plock = malloc(sizeof(pthread_mutex_t));
//	pthread_mutex_init(plock, NULL);

	printf("%d\n", getpid());
	pthread_create(&mypid1, NULL, mypthread, "p1");
	pthread_create(&mypid2, NULL, mypthread, "p2");
	pthread_join(mypid1, &tret);

	pthread_join(mypid2, &tret);

//	pthread_mutex_destroy(plock);

	return 0;

}

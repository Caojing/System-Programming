#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int msg = 0;
pthread_mutex_t pthid = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t *plock;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;

void *p(void *arg)
{

	int counter = 0;
	while(1)
	{
		counter++;

		pthread_mutex_lock(&pthid);
		msg = rand() % 100;
		printf("%s %d -> %d\n", (char *)arg, counter, msg);
		pthread_mutex_unlock(&pthid);

		pthread_cond_signal(&has_product);
		sleep(1);
	
	}
	
	return ;
}

void *c(void *arg)
{
	int counter = 0;

	while(1)
	{
		counter++;

		pthread_mutex_lock(&pthid);
		pthread_cond_wait(&has_product, &pthid);
		printf("%s %d -> %d\n", (char *)arg, counter, msg);
		pthread_mutex_unlock(&pthid);
	
	}
	
	return ;
}

int main(void)
{
	pthread_t mypid1;
	pthread_t mypid2;
	void * tret;
//	pthread_mutex_t tmp;
//	plock = &tmp;
//	plock = malloc(sizeof(pthread_mutex_t));
//	pthread_mutex_init(plock, NULL);

//	printf("%d\n", getpid());
	pthread_create(&mypid1, NULL, p, "product");
	pthread_create(&mypid2, NULL, c, "consume");
	pthread_join(mypid1, &tret);

	pthread_join(mypid2, &tret);

//	pthread_mutex_destroy(plock);

	return 0;

}

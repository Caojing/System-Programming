#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

int msg = 0;
pthread_mutex_t pthid = PTHREAD_MUTEX_INITIALIZER;
sem_t has_product;
sem_t is_empty;

void *p(void *arg)
{

	int counter = 0;
	while(1)
	{
		counter++;
	
		sem_wait(&is_empty);

		pthread_mutex_lock(&pthid);
		msg = rand() % 100;
		printf("%s %d -> %d\n", (char *)arg, counter, msg);
		pthread_mutex_unlock(&pthid);

		sem_post(&has_product);
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
		
		sem_wait(&has_product);

		pthread_mutex_lock(&pthid);
		printf("%s %d -> %d\n", (char *)arg, counter, msg);
		pthread_mutex_unlock(&pthid);

		sem_post(&is_empty);
	
	}
	
	return ;
}

int main(void)
{
	pthread_t mypid1;
	pthread_t mypid2;
	void * tret;

	sem_init(&has_product, 0, 0);
	sem_init(&is_empty, 0, 1);

	pthread_create(&mypid1, NULL, p, "product");
	pthread_create(&mypid2, NULL, c, "consume");

	pthread_join(mypid1, &tret);
	pthread_join(mypid2, &tret);

	return 0;

}

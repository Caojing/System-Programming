#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

int fwin = 0;
int cwin = 0; 

int get_rand(void)  // 0 is replace 剪刀   1 is replace 石头   2 is replace     布 
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
         fprintf(stderr, "user: %d vs computer: %d\n", father, child);
 
         if(cmp(father, child) > 0)
                 fwin++;

         if(cmp(father, child) < 0)
                 cwin++;
 
         fprintf(stderr, "\t\t\t(%d vs %d)\n", fwin, cwin);

         if(fwin < 2 && cwin < 2)
                return 0;

         if(fwin == 2)
                 fprintf(stderr, "user win\n");

         if(cwin == 2)
                 fprintf(stderr, "computer win\n");
 
         return 1;
}

pthread_mutex_t mylock = PTHREAD_MUTEX_INITIALIZER;
sem_t p1_has_product;
sem_t p2_has_product;
sem_t p1_is_empty;
sem_t p2_is_empty;

int p1_msg = 0;
int p2_msg = 0;

void *p1(void *arg)
{
	int counter = 0;
	
	while(1)
	{
		counter++;

		sem_wait(&p2_is_empty);

		pthread_mutex_lock(&mylock);
		printf("user: ");
		scanf("%d", &p1_msg);
//		p1_msg = get_rand();
//		printf("%s: counter %d -> %d\n", (char *)arg, counter, p1_msg);
		pthread_mutex_unlock(&mylock);

		sem_post(&p1_has_product);
	}

	return NULL;
}

void *p2(void *arg)
{
	int counter = 0;

	while(1)
	{
		counter++;

		sem_wait(&p1_is_empty);
	
		pthread_mutex_lock(&mylock);
//		scanf("%d", &p2_msg);
		p2_msg = get_rand();
//		printf("%s: counter %d -> %d\n", (char *)arg, counter, p2_msg);
		pthread_mutex_unlock(&mylock);

		sem_post(&p2_has_product);
	}

	return NULL;
}

void *c(void *arg)
{
	int counter = 0;

	while(1)
	{
		counter++;
		
		sem_wait(&p1_has_product);
		sem_wait(&p2_has_product);

		pthread_mutex_lock(&mylock);
//		printf("%s: %d -> %d vs %d\n", (char *)arg, counter, p1_msg, p2_msg);
		pthread_mutex_unlock(&mylock);

		if(judge(p1_msg, p2_msg))
			exit(0);

		sem_post(&p1_is_empty);
		sem_post(&p2_is_empty);
	}

	return NULL;
}

int main(void)
{
	
	srand(time(NULL));
	
	pthread_t pthid1;
	pthread_t pthid2;
	pthread_t pthid3;

	sem_init(&p1_has_product, 0, 0);
	sem_init(&p2_has_product, 0, 0);
	sem_init(&p1_is_empty, 0, 1);
	sem_init(&p2_is_empty, 0, 1);

	pthread_create(&pthid1, NULL, p1, "father");
	pthread_create(&pthid2, NULL, p2, "child");
	pthread_create(&pthid3, NULL, c, "c");

	pthread_join(pthid1, NULL);
	pthread_join(pthid2, NULL);
	pthread_join(pthid3, NULL);
		
	sem_destroy(&p1_is_empty);
	sem_destroy(&p2_is_empty);
	sem_destroy(&p1_has_product);
	sem_destroy(&p2_has_product);

	return 0;
}

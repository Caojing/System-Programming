###about thread
*任意一个线程调用exit或者_exit都会终止一个进程所有线程。
*如果只需要终止一个线程而不终止整个进程有如下三种方法：
	-从线程函数return。这种方法对主线程不适用。main函数return相当与调用exit。
	-一个线程可以调用pthread_cancel()（同步异步）终止同一进程另一进程。
	-线程可以调用pthread_exit()终止自己。
*用return和pthread_exit()返回的指针必须是指向内存单元是全局变量或者是malloc分配的。因为当线程返回时线程已经终止了。
###线程间同步
*两个线程都要吧全局变量增加1，需要在平台实现三个步骤：
	-从内存读变量到寄存器
	-把寄存器的值加1
	-把寄存器写回到内存
*原子操作
	-要么都执行，要么都不执行，不会在中途被打断。也不会在其他处理器上并行这个这个操作。
####mutex用pthread_mutex_t 类型变量表示，可以如下初始化和销毁：
*静态
	-pthread_mutex_t mypthread = PTHREAD_MUTEX_INITIALIZER;
*动态
	-pthread_mutex_t *plock;
	-plock = malloc(sizeof(pthread_mutex_t));
	-int pthread_mutex_init(plock, NULL);
	-int pthread_mutex_destroy(plock);


	-pthread_mutex_t *plock;
	-pthread_mutex_t tmp;
	-plock = &tmp;
	-pthread_mutex_t destroy(plock);

###为了防止线程间访问冲突，需要引入互斥锁：
	-pthread_mutex_lock(&mypthread);
	-pthread_mutex_unlock(&mypthread);
	_如果一个线程既想获得锁也不想挂起等待则需要pthread_mutex_trylock(&mypthread)
###挂起等待
	-首先把自己加入等待队列
	-把自己状态设置为睡眠
	-然后调用调度器切换到别的线程
###唤醒等待
	-从等待队列中任取一项
	-将他的状态改为就绪
	-加入就绪队列
###死锁
	-一个线程先后调用两次lock，在第二次调用的时候，由于所已经被占用，所以要挂起等待，但是锁正在被自己用，该线程又被挂起等待没有机会释放锁，因此永远就处于挂起等待了。
	-为了避免死锁，应该尽量使用pthread_mutex_trylock()代替pthread_mutex_lock();

###Condition variable
*静态
	-pthread_cond_t mypthread = PTHREAD_MUTEX_INITIALIZER;
*动态
	-pthread_cond_t *plock;
	-plock = malloc(sizeof(pthread_mutex_t));
	-int pthread_cond_init(plock, NULL);
	-int pthread_cond_destroy(plock);


	-pthread_cond_t *plock;
	-pthread_cond_t tmp;
	-plock = &tmp;
	-pthread_cond_t destroy(plock);

*pthread_cond_signal(&has_product)
*pthread_cond_wait(&has_product, &mylock);
###semaphore

	-sem_t has_product;
	-sem_t is_empty;
	-sem_wait(&has_product)
	-sem_post(&is_empty)
	-sem_init(&has_product, 0, 0);
	-sem_init(&is_empty, 0, 1);

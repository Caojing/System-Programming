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
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int fwin = 0;
int cwin = 0;

int get_rand(void)  // 0 is replace 剪刀   1 is replace 石头   2 is replace 布 
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
	printf("father: %d vs child: %d\n", father, child);
	
	if(cmp(father, child) > 0)
		fwin++;
	if(cmp(father, child) < 0)
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

int main(void)
{
	pid_t pid;
	int fd[2];
	int n = 0;
	
	pipe(fd);
	pid = fork();

	if(pid == 0) //child
	{
		close(fd[1]);		
		srand(time(NULL));
		
		while(1)
		{
			read(fd[0], &n, 1);

			if(judge(n, get_rand()))
				break;
		}
	}
	else  //father
	{
		close(fd[0]);
		while(1)
		{
			n = get_rand();

			write(fd[1], &n, 1);
			sleep(1);
		}
	}
}
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
#include <stdio.h>

int main(void)
{
	extern char ** environ;
	int i = 0;

	for(i = 0; environ[i]; i++)
	{
		printf("%s\n", environ[i]);
	}
	
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char ch = 0;
	int fd;

	fd = open(argv[1], O_RDONLY);
	dup2(fd, 0);
	close(fd);
	while((ch = getchar()) != EOF)
		printf("%c", toupper(ch));

	return 0;
}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("%s\n", getenv("PATH"));
	setenv("PATH", "myhello", 1);
	printf("%s\n", getenv("PATH"));


	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int fd;

	pid = fork();
	if(pid == 0)
	{
		
		printf("this is child\n");
		fd = open(argv[1], O_RDONLY | O_CREAT);
		dup2(fd, 0);
		close(fd);
		execl("./upper", "upper", NULL);
//		char *env[] = {"ps"};
//		execl("/bin/ls", "-a", NULL);
//		execlp("ls", "ls", "-al", NULL);
//		execv("/bin/ls", env);
	}
	else
	{
		sleep(1);
		printf("this is parent\n");
	}

	return 0;
}
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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int fd[2];
	char  str[10];
	pid_t pid;

	pipe(fd);
	pid = fork();

	if(pid == 0)
	{
		printf("this is child\n");
		close(fd[1]);
		read(fd[0], str, 5);
		printf("%s\n", str);
	}
	else
	{
		printf("this is parent\n");
		close(fd[0]);
		write(fd[1], "hello", 5);
		wait();
	}
	
	return 0;
}
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
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int counter = 0;
	
	alarm(1);
	for(counter = 0; ;)
	{
		printf("%d\n", ++counter);
	}

	return 0;
}
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
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define __le32  int 
#define __le16  short
#define __u8    char
#define __u16   short
#define __u32   int

struct ext2_super_block {
           __le32  s_inodes_count;         /* Inodes count */
           __le32  s_blocks_count;         /* Blocks count */
           __le32  s_r_blocks_count;       /* Reserved blocks count */
           __le32  s_free_blocks_count;    /* Free blocks count */
           __le32  s_free_inodes_count;    /* Free inodes count */
           __le32  s_first_data_block;     /* First Data Block */
           __le32  s_log_block_size;       /* Block size */
           __le32  s_log_frag_size;        /* Fragment size */
           __le32  s_blocks_per_group;     /* # Blocks per group */
           __le32  s_frags_per_group;      /* # Fragments per group */
           __le32  s_inodes_per_group;     /* # Inodes per group */
           __le32  s_mtime;                /* Mount time */
           __le32  s_wtime;                /* Write time */
           __le16  s_mnt_count;            /* Mount count */
           __le16  s_max_mnt_count;        /* Maximal mount count */
           __le16  s_magic;                /* Magic signature */
           __le16  s_state;                /* File system state */
           __le16  s_errors;               /* Behaviour when detecting errors */
            __le16  s_minor_rev_level;      /* minor revision level */
           __le32  s_lastcheck;            /* time of last check */
           __le32  s_checkinterval;        /* max. time between checks */
           __le32  s_creator_os;           /* OS */
           __le32  s_rev_level;            /* Revision level */
           __le16  s_def_resuid;           /* Default uid for reserved blocks */
           __le16  s_def_resgid;           /* Default gid for reserved blocks */
          /*
            * These fields are for EXT2_DYNAMIC_REV superblocks only.
            *
            * Note: the difference between the compatible feature set and
            * the incompatible feature set is that if there is a bit set
           * in the incompatible feature set that the kernel doesn't
	*/
       	   __le32  s_first_ino;            /* First non-reserved inode */
           __le16   s_inode_size;          /* size of inode structure */
           __le16  s_block_group_nr;       /* block group # of this superblock     */
           __le32  s_feature_compat;       /* compatible feature set */
           __le32  s_feature_incompat;     /* incompatible feature set */
           __le32  s_feature_ro_compat;    /* readonly-compatible feature set */
           __u8    s_uuid[16];             /* 128-bit uuid for volume */
           char    s_volume_name[16];      /* volume name */
           char    s_last_mounted[64];     /* directory where last mounted */
           __le32  s_algorithm_usage_bitmap; /* For compression */
          /*
           * Performance hints.  Directory preallocation should only
           * happen if the EXT2_COMPAT_PREALLOC flag is on.
           */
          __u8    s_prealloc_blocks;      /* Nr of blocks to try to preallocate*/
          __u8    s_prealloc_dir_blocks;  /* Nr to preallocate for dirs */
          __u16   s_padding1;
          /*
           * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
           */
          __u8    s_journal_uuid[16];     /* uuid of journal superblock */
          __u32   s_journal_inum;         /* inode number of journal file */
          __u32   s_journal_dev;          /* device number of journal file */
          __u32   s_last_orphan;          /* start of list of inodes to delete     */
          __u32   s_hash_seed[4];         /* HTREE hash seed */
          __u8    s_def_hash_version;     /* Default hash version to use */
          __u8    s_reserved_char_pad;
          __u16   s_reserved_word_pad;
          __le32  s_default_mount_opts;
          __le32  s_first_meta_bg;        /* First metablock block group */
        __u32   s_reserved[190];        /* Padding to the end of the block */
};

/*
   * Structure of a blocks group descriptor
*/
struct ext2_group_desc
{
          __le32  bg_block_bitmap;                /* Blocks bitmap block */
          __le32  bg_inode_bitmap;                /* Inodes bitmap block */
          __le32  bg_inode_table;         /* Inodes table block */
          __le16  bg_free_blocks_count;   /* Free blocks count */
          __le16  bg_free_inodes_count;   /* Free inodes count */
          __le16  bg_used_dirs_count;     /* Directories count */
          __le16  bg_pad;
          __le32  bg_reserved[3];
};
/*
* Constants relative to the data blocks
*/
#define EXT2_NDIR_BLOCKS                12
#define EXT2_IND_BLOCK                  EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK                 (EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK                 (EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS                   (EXT2_TIND_BLOCK + 1) 
 /*
   * Structure of an inode on the disk
 */
  struct ext2_inode {
          __le16  i_mode;         /* File mode */
          __le16  i_uid;          /* Low 16 bits of Owner Uid */
          __le32  i_size;         /* Size in bytes */
          __le32  i_atime;        /* Access time */
          __le32  i_ctime;        /* Creation time */
          __le32  i_mtime;        /* Modification time */
          __le32  i_dtime;        /* Deletion Time */
          __le16  i_gid;          /* Low 16 bits of Group Id */
          __le16  i_links_count;  /* Links count */
          __le32  i_blocks;       /* Blocks count */
          __le32  i_flags;        /* File flags */
          union {
                  struct {
                          __le32  l_i_reserved1;
                  } linux1;
                  struct {
                          __le32  h_i_translator;
                  } hurd1;
                  struct {
                         __le32  m_i_reserved1;
                  } masix1;
          } osd1;                         /* OS dependent 1 */
         __le32  i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
  };
 #define EXT2_NAME_LEN 255
 struct ext2_dir_entry_2 {
          __le32  inode;                  /* Inode number */
          __le16  rec_len;                /* Directory entry length */
          __u8    name_len;               /* Name length */
          __u8    file_type;
          char    name[EXT2_NAME_LEN];    /* File name */
 };

#if 0
#define PRINT(x)	printf(#x " = %d\n", x)
#define PRINTP(x)	printf(#x " = %p\n", x)
#define PRINTF(x)	printf(#x " = %s\n", x)
#else
#define PRINT(x)
#define PRINTP(x)
#define PRINTF(x)
#endif 

char *p = NULL;
int inode_size = 0;
int block_size = 0;
int group_block = 0;

void myputns(char *buf, int len)
{
	int i = 0;

	for(i = 0; i < len; i++)
		putchar(buf[i]);

}

struct ext2_super_block * get_super_block(void)
{
	return (struct ext2_super_block *)(p + 0x400);
}

void * get_block(int block)
{
	return (void *) (p + block * block_size);
}

struct ext2_inode * get_inode(int inode)
{
	int inode_table_block = 0;

	inode_table_block = ((struct ext2_group_desc *)get_block(group_block))->bg_inode_table;

	PRINT(inode_table_block);
	
	inode --;

	return (struct ext2_inode *)(get_block(inode_table_block) + inode * inode_size);
}

void ls(void)
{
	int len = 0;
	struct ext2_dir_entry_2 *pdir = NULL;
	
	pdir = (struct ext2_dir_entry_2 *)(get_block(get_inode(group_block)->i_block[0]));
	
	PRINTP(pdir);
	
	while(len < block_size)
	{
		myputns(pdir->name, pdir->name_len);
		putchar('\t');	

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2	*)((int)pdir + pdir->rec_len);	
	}
	
	putchar('\n');
	return ;
}

int namei(char *filename)
{
	struct ext2_dir_entry_2 *pdir = NULL;
	int len = 0;
	
	pdir = (struct ext2_dir_entry_2 *)(get_block(get_inode(group_block)->i_block[0]));

	while(len < block_size)
	{
		if(strlen(filename) == pdir->name_len)
		{
			if(strncmp(filename, pdir->name, pdir->name_len) == 0)
				return pdir->inode;
		}

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}
}

void cat(char *filename)
{
	int i = 0;
	struct ext2_inode *inode;
	
	i = namei(filename);
	
	if(i > 0)
	{
		inode = get_inode(i);
		myputns(get_block(inode->i_block[0]), inode->i_size);
	}
	else
		printf("%s is not found\n", filename);
	
	return ;
}

int main(int argc, char *argv[])
{
	int fd = 0;
	char *filename = "fs";

	if(argc >= 2)
		filename = argv[1];

	fd = open(filename, O_RDWR);
	if(fd < 0)
		perror("open file filename");
		
	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	PRINT(get_super_block()->s_inodes_count);	
	PRINT(get_super_block()->s_blocks_count);
	PRINT(get_super_block()->s_first_data_block);
	PRINT(get_super_block()->s_inode_size);
	PRINT(get_super_block()->s_log_block_size);

	inode_size = get_super_block()->s_inode_size;
	block_size = 1024 << get_super_block()->s_log_block_size;

	group_block = get_super_block()->s_first_data_block + 1;

	PRINT(inode_size);
	PRINT(block_size);
	PRINT(group_block);

	PRINT(((struct ext2_group_desc *)get_block(group_block))->bg_block_bitmap);
	PRINT(((struct ext2_group_desc *)get_block(group_block))->bg_inode_bitmap);
	PRINT(((struct ext2_group_desc *)get_block(group_block))->bg_inode_table);

	PRINT(get_inode(group_block)->i_block[0]);	
	
	while(1)
	{
		char buf[64] = {'\0'};
		char cmd[32] = {'\0'};
		char arg[32] = {'\0'};

		printf("MyExt2 #");

		fgets(buf, 64, stdin);
		sscanf(buf, "%s %s", cmd, arg);

		PRINTF(cmd);
		PRINTF(arg);

		if(strcmp(cmd, "ls") == 0)
			ls();
		if(strcmp(cmd, "cat") == 0)
			cat(arg);
		if(strcmp(cmd, "exit") == 0)
			break;
	}
	munmap(p, 1024*1024);
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#define __le32	int 
#define __le16	short
#define __u8	char
#define __u16	short
#define __u32	int

struct ext2_super_block {
          __le32  s_inodes_count;         /* Inodes count */
          __le32  s_blocks_count;         /* Blocks count */
          __le32  s_r_blocks_count;       /* Reserved blocks count */
          __le32  s_free_blocks_count;    /* Free blocks count */
          __le32  s_free_inodes_count;    /* Free inodes count */
          __le32  s_first_data_block;     /* First Data Block */
          __le32  s_log_block_size;       /* Block size */
          __le32  s_log_frag_size;        /* Fragment size */
          __le32  s_blocks_per_group;     /* # Blocks per group */
          __le32  s_frags_per_group;      /* # Fragments per group */
          __le32  s_inodes_per_group;     /* # Inodes per group */
          __le32  s_mtime;                /* Mount time */
          __le32  s_wtime;                /* Write time */
          __le16  s_mnt_count;            /* Mount count */
          __le16  s_max_mnt_count;        /* Maximal mount count */
          __le16  s_magic;                /* Magic signature */
          __le16  s_state;                /* File system state */
	  __le16  s_errors;               /* Behaviour when detecting errors */
          __le16  s_minor_rev_level;      /* minor revision level */
          __le32  s_lastcheck;            /* time of last check */
          __le32  s_checkinterval;        /* max. time between checks */
          __le32  s_creator_os;           /* OS */
          __le32  s_rev_level;            /* Revision level */
          __le16  s_def_resuid;           /* Default uid for reserved blocks */
          __le16  s_def_resgid;           /* Default gid for reserved blocks */
          /*
           * These fields are for EXT2_DYNAMIC_REV superblocks only.
           *
           * Note: the difference between the compatible feature set and
           * the incompatible feature set is that if there is a bit set
           * in the incompatible feature set that the kernel doesn't
		


          *know about, it should refuse to mount the filesystem.
           * e2fsck's requirements are more strict; if it doesn't know
           * about a feature in either the compatible or incompatible
           * feature set, it must abort and not try to meddle with
           * things it doesn't understand...
           */
          __le32  s_first_ino;            /* First non-reserved inode */
          __le16   s_inode_size;          /* size of inode structure */
          __le16  s_block_group_nr;       /* block group # of this superblock     */
          __le32  s_feature_compat;       /* compatible feature set */
          __le32  s_feature_incompat;     /* incompatible feature set */
          __le32  s_feature_ro_compat;    /* readonly-compatible feature set */
          __u8    s_uuid[16];             /* 128-bit uuid for volume */
          char    s_volume_name[16];      /* volume name */
          char    s_last_mounted[64];     /* directory where last mounted */
          __le32  s_algorithm_usage_bitmap; /* For compression */
          /*
           * Performance hints.  Directory preallocation should only
           * happen if the EXT2_COMPAT_PREALLOC flag is on.
           */
          __u8    s_prealloc_blocks;      /* Nr of blocks to try to preallocate*/
          __u8    s_prealloc_dir_blocks;  /* Nr to preallocate for dirs */
          __u16   s_padding1;
          /*
           * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
           */
          __u8    s_journal_uuid[16];     /* uuid of journal superblock */
          __u32   s_journal_inum;         /* inode number of journal file */
          __u32   s_journal_dev;          /* device number of journal file */
          __u32   s_last_orphan;          /* start of list of inodes to delete     */
          __u32   s_hash_seed[4];         /* HTREE hash seed */
          __u8    s_def_hash_version;     /* Default hash version to use */
          __u8    s_reserved_char_pad;
          __u16   s_reserved_word_pad;
          __le32  s_default_mount_opts;
	  __le32  s_first_meta_bg;        /* First metablock block group */
        __u32   s_reserved[190];        /* Padding to the end of the block */
  };
  
  /*
   * Structure of a blocks group descriptor
  */
  struct ext2_group_desc
  {
          __le32  bg_block_bitmap;                /* Blocks bitmap block */
          __le32  bg_inode_bitmap;                /* Inodes bitmap block */
          __le32  bg_inode_table;         /* Inodes table block */
          __le16  bg_free_blocks_count;   /* Free blocks count */
          __le16  bg_free_inodes_count;   /* Free inodes count */
          __le16  bg_used_dirs_count;     /* Directories count */
          __le16  bg_pad;
         __le32  bg_reserved[3];
 };
 /*
  * Constants relative to the data blocks
 */
 #define EXT2_NDIR_BLOCKS                12
 #define EXT2_IND_BLOCK                  EXT2_NDIR_BLOCKS
 #define EXT2_DIND_BLOCK                 (EXT2_IND_BLOCK + 1)
 #define EXT2_TIND_BLOCK                 (EXT2_DIND_BLOCK + 1)
 #define EXT2_N_BLOCKS                   (EXT2_TIND_BLOCK + 1) 
/*
  * Structure of an inode on the disk
*/
 struct ext2_inode {
         __le16  i_mode;         /* File mode */
         __le16  i_uid;          /* Low 16 bits of Owner Uid */
         __le32  i_size;         /* Size in bytes */
         __le32  i_atime;        /* Access time */
         __le32  i_ctime;        /* Creation time */
         __le32  i_mtime;        /* Modification time */
         __le32  i_dtime;        /* Deletion Time */
         __le16  i_gid;          /* Low 16 bits of Group Id */
         __le16  i_links_count;  /* Links count */
         __le32  i_blocks;       /* Blocks count */
         __le32  i_flags;        /* File flags */
         union {
                 struct {
                         __le32  l_i_reserved1;
                 } linux1;
                 struct {
                         __le32  h_i_translator;
                 } hurd1;
                 struct {
                        __le32  m_i_reserved1;
                 } masix1;
         } osd1;                         /* OS dependent 1 */
	__le32  i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
 }; 
#define EXT2_NAME_LEN 255
struct ext2_dir_entry_2 {
         __le32  inode;                  /* Inode number */
         __le16  rec_len;                /* Directory entry length */
         __u8    name_len;               /* Name length */
         __u8    file_type;
         char    name[EXT2_NAME_LEN];    /* File name */
};

#if 0
#define PRINT(x)	printf(#x " = %d\n", x)
#define PRINTP(x)	printf(#x " = %s\n", x)
#else
#define PRINT(x)
#define PRINTP(x)
#endif

int inode_count = 0;
int block_count = 0;
int group_block = 0;
int inode_size = 0;
int block_size = 0;
char *p = NULL;

void myputns(char *buf, int len)
{
	int i = 0;
	
	for(i = 0; i < len; i++)
		putchar(buf[i]);
}

struct ext2_super_block *get_super_block(void)
{
	return (struct ext2_super_block *)(p + 0x400);
}

void *get_block(int block)
{
	return (void *)(p + block * block_size);
}

struct ext2_inode *get_inode(int inode)
{
	int inode_table_block;
	
	inode_table_block = ((struct ext2_group_desc *)get_block(group_block))->bg_inode_table;
	
	inode--;

	return (struct ext2_inode *)(get_block(inode_table_block) + inode * inode_size);
}

void ls(void)
{
	int len = 0;
        struct ext2_dir_entry_2 *pdir;

	pdir = (struct ext2_dir_entry_2 *)get_block(get_inode(2)->i_block[0]);

	while(len < block_size)
	{
		PRINT(pdir->inode);
		PRINT(pdir->rec_len);
		PRINT(pdir->name_len);
		myputns(pdir->name, pdir->name_len);
		putchar('\t');

		len += pdir->rec_len;

		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}
	
	putchar('\n');

}

int namei(char *filename)
{
	struct ext2_dir_entry_2 *pdir;
	int len = 0;

	pdir = (struct ext2_dir_entry_2	*)get_block(get_inode(2)->i_block[0]);

	while(len < block_size)
	{
		PRINTP(filename);
		PRINTP(pdir->name);
		PRINTP(pdir->name_len);
		PRINT(strlen(filename));
		
		if(strlen(filename) == pdir->name_len)
		{
			if(strncmp(filename, pdir->name, pdir->name_len) == 0)
				return pdir->inode;
		}

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}

}

void cat(char *filename)
{
	struct ext2_inode *inode;
	int i = 0;

	i = namei(filename);

	if(i > 0)
	{
		inode = get_inode(i);
		myputns(get_block(inode->i_block[0]), inode->i_size);
	}
	else
		printf(" %s not found\n", filename);

	return ;
}

int main(int argc, char *argv[])
{
	char *filename = "fs";
	int fd = 0;
	int tmp;

	if(argc >= 2)
		filename = argv[1];
	
	fd = open(filename, O_RDWR);
	
	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	PRINT(get_super_block()->s_inodes_count);
	PRINT(get_super_block()->s_blocks_count);
	PRINT(get_super_block()->s_inode_size);
	PRINT(get_super_block()->s_first_data_block);
	PRINT(get_super_block()->s_log_block_size);

	inode_size = get_super_block()->s_inode_size;
	block_size = 1024 << (get_super_block()->s_log_block_size);

	PRINT(inode_size); 
	PRINT(block_size);

	group_block = get_super_block()->s_first_data_block + 1;
	PRINT(group_block);

	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_block_bitmap);
	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_inode_bitmap);
	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_inode_table);

	PRINT(get_inode(2)->i_block[0]);

#if 0
	printf("<<<<<<<<<<1.ls()>>>>>>>>>>>>>>>\n");
	printf("<<<<<<<<<<2.cat()>>>>>>>>>>>>>>\n");

	scanf("%d", &tmp);
	printf("myshell: tmp\n");
	switch(tmp){


		case 1:	
				ls();
//		case 2:
//				cat();
		default:
				break;
	}
#endif	
	while(1)
	{
		char buf[64];
		char cmd[32];
		char arg[32];
		int ret = 0;

		printf("myshell # ");

		fgets(buf, 64, stdin);
		ret = sscanf(buf, "%s %s", cmd, arg);

		if(ret == 0)
			continue;

		PRINTP(cmd);
		PRINTP(arg);
	
		if(strcmp(cmd, "ls") == 0)
			ls();
		if(strcmp(cmd, "cat") == 0)
			cat(arg);
		if(strcmp(cmd, "exit") == 0)
			break;
		
	}
	
	munmap(p, 1024*1024);
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

struct inode
{
	short st_mode;
	short user;
	int size;
	int time[4];
	short group;
	short links;
	int blockcount;
	int flags;
	int os;
	int block[15];
};

struct dir_file
{
	int inode;
	short record_len;
	char name_len;
	char file_type;
	char name;
};
typedef struct dir_file dir_file_t;

void myputns(char *buf, int len)
{
	int i = 0;
	for(i = 0; i < len; i++ )
		putchar(buf[i]);
	putchar('\n');
}

int main(int argc, char *argv[])
{
	char *filename = "fs";
	int fd;
	dir_file_t *pdir;
	char *p;
	int *inode;
	struct inode *in;
	int len = 0;

	if(argc >= 2)
		filename = argv[1];
	fd = open(filename, O_RDWR);

	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

#define INODE_SIZE		128
#define INODE_TABLE_ADDR	0x2000
#define inode(x)		INODE_TABLE_ADDR + INODE_SIZE *(x - 1)


#define BLOCK_SIZE		1024
#define block(x)		BLOCK_SIZE * x	

#define PRINT(x)		printf(#x " = %d\n", x)

	pdir = (dir_file_t *) (p + 0x6000);
	
	while(len < 1024)
	{
		printf("inode : 0x%x\n", pdir->inode);
		printf("record_len : 0x%x\n", pdir->record_len);
		printf("name_len : 0x%x\n", pdir->name_len);
		printf("file_type : %d\n", pdir->file_type);
		myputns(&pdir->name, pdir->name_len);

		if(pdir->file_type == 1)
		{
			
			in =(struct inode *) (p + inode(pdir->inode));
			PRINT(in->size);
			PRINT(in->block[0]);
			myputns((char *)(p + block(in->block[0])), in->size);
		}
		
		len += pdir->record_len;

		pdir = (dir_file_t *)((int)pdir + pdir->record_len);
		 
	}

	munmap(p, 10);
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define OFFSET	0x34
#define SIZE	0x22

void put_hex(char c)
{
	char *hex = "0123456789abcdef";

	putchar(hex[((c >> 4) & 0x0F)]);
	putchar(hex[((c >> 0) & 0x0F)]);
	putchar (' ');

	return ;
}

int main(int argc, char *argv[])
{
	int fd = 0;
	char *filename = "read";
	int counter = 0;
	char c = 0;
	int ret = 0;

	if(argc >= 2)
	{
		filename = argv[1];
	}

	fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		perror("open filename");
		exit(1);
	}

	lseek(fd, OFFSET, SEEK_SET);

	while(1)
	{
		ret = read(fd, &c, 1);	
		if(ret == 0)
			break;

		put_hex(c);
		printf("%p\n", &c);		
		counter++;
		
		if(counter % 16 == 0)
			putchar('\n');
		if(counter == SIZE)
			break;
	}

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	char buf[10];
	int n = 0;

	n = read(STDIN_FILENO, buf, 10);
//	printf("n = %d\n", n);

	if(n < 0)
	{
		perror("read STDIN_FILENO");
		exit(1);
	}

	write(STDOUT_FILENO, buf, n);

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int flags = 0;
	char c = 'c';
	int ret = 0;
	int fd = 0;
	
#if 0	
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);

	flags |= O_NONBLOCK;
	printf("flags = %x\n", flags); 
#endif
//	fd = open("a.txt", O_RDONLY | O_CREAT);

	fcntl(0, F_SETFL, O_RDONLY | O_NONBLOCK);
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);

	ret = read(0, &c, 1);
/*	if(ret < 0)
	{
		perror("read 0");
		exit(1);
	}
*/	
	putchar(c);
	putchar('A');

	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int flags = 0;
	int ret = 0;
	int counter = 0;
	char buf[10];
	
	fcntl(0, F_SETFL, O_RDWR | O_NONBLOCK);
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);
		
tryagain:
	if(counter >5)
		goto end;

	ret = read(0, buf, 10);
	counter ++;
	
	if(ret < 0)
	{
		goto tryagain;
	}

	write(0, buf, ret);
end:
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
	int fd = 0;
	int n = 0;
	char buf[10];
	int counter = 0;

	fd = open("./a", O_RDONLY | O_NONBLOCK);
	if(fd < 0)
	{
		perror("open a.txt");
		exit(1);
	}

tryagain:
		
	if(counter >= 5)
		goto end;

	sleep(1);	
	n = read(fd, buf, 10);

	counter ++;

	if(n < 0)
	{
		printf("errno = %d\n", errno);
		printf("n = %d\n", n);
		goto tryagain;
	}

	n = write(1, buf, n);
end:
	return 0;
}

#include <stdio.h>
#include <unistd.h>

#define PRINT(x)	printf(#x " = %d\n", (int)x)

int main(void)
{
	PRINT(STDIN_FILENO);
	PRINT(STDOUT_FILENO);
	PRINT(STDERR_FILENO);

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE	1024

int main(int argc, char *argv[])
{
	int fd = 0;
	char buf[BUFSIZE];	
	int ret = 0;
	
	if(argc >= 2)	
		fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0664 );	
	if(fd < 0)
	{
		perror("open argv[1]");
		exit(1);
	}

	while(1)
	{
		ret = read(0, buf, BUFSIZE);
		if(ret == 0)
			break;
			
		write(1, buf, ret);

		if(fd > 0)
			write(fd, buf, ret);
			
	}

	if(fd > 0)
		close(fd);

	return 0;
}


hello
ls
cd 
pwd
mkdir
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
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int fwin = 0;
int cwin = 0;

int get_rand(void)  // 0 is replace 剪刀   1 is replace 石头   2 is replace 布 
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
	printf("father: %d vs child: %d\n", father, child);
	
	if(cmp(father, child) > 0)
		fwin++;
	if(cmp(father, child) < 0)
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

int main(void)
{
	pid_t pid;
	int fd[2];
	int n = 0;
	
	pipe(fd);
	pid = fork();

	if(pid == 0) //child
	{
		close(fd[1]);		
		srand(time(NULL));
		
		while(1)
		{
			read(fd[0], &n, 1);

			if(judge(n, get_rand()))
				break;
		}
	}
	else  //father
	{
		close(fd[0]);
		while(1)
		{
			n = get_rand();

			write(fd[1], &n, 1);
			sleep(1);
		}
	}
}
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
#include <stdio.h>

int main(void)
{
	extern char ** environ;
	int i = 0;

	for(i = 0; environ[i]; i++)
	{
		printf("%s\n", environ[i]);
	}
	
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char ch = 0;
	int fd;

	fd = open(argv[1], O_RDONLY);
	dup2(fd, 0);
	close(fd);
	while((ch = getchar()) != EOF)
		printf("%c", toupper(ch));

	return 0;
}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("%s\n", getenv("PATH"));
	setenv("PATH", "myhello", 1);
	printf("%s\n", getenv("PATH"));


	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int fd;

	pid = fork();
	if(pid == 0)
	{
		
		printf("this is child\n");
		fd = open(argv[1], O_RDONLY | O_CREAT);
		dup2(fd, 0);
		close(fd);
		execl("./upper", "upper", NULL);
//		char *env[] = {"ps"};
//		execl("/bin/ls", "-a", NULL);
//		execlp("ls", "ls", "-al", NULL);
//		execv("/bin/ls", env);
	}
	else
	{
		sleep(1);
		printf("this is parent\n");
	}

	return 0;
}
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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int fd[2];
	char  str[10];
	pid_t pid;

	pipe(fd);
	pid = fork();

	if(pid == 0)
	{
		printf("this is child\n");
		close(fd[1]);
		read(fd[0], str, 5);
		printf("%s\n", str);
	}
	else
	{
		printf("this is parent\n");
		close(fd[0]);
		write(fd[1], "hello", 5);
		wait();
	}
	
	return 0;
}
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
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int counter = 0;
	
	alarm(1);
	for(counter = 0; ;)
	{
		printf("%d\n", ++counter);
	}

	return 0;
}
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
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define __le32  int 
#define __le16  short
#define __u8    char
#define __u16   short
#define __u32   int

struct ext2_super_block {
           __le32  s_inodes_count;         /* Inodes count */
           __le32  s_blocks_count;         /* Blocks count */
           __le32  s_r_blocks_count;       /* Reserved blocks count */
           __le32  s_free_blocks_count;    /* Free blocks count */
           __le32  s_free_inodes_count;    /* Free inodes count */
           __le32  s_first_data_block;     /* First Data Block */
           __le32  s_log_block_size;       /* Block size */
           __le32  s_log_frag_size;        /* Fragment size */
           __le32  s_blocks_per_group;     /* # Blocks per group */
           __le32  s_frags_per_group;      /* # Fragments per group */
           __le32  s_inodes_per_group;     /* # Inodes per group */
           __le32  s_mtime;                /* Mount time */
           __le32  s_wtime;                /* Write time */
           __le16  s_mnt_count;            /* Mount count */
           __le16  s_max_mnt_count;        /* Maximal mount count */
           __le16  s_magic;                /* Magic signature */
           __le16  s_state;                /* File system state */
           __le16  s_errors;               /* Behaviour when detecting errors */
            __le16  s_minor_rev_level;      /* minor revision level */
           __le32  s_lastcheck;            /* time of last check */
           __le32  s_checkinterval;        /* max. time between checks */
           __le32  s_creator_os;           /* OS */
           __le32  s_rev_level;            /* Revision level */
           __le16  s_def_resuid;           /* Default uid for reserved blocks */
           __le16  s_def_resgid;           /* Default gid for reserved blocks */
          /*
            * These fields are for EXT2_DYNAMIC_REV superblocks only.
            *
            * Note: the difference between the compatible feature set and
            * the incompatible feature set is that if there is a bit set
           * in the incompatible feature set that the kernel doesn't
	*/
       	   __le32  s_first_ino;            /* First non-reserved inode */
           __le16   s_inode_size;          /* size of inode structure */
           __le16  s_block_group_nr;       /* block group # of this superblock     */
           __le32  s_feature_compat;       /* compatible feature set */
           __le32  s_feature_incompat;     /* incompatible feature set */
           __le32  s_feature_ro_compat;    /* readonly-compatible feature set */
           __u8    s_uuid[16];             /* 128-bit uuid for volume */
           char    s_volume_name[16];      /* volume name */
           char    s_last_mounted[64];     /* directory where last mounted */
           __le32  s_algorithm_usage_bitmap; /* For compression */
          /*
           * Performance hints.  Directory preallocation should only
           * happen if the EXT2_COMPAT_PREALLOC flag is on.
           */
          __u8    s_prealloc_blocks;      /* Nr of blocks to try to preallocate*/
          __u8    s_prealloc_dir_blocks;  /* Nr to preallocate for dirs */
          __u16   s_padding1;
          /*
           * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
           */
          __u8    s_journal_uuid[16];     /* uuid of journal superblock */
          __u32   s_journal_inum;         /* inode number of journal file */
          __u32   s_journal_dev;          /* device number of journal file */
          __u32   s_last_orphan;          /* start of list of inodes to delete     */
          __u32   s_hash_seed[4];         /* HTREE hash seed */
          __u8    s_def_hash_version;     /* Default hash version to use */
          __u8    s_reserved_char_pad;
          __u16   s_reserved_word_pad;
          __le32  s_default_mount_opts;
          __le32  s_first_meta_bg;        /* First metablock block group */
        __u32   s_reserved[190];        /* Padding to the end of the block */
};

/*
   * Structure of a blocks group descriptor
*/
struct ext2_group_desc
{
          __le32  bg_block_bitmap;                /* Blocks bitmap block */
          __le32  bg_inode_bitmap;                /* Inodes bitmap block */
          __le32  bg_inode_table;         /* Inodes table block */
          __le16  bg_free_blocks_count;   /* Free blocks count */
          __le16  bg_free_inodes_count;   /* Free inodes count */
          __le16  bg_used_dirs_count;     /* Directories count */
          __le16  bg_pad;
          __le32  bg_reserved[3];
};
/*
* Constants relative to the data blocks
*/
#define EXT2_NDIR_BLOCKS                12
#define EXT2_IND_BLOCK                  EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK                 (EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK                 (EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS                   (EXT2_TIND_BLOCK + 1) 
 /*
   * Structure of an inode on the disk
 */
  struct ext2_inode {
          __le16  i_mode;         /* File mode */
          __le16  i_uid;          /* Low 16 bits of Owner Uid */
          __le32  i_size;         /* Size in bytes */
          __le32  i_atime;        /* Access time */
          __le32  i_ctime;        /* Creation time */
          __le32  i_mtime;        /* Modification time */
          __le32  i_dtime;        /* Deletion Time */
          __le16  i_gid;          /* Low 16 bits of Group Id */
          __le16  i_links_count;  /* Links count */
          __le32  i_blocks;       /* Blocks count */
          __le32  i_flags;        /* File flags */
          union {
                  struct {
                          __le32  l_i_reserved1;
                  } linux1;
                  struct {
                          __le32  h_i_translator;
                  } hurd1;
                  struct {
                         __le32  m_i_reserved1;
                  } masix1;
          } osd1;                         /* OS dependent 1 */
         __le32  i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
  };
 #define EXT2_NAME_LEN 255
 struct ext2_dir_entry_2 {
          __le32  inode;                  /* Inode number */
          __le16  rec_len;                /* Directory entry length */
          __u8    name_len;               /* Name length */
          __u8    file_type;
          char    name[EXT2_NAME_LEN];    /* File name */
 };

#if 0
#define PRINT(x)	printf(#x " = %d\n", x)
#define PRINTP(x)	printf(#x " = %p\n", x)
#define PRINTF(x)	printf(#x " = %s\n", x)
#else
#define PRINT(x)
#define PRINTP(x)
#define PRINTF(x)
#endif 

char *p = NULL;
int inode_size = 0;
int block_size = 0;
int group_block = 0;

void myputns(char *buf, int len)
{
	int i = 0;

	for(i = 0; i < len; i++)
		putchar(buf[i]);

}

struct ext2_super_block * get_super_block(void)
{
	return (struct ext2_super_block *)(p + 0x400);
}

void * get_block(int block)
{
	return (void *) (p + block * block_size);
}

struct ext2_inode * get_inode(int inode)
{
	int inode_table_block = 0;

	inode_table_block = ((struct ext2_group_desc *)get_block(group_block))->bg_inode_table;

	PRINT(inode_table_block);
	
	inode --;

	return (struct ext2_inode *)(get_block(inode_table_block) + inode * inode_size);
}

void ls(void)
{
	int len = 0;
	struct ext2_dir_entry_2 *pdir = NULL;
	
	pdir = (struct ext2_dir_entry_2 *)(get_block(get_inode(group_block)->i_block[0]));
	
	PRINTP(pdir);
	
	while(len < block_size)
	{
		myputns(pdir->name, pdir->name_len);
		putchar('\t');	

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2	*)((int)pdir + pdir->rec_len);	
	}
	
	putchar('\n');
	return ;
}

int namei(char *filename)
{
	struct ext2_dir_entry_2 *pdir = NULL;
	int len = 0;
	
	pdir = (struct ext2_dir_entry_2 *)(get_block(get_inode(group_block)->i_block[0]));

	while(len < block_size)
	{
		if(strlen(filename) == pdir->name_len)
		{
			if(strncmp(filename, pdir->name, pdir->name_len) == 0)
				return pdir->inode;
		}

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}
}

void cat(char *filename)
{
	int i = 0;
	struct ext2_inode *inode;
	
	i = namei(filename);
	
	if(i > 0)
	{
		inode = get_inode(i);
		myputns(get_block(inode->i_block[0]), inode->i_size);
	}
	else
		printf("%s is not found\n", filename);
	
	return ;
}

int main(int argc, char *argv[])
{
	int fd = 0;
	char *filename = "fs";

	if(argc >= 2)
		filename = argv[1];

	fd = open(filename, O_RDWR);
	if(fd < 0)
		perror("open file filename");
		
	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	PRINT(get_super_block()->s_inodes_count);	
	PRINT(get_super_block()->s_blocks_count);
	PRINT(get_super_block()->s_first_data_block);
	PRINT(get_super_block()->s_inode_size);
	PRINT(get_super_block()->s_log_block_size);

	inode_size = get_super_block()->s_inode_size;
	block_size = 1024 << get_super_block()->s_log_block_size;

	group_block = get_super_block()->s_first_data_block + 1;

	PRINT(inode_size);
	PRINT(block_size);
	PRINT(group_block);

	PRINT(((struct ext2_group_desc *)get_block(group_block))->bg_block_bitmap);
	PRINT(((struct ext2_group_desc *)get_block(group_block))->bg_inode_bitmap);
	PRINT(((struct ext2_group_desc *)get_block(group_block))->bg_inode_table);

	PRINT(get_inode(group_block)->i_block[0]);	
	
	while(1)
	{
		char buf[64] = {'\0'};
		char cmd[32] = {'\0'};
		char arg[32] = {'\0'};

		printf("MyExt2 #");

		fgets(buf, 64, stdin);
		sscanf(buf, "%s %s", cmd, arg);

		PRINTF(cmd);
		PRINTF(arg);

		if(strcmp(cmd, "ls") == 0)
			ls();
		if(strcmp(cmd, "cat") == 0)
			cat(arg);
		if(strcmp(cmd, "exit") == 0)
			break;
	}
	munmap(p, 1024*1024);
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#define __le32	int 
#define __le16	short
#define __u8	char
#define __u16	short
#define __u32	int

struct ext2_super_block {
          __le32  s_inodes_count;         /* Inodes count */
          __le32  s_blocks_count;         /* Blocks count */
          __le32  s_r_blocks_count;       /* Reserved blocks count */
          __le32  s_free_blocks_count;    /* Free blocks count */
          __le32  s_free_inodes_count;    /* Free inodes count */
          __le32  s_first_data_block;     /* First Data Block */
          __le32  s_log_block_size;       /* Block size */
          __le32  s_log_frag_size;        /* Fragment size */
          __le32  s_blocks_per_group;     /* # Blocks per group */
          __le32  s_frags_per_group;      /* # Fragments per group */
          __le32  s_inodes_per_group;     /* # Inodes per group */
          __le32  s_mtime;                /* Mount time */
          __le32  s_wtime;                /* Write time */
          __le16  s_mnt_count;            /* Mount count */
          __le16  s_max_mnt_count;        /* Maximal mount count */
          __le16  s_magic;                /* Magic signature */
          __le16  s_state;                /* File system state */
	  __le16  s_errors;               /* Behaviour when detecting errors */
          __le16  s_minor_rev_level;      /* minor revision level */
          __le32  s_lastcheck;            /* time of last check */
          __le32  s_checkinterval;        /* max. time between checks */
          __le32  s_creator_os;           /* OS */
          __le32  s_rev_level;            /* Revision level */
          __le16  s_def_resuid;           /* Default uid for reserved blocks */
          __le16  s_def_resgid;           /* Default gid for reserved blocks */
          /*
           * These fields are for EXT2_DYNAMIC_REV superblocks only.
           *
           * Note: the difference between the compatible feature set and
           * the incompatible feature set is that if there is a bit set
           * in the incompatible feature set that the kernel doesn't
		


          *know about, it should refuse to mount the filesystem.
           * e2fsck's requirements are more strict; if it doesn't know
           * about a feature in either the compatible or incompatible
           * feature set, it must abort and not try to meddle with
           * things it doesn't understand...
           */
          __le32  s_first_ino;            /* First non-reserved inode */
          __le16   s_inode_size;          /* size of inode structure */
          __le16  s_block_group_nr;       /* block group # of this superblock     */
          __le32  s_feature_compat;       /* compatible feature set */
          __le32  s_feature_incompat;     /* incompatible feature set */
          __le32  s_feature_ro_compat;    /* readonly-compatible feature set */
          __u8    s_uuid[16];             /* 128-bit uuid for volume */
          char    s_volume_name[16];      /* volume name */
          char    s_last_mounted[64];     /* directory where last mounted */
          __le32  s_algorithm_usage_bitmap; /* For compression */
          /*
           * Performance hints.  Directory preallocation should only
           * happen if the EXT2_COMPAT_PREALLOC flag is on.
           */
          __u8    s_prealloc_blocks;      /* Nr of blocks to try to preallocate*/
          __u8    s_prealloc_dir_blocks;  /* Nr to preallocate for dirs */
          __u16   s_padding1;
          /*
           * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
           */
          __u8    s_journal_uuid[16];     /* uuid of journal superblock */
          __u32   s_journal_inum;         /* inode number of journal file */
          __u32   s_journal_dev;          /* device number of journal file */
          __u32   s_last_orphan;          /* start of list of inodes to delete     */
          __u32   s_hash_seed[4];         /* HTREE hash seed */
          __u8    s_def_hash_version;     /* Default hash version to use */
          __u8    s_reserved_char_pad;
          __u16   s_reserved_word_pad;
          __le32  s_default_mount_opts;
	  __le32  s_first_meta_bg;        /* First metablock block group */
        __u32   s_reserved[190];        /* Padding to the end of the block */
  };
  
  /*
   * Structure of a blocks group descriptor
  */
  struct ext2_group_desc
  {
          __le32  bg_block_bitmap;                /* Blocks bitmap block */
          __le32  bg_inode_bitmap;                /* Inodes bitmap block */
          __le32  bg_inode_table;         /* Inodes table block */
          __le16  bg_free_blocks_count;   /* Free blocks count */
          __le16  bg_free_inodes_count;   /* Free inodes count */
          __le16  bg_used_dirs_count;     /* Directories count */
          __le16  bg_pad;
         __le32  bg_reserved[3];
 };
 /*
  * Constants relative to the data blocks
 */
 #define EXT2_NDIR_BLOCKS                12
 #define EXT2_IND_BLOCK                  EXT2_NDIR_BLOCKS
 #define EXT2_DIND_BLOCK                 (EXT2_IND_BLOCK + 1)
 #define EXT2_TIND_BLOCK                 (EXT2_DIND_BLOCK + 1)
 #define EXT2_N_BLOCKS                   (EXT2_TIND_BLOCK + 1) 
/*
  * Structure of an inode on the disk
*/
 struct ext2_inode {
         __le16  i_mode;         /* File mode */
         __le16  i_uid;          /* Low 16 bits of Owner Uid */
         __le32  i_size;         /* Size in bytes */
         __le32  i_atime;        /* Access time */
         __le32  i_ctime;        /* Creation time */
         __le32  i_mtime;        /* Modification time */
         __le32  i_dtime;        /* Deletion Time */
         __le16  i_gid;          /* Low 16 bits of Group Id */
         __le16  i_links_count;  /* Links count */
         __le32  i_blocks;       /* Blocks count */
         __le32  i_flags;        /* File flags */
         union {
                 struct {
                         __le32  l_i_reserved1;
                 } linux1;
                 struct {
                         __le32  h_i_translator;
                 } hurd1;
                 struct {
                        __le32  m_i_reserved1;
                 } masix1;
         } osd1;                         /* OS dependent 1 */
	__le32  i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
 }; 
#define EXT2_NAME_LEN 255
struct ext2_dir_entry_2 {
         __le32  inode;                  /* Inode number */
         __le16  rec_len;                /* Directory entry length */
         __u8    name_len;               /* Name length */
         __u8    file_type;
         char    name[EXT2_NAME_LEN];    /* File name */
};

#if 0
#define PRINT(x)	printf(#x " = %d\n", x)
#define PRINTP(x)	printf(#x " = %s\n", x)
#else
#define PRINT(x)
#define PRINTP(x)
#endif

int inode_count = 0;
int block_count = 0;
int group_block = 0;
int inode_size = 0;
int block_size = 0;
char *p = NULL;

void myputns(char *buf, int len)
{
	int i = 0;
	
	for(i = 0; i < len; i++)
		putchar(buf[i]);
}

struct ext2_super_block *get_super_block(void)
{
	return (struct ext2_super_block *)(p + 0x400);
}

void *get_block(int block)
{
	return (void *)(p + block * block_size);
}

struct ext2_inode *get_inode(int inode)
{
	int inode_table_block;
	
	inode_table_block = ((struct ext2_group_desc *)get_block(group_block))->bg_inode_table;
	
	inode--;

	return (struct ext2_inode *)(get_block(inode_table_block) + inode * inode_size);
}

void ls(void)
{
	int len = 0;
        struct ext2_dir_entry_2 *pdir;

	pdir = (struct ext2_dir_entry_2 *)get_block(get_inode(2)->i_block[0]);

	while(len < block_size)
	{
		PRINT(pdir->inode);
		PRINT(pdir->rec_len);
		PRINT(pdir->name_len);
		myputns(pdir->name, pdir->name_len);
		putchar('\t');

		len += pdir->rec_len;

		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}
	
	putchar('\n');

}

int namei(char *filename)
{
	struct ext2_dir_entry_2 *pdir;
	int len = 0;

	pdir = (struct ext2_dir_entry_2	*)get_block(get_inode(2)->i_block[0]);

	while(len < block_size)
	{
		PRINTP(filename);
		PRINTP(pdir->name);
		PRINTP(pdir->name_len);
		PRINT(strlen(filename));
		
		if(strlen(filename) == pdir->name_len)
		{
			if(strncmp(filename, pdir->name, pdir->name_len) == 0)
				return pdir->inode;
		}

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}

}

void cat(char *filename)
{
	struct ext2_inode *inode;
	int i = 0;

	i = namei(filename);

	if(i > 0)
	{
		inode = get_inode(i);
		myputns(get_block(inode->i_block[0]), inode->i_size);
	}
	else
		printf(" %s not found\n", filename);

	return ;
}

int main(int argc, char *argv[])
{
	char *filename = "fs";
	int fd = 0;
	int tmp;

	if(argc >= 2)
		filename = argv[1];
	
	fd = open(filename, O_RDWR);
	
	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	PRINT(get_super_block()->s_inodes_count);
	PRINT(get_super_block()->s_blocks_count);
	PRINT(get_super_block()->s_inode_size);
	PRINT(get_super_block()->s_first_data_block);
	PRINT(get_super_block()->s_log_block_size);

	inode_size = get_super_block()->s_inode_size;
	block_size = 1024 << (get_super_block()->s_log_block_size);

	PRINT(inode_size); 
	PRINT(block_size);

	group_block = get_super_block()->s_first_data_block + 1;
	PRINT(group_block);

	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_block_bitmap);
	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_inode_bitmap);
	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_inode_table);

	PRINT(get_inode(2)->i_block[0]);

#if 0
	printf("<<<<<<<<<<1.ls()>>>>>>>>>>>>>>>\n");
	printf("<<<<<<<<<<2.cat()>>>>>>>>>>>>>>\n");

	scanf("%d", &tmp);
	printf("myshell: tmp\n");
	switch(tmp){


		case 1:	
				ls();
//		case 2:
//				cat();
		default:
				break;
	}
#endif	
	while(1)
	{
		char buf[64];
		char cmd[32];
		char arg[32];
		int ret = 0;

		printf("myshell # ");

		fgets(buf, 64, stdin);
		ret = sscanf(buf, "%s %s", cmd, arg);

		if(ret == 0)
			continue;

		PRINTP(cmd);
		PRINTP(arg);
	
		if(strcmp(cmd, "ls") == 0)
			ls();
		if(strcmp(cmd, "cat") == 0)
			cat(arg);
		if(strcmp(cmd, "exit") == 0)
			break;
		
	}
	
	munmap(p, 1024*1024);
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

struct inode
{
	short st_mode;
	short user;
	int size;
	int time[4];
	short group;
	short links;
	int blockcount;
	int flags;
	int os;
	int block[15];
};

struct dir_file
{
	int inode;
	short record_len;
	char name_len;
	char file_type;
	char name;
};
typedef struct dir_file dir_file_t;

void myputns(char *buf, int len)
{
	int i = 0;
	for(i = 0; i < len; i++ )
		putchar(buf[i]);
	putchar('\n');
}

int main(int argc, char *argv[])
{
	char *filename = "fs";
	int fd;
	dir_file_t *pdir;
	char *p;
	int *inode;
	struct inode *in;
	int len = 0;

	if(argc >= 2)
		filename = argv[1];
	fd = open(filename, O_RDWR);

	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

#define INODE_SIZE		128
#define INODE_TABLE_ADDR	0x2000
#define inode(x)		INODE_TABLE_ADDR + INODE_SIZE *(x - 1)


#define BLOCK_SIZE		1024
#define block(x)		BLOCK_SIZE * x	

#define PRINT(x)		printf(#x " = %d\n", x)

	pdir = (dir_file_t *) (p + 0x6000);
	
	while(len < 1024)
	{
		printf("inode : 0x%x\n", pdir->inode);
		printf("record_len : 0x%x\n", pdir->record_len);
		printf("name_len : 0x%x\n", pdir->name_len);
		printf("file_type : %d\n", pdir->file_type);
		myputns(&pdir->name, pdir->name_len);

		if(pdir->file_type == 1)
		{
			
			in =(struct inode *) (p + inode(pdir->inode));
			PRINT(in->size);
			PRINT(in->block[0]);
			myputns((char *)(p + block(in->block[0])), in->size);
		}
		
		len += pdir->record_len;

		pdir = (dir_file_t *)((int)pdir + pdir->record_len);
		 
	}

	munmap(p, 10);
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define OFFSET	0x34
#define SIZE	0x22

void put_hex(char c)
{
	char *hex = "0123456789abcdef";

	putchar(hex[((c >> 4) & 0x0F)]);
	putchar(hex[((c >> 0) & 0x0F)]);
	putchar (' ');

	return ;
}

int main(int argc, char *argv[])
{
	int fd = 0;
	char *filename = "read";
	int counter = 0;
	char c = 0;
	int ret = 0;

	if(argc >= 2)
	{
		filename = argv[1];
	}

	fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		perror("open filename");
		exit(1);
	}

	lseek(fd, OFFSET, SEEK_SET);

	while(1)
	{
		ret = read(fd, &c, 1);	
		if(ret == 0)
			break;

		put_hex(c);
		printf("%p\n", &c);		
		counter++;
		
		if(counter % 16 == 0)
			putchar('\n');
		if(counter == SIZE)
			break;
	}

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	char buf[10];
	int n = 0;

	n = read(STDIN_FILENO, buf, 10);
//	printf("n = %d\n", n);

	if(n < 0)
	{
		perror("read STDIN_FILENO");
		exit(1);
	}

	write(STDOUT_FILENO, buf, n);

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int flags = 0;
	char c = 'c';
	int ret = 0;
	int fd = 0;
	
#if 0	
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);

	flags |= O_NONBLOCK;
	printf("flags = %x\n", flags); 
#endif
//	fd = open("a.txt", O_RDONLY | O_CREAT);

	fcntl(0, F_SETFL, O_RDONLY | O_NONBLOCK);
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);

	ret = read(0, &c, 1);
/*	if(ret < 0)
	{
		perror("read 0");
		exit(1);
	}
*/	
	putchar(c);
	putchar('A');

	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int flags = 0;
	int ret = 0;
	int counter = 0;
	char buf[10];
	
	fcntl(0, F_SETFL, O_RDWR | O_NONBLOCK);
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);
		
tryagain:
	if(counter >5)
		goto end;

	ret = read(0, buf, 10);
	counter ++;
	
	if(ret < 0)
	{
		goto tryagain;
	}

	write(0, buf, ret);
end:
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
	int fd = 0;
	int n = 0;
	char buf[10];
	int counter = 0;

	fd = open("./a", O_RDONLY | O_NONBLOCK);
	if(fd < 0)
	{
		perror("open a.txt");
		exit(1);
	}

tryagain:
		
	if(counter >= 5)
		goto end;

	sleep(1);	
	n = read(fd, buf, 10);

	counter ++;

	if(n < 0)
	{
		printf("errno = %d\n", errno);
		printf("n = %d\n", n);
		goto tryagain;
	}

	n = write(1, buf, n);
end:
	return 0;
}

#include <stdio.h>
#include <unistd.h>

#define PRINT(x)	printf(#x " = %d\n", (int)x)

int main(void)
{
	PRINT(STDIN_FILENO);
	PRINT(STDOUT_FILENO);
	PRINT(STDERR_FILENO);

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE	1024

int main(int argc, char *argv[])
{
	int fd = 0;
	char buf[BUFSIZE];	
	int ret = 0;
	
	if(argc >= 2)	
		fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0664 );	
	if(fd < 0)
	{
		perror("open argv[1]");
		exit(1);
	}

	while(1)
	{
		ret = read(0, buf, BUFSIZE);
		if(ret == 0)
			break;
			
		write(1, buf, ret);

		if(fd > 0)
			write(fd, buf, ret);
			
	}

	if(fd > 0)
		close(fd);

	return 0;
}


hello
ls
cd 
pwd
mkdir
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
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int fwin = 0;
int cwin = 0;

int get_rand(void)  // 0 is replace 剪刀   1 is replace 石头   2 is replace 布 
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
	printf("father: %d vs child: %d\n", father, child);
	
	if(cmp(father, child) > 0)
		fwin++;
	if(cmp(father, child) < 0)
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

int main(void)
{
	pid_t pid;
	int fd[2];
	int n = 0;
	
	pipe(fd);
	pid = fork();

	if(pid == 0) //child
	{
		close(fd[1]);		
		srand(time(NULL));
		
		while(1)
		{
			read(fd[0], &n, 1);

			if(judge(n, get_rand()))
				break;
		}
	}
	else  //father
	{
		close(fd[0]);
		while(1)
		{
			n = get_rand();

			write(fd[1], &n, 1);
			sleep(1);
		}
	}
}
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
#include <stdio.h>

int main(void)
{
	extern char ** environ;
	int i = 0;

	for(i = 0; environ[i]; i++)
	{
		printf("%s\n", environ[i]);
	}
	
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char ch = 0;
	int fd;

	fd = open(argv[1], O_RDONLY);
	dup2(fd, 0);
	close(fd);
	while((ch = getchar()) != EOF)
		printf("%c", toupper(ch));

	return 0;
}
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("%s\n", getenv("PATH"));
	setenv("PATH", "myhello", 1);
	printf("%s\n", getenv("PATH"));


	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int fd;

	pid = fork();
	if(pid == 0)
	{
		
		printf("this is child\n");
		fd = open(argv[1], O_RDONLY | O_CREAT);
		dup2(fd, 0);
		close(fd);
		execl("./upper", "upper", NULL);
//		char *env[] = {"ps"};
//		execl("/bin/ls", "-a", NULL);
//		execlp("ls", "ls", "-al", NULL);
//		execv("/bin/ls", env);
	}
	else
	{
		sleep(1);
		printf("this is parent\n");
	}

	return 0;
}
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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int fd[2];
	char  str[10];
	pid_t pid;

	pipe(fd);
	pid = fork();

	if(pid == 0)
	{
		printf("this is child\n");
		close(fd[1]);
		read(fd[0], str, 5);
		printf("%s\n", str);
	}
	else
	{
		printf("this is parent\n");
		close(fd[0]);
		write(fd[1], "hello", 5);
		wait();
	}
	
	return 0;
}
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
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int counter = 0;
	
	alarm(1);
	for(counter = 0; ;)
	{
		printf("%d\n", ++counter);
	}

	return 0;
}
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
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define __le32  int 
#define __le16  short
#define __u8    char
#define __u16   short
#define __u32   int

struct ext2_super_block {
           __le32  s_inodes_count;         /* Inodes count */
           __le32  s_blocks_count;         /* Blocks count */
           __le32  s_r_blocks_count;       /* Reserved blocks count */
           __le32  s_free_blocks_count;    /* Free blocks count */
           __le32  s_free_inodes_count;    /* Free inodes count */
           __le32  s_first_data_block;     /* First Data Block */
           __le32  s_log_block_size;       /* Block size */
           __le32  s_log_frag_size;        /* Fragment size */
           __le32  s_blocks_per_group;     /* # Blocks per group */
           __le32  s_frags_per_group;      /* # Fragments per group */
           __le32  s_inodes_per_group;     /* # Inodes per group */
           __le32  s_mtime;                /* Mount time */
           __le32  s_wtime;                /* Write time */
           __le16  s_mnt_count;            /* Mount count */
           __le16  s_max_mnt_count;        /* Maximal mount count */
           __le16  s_magic;                /* Magic signature */
           __le16  s_state;                /* File system state */
           __le16  s_errors;               /* Behaviour when detecting errors */
            __le16  s_minor_rev_level;      /* minor revision level */
           __le32  s_lastcheck;            /* time of last check */
           __le32  s_checkinterval;        /* max. time between checks */
           __le32  s_creator_os;           /* OS */
           __le32  s_rev_level;            /* Revision level */
           __le16  s_def_resuid;           /* Default uid for reserved blocks */
           __le16  s_def_resgid;           /* Default gid for reserved blocks */
          /*
            * These fields are for EXT2_DYNAMIC_REV superblocks only.
            *
            * Note: the difference between the compatible feature set and
            * the incompatible feature set is that if there is a bit set
           * in the incompatible feature set that the kernel doesn't
	*/
       	   __le32  s_first_ino;            /* First non-reserved inode */
           __le16   s_inode_size;          /* size of inode structure */
           __le16  s_block_group_nr;       /* block group # of this superblock     */
           __le32  s_feature_compat;       /* compatible feature set */
           __le32  s_feature_incompat;     /* incompatible feature set */
           __le32  s_feature_ro_compat;    /* readonly-compatible feature set */
           __u8    s_uuid[16];             /* 128-bit uuid for volume */
           char    s_volume_name[16];      /* volume name */
           char    s_last_mounted[64];     /* directory where last mounted */
           __le32  s_algorithm_usage_bitmap; /* For compression */
          /*
           * Performance hints.  Directory preallocation should only
           * happen if the EXT2_COMPAT_PREALLOC flag is on.
           */
          __u8    s_prealloc_blocks;      /* Nr of blocks to try to preallocate*/
          __u8    s_prealloc_dir_blocks;  /* Nr to preallocate for dirs */
          __u16   s_padding1;
          /*
           * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
           */
          __u8    s_journal_uuid[16];     /* uuid of journal superblock */
          __u32   s_journal_inum;         /* inode number of journal file */
          __u32   s_journal_dev;          /* device number of journal file */
          __u32   s_last_orphan;          /* start of list of inodes to delete     */
          __u32   s_hash_seed[4];         /* HTREE hash seed */
          __u8    s_def_hash_version;     /* Default hash version to use */
          __u8    s_reserved_char_pad;
          __u16   s_reserved_word_pad;
          __le32  s_default_mount_opts;
          __le32  s_first_meta_bg;        /* First metablock block group */
        __u32   s_reserved[190];        /* Padding to the end of the block */
};

/*
   * Structure of a blocks group descriptor
*/
struct ext2_group_desc
{
          __le32  bg_block_bitmap;                /* Blocks bitmap block */
          __le32  bg_inode_bitmap;                /* Inodes bitmap block */
          __le32  bg_inode_table;         /* Inodes table block */
          __le16  bg_free_blocks_count;   /* Free blocks count */
          __le16  bg_free_inodes_count;   /* Free inodes count */
          __le16  bg_used_dirs_count;     /* Directories count */
          __le16  bg_pad;
          __le32  bg_reserved[3];
};
/*
* Constants relative to the data blocks
*/
#define EXT2_NDIR_BLOCKS                12
#define EXT2_IND_BLOCK                  EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK                 (EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK                 (EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS                   (EXT2_TIND_BLOCK + 1) 
 /*
   * Structure of an inode on the disk
 */
  struct ext2_inode {
          __le16  i_mode;         /* File mode */
          __le16  i_uid;          /* Low 16 bits of Owner Uid */
          __le32  i_size;         /* Size in bytes */
          __le32  i_atime;        /* Access time */
          __le32  i_ctime;        /* Creation time */
          __le32  i_mtime;        /* Modification time */
          __le32  i_dtime;        /* Deletion Time */
          __le16  i_gid;          /* Low 16 bits of Group Id */
          __le16  i_links_count;  /* Links count */
          __le32  i_blocks;       /* Blocks count */
          __le32  i_flags;        /* File flags */
          union {
                  struct {
                          __le32  l_i_reserved1;
                  } linux1;
                  struct {
                          __le32  h_i_translator;
                  } hurd1;
                  struct {
                         __le32  m_i_reserved1;
                  } masix1;
          } osd1;                         /* OS dependent 1 */
         __le32  i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
  };
 #define EXT2_NAME_LEN 255
 struct ext2_dir_entry_2 {
          __le32  inode;                  /* Inode number */
          __le16  rec_len;                /* Directory entry length */
          __u8    name_len;               /* Name length */
          __u8    file_type;
          char    name[EXT2_NAME_LEN];    /* File name */
 };

#if 0
#define PRINT(x)	printf(#x " = %d\n", x)
#define PRINTP(x)	printf(#x " = %p\n", x)
#define PRINTF(x)	printf(#x " = %s\n", x)
#else
#define PRINT(x)
#define PRINTP(x)
#define PRINTF(x)
#endif 

char *p = NULL;
int inode_size = 0;
int block_size = 0;
int group_block = 0;

void myputns(char *buf, int len)
{
	int i = 0;

	for(i = 0; i < len; i++)
		putchar(buf[i]);

}

struct ext2_super_block * get_super_block(void)
{
	return (struct ext2_super_block *)(p + 0x400);
}

void * get_block(int block)
{
	return (void *) (p + block * block_size);
}

struct ext2_inode * get_inode(int inode)
{
	int inode_table_block = 0;

	inode_table_block = ((struct ext2_group_desc *)get_block(group_block))->bg_inode_table;

	PRINT(inode_table_block);
	
	inode --;

	return (struct ext2_inode *)(get_block(inode_table_block) + inode * inode_size);
}

void ls(void)
{
	int len = 0;
	struct ext2_dir_entry_2 *pdir = NULL;
	
	pdir = (struct ext2_dir_entry_2 *)(get_block(get_inode(group_block)->i_block[0]));
	
	PRINTP(pdir);
	
	while(len < block_size)
	{
		myputns(pdir->name, pdir->name_len);
		putchar('\t');	

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2	*)((int)pdir + pdir->rec_len);	
	}
	
	putchar('\n');
	return ;
}

int namei(char *filename)
{
	struct ext2_dir_entry_2 *pdir = NULL;
	int len = 0;
	
	pdir = (struct ext2_dir_entry_2 *)(get_block(get_inode(group_block)->i_block[0]));

	while(len < block_size)
	{
		if(strlen(filename) == pdir->name_len)
		{
			if(strncmp(filename, pdir->name, pdir->name_len) == 0)
				return pdir->inode;
		}

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}
}

void cat(char *filename)
{
	int i = 0;
	struct ext2_inode *inode;
	
	i = namei(filename);
	
	if(i > 0)
	{
		inode = get_inode(i);
		myputns(get_block(inode->i_block[0]), inode->i_size);
	}
	else
		printf("%s is not found\n", filename);
	
	return ;
}

int main(int argc, char *argv[])
{
	int fd = 0;
	char *filename = "fs";

	if(argc >= 2)
		filename = argv[1];

	fd = open(filename, O_RDWR);
	if(fd < 0)
		perror("open file filename");
		
	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	PRINT(get_super_block()->s_inodes_count);	
	PRINT(get_super_block()->s_blocks_count);
	PRINT(get_super_block()->s_first_data_block);
	PRINT(get_super_block()->s_inode_size);
	PRINT(get_super_block()->s_log_block_size);

	inode_size = get_super_block()->s_inode_size;
	block_size = 1024 << get_super_block()->s_log_block_size;

	group_block = get_super_block()->s_first_data_block + 1;

	PRINT(inode_size);
	PRINT(block_size);
	PRINT(group_block);

	PRINT(((struct ext2_group_desc *)get_block(group_block))->bg_block_bitmap);
	PRINT(((struct ext2_group_desc *)get_block(group_block))->bg_inode_bitmap);
	PRINT(((struct ext2_group_desc *)get_block(group_block))->bg_inode_table);

	PRINT(get_inode(group_block)->i_block[0]);	
	
	while(1)
	{
		char buf[64] = {'\0'};
		char cmd[32] = {'\0'};
		char arg[32] = {'\0'};

		printf("MyExt2 #");

		fgets(buf, 64, stdin);
		sscanf(buf, "%s %s", cmd, arg);

		PRINTF(cmd);
		PRINTF(arg);

		if(strcmp(cmd, "ls") == 0)
			ls();
		if(strcmp(cmd, "cat") == 0)
			cat(arg);
		if(strcmp(cmd, "exit") == 0)
			break;
	}
	munmap(p, 1024*1024);
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#define __le32	int 
#define __le16	short
#define __u8	char
#define __u16	short
#define __u32	int

struct ext2_super_block {
          __le32  s_inodes_count;         /* Inodes count */
          __le32  s_blocks_count;         /* Blocks count */
          __le32  s_r_blocks_count;       /* Reserved blocks count */
          __le32  s_free_blocks_count;    /* Free blocks count */
          __le32  s_free_inodes_count;    /* Free inodes count */
          __le32  s_first_data_block;     /* First Data Block */
          __le32  s_log_block_size;       /* Block size */
          __le32  s_log_frag_size;        /* Fragment size */
          __le32  s_blocks_per_group;     /* # Blocks per group */
          __le32  s_frags_per_group;      /* # Fragments per group */
          __le32  s_inodes_per_group;     /* # Inodes per group */
          __le32  s_mtime;                /* Mount time */
          __le32  s_wtime;                /* Write time */
          __le16  s_mnt_count;            /* Mount count */
          __le16  s_max_mnt_count;        /* Maximal mount count */
          __le16  s_magic;                /* Magic signature */
          __le16  s_state;                /* File system state */
	  __le16  s_errors;               /* Behaviour when detecting errors */
          __le16  s_minor_rev_level;      /* minor revision level */
          __le32  s_lastcheck;            /* time of last check */
          __le32  s_checkinterval;        /* max. time between checks */
          __le32  s_creator_os;           /* OS */
          __le32  s_rev_level;            /* Revision level */
          __le16  s_def_resuid;           /* Default uid for reserved blocks */
          __le16  s_def_resgid;           /* Default gid for reserved blocks */
          /*
           * These fields are for EXT2_DYNAMIC_REV superblocks only.
           *
           * Note: the difference between the compatible feature set and
           * the incompatible feature set is that if there is a bit set
           * in the incompatible feature set that the kernel doesn't
		


          *know about, it should refuse to mount the filesystem.
           * e2fsck's requirements are more strict; if it doesn't know
           * about a feature in either the compatible or incompatible
           * feature set, it must abort and not try to meddle with
           * things it doesn't understand...
           */
          __le32  s_first_ino;            /* First non-reserved inode */
          __le16   s_inode_size;          /* size of inode structure */
          __le16  s_block_group_nr;       /* block group # of this superblock     */
          __le32  s_feature_compat;       /* compatible feature set */
          __le32  s_feature_incompat;     /* incompatible feature set */
          __le32  s_feature_ro_compat;    /* readonly-compatible feature set */
          __u8    s_uuid[16];             /* 128-bit uuid for volume */
          char    s_volume_name[16];      /* volume name */
          char    s_last_mounted[64];     /* directory where last mounted */
          __le32  s_algorithm_usage_bitmap; /* For compression */
          /*
           * Performance hints.  Directory preallocation should only
           * happen if the EXT2_COMPAT_PREALLOC flag is on.
           */
          __u8    s_prealloc_blocks;      /* Nr of blocks to try to preallocate*/
          __u8    s_prealloc_dir_blocks;  /* Nr to preallocate for dirs */
          __u16   s_padding1;
          /*
           * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
           */
          __u8    s_journal_uuid[16];     /* uuid of journal superblock */
          __u32   s_journal_inum;         /* inode number of journal file */
          __u32   s_journal_dev;          /* device number of journal file */
          __u32   s_last_orphan;          /* start of list of inodes to delete     */
          __u32   s_hash_seed[4];         /* HTREE hash seed */
          __u8    s_def_hash_version;     /* Default hash version to use */
          __u8    s_reserved_char_pad;
          __u16   s_reserved_word_pad;
          __le32  s_default_mount_opts;
	  __le32  s_first_meta_bg;        /* First metablock block group */
        __u32   s_reserved[190];        /* Padding to the end of the block */
  };
  
  /*
   * Structure of a blocks group descriptor
  */
  struct ext2_group_desc
  {
          __le32  bg_block_bitmap;                /* Blocks bitmap block */
          __le32  bg_inode_bitmap;                /* Inodes bitmap block */
          __le32  bg_inode_table;         /* Inodes table block */
          __le16  bg_free_blocks_count;   /* Free blocks count */
          __le16  bg_free_inodes_count;   /* Free inodes count */
          __le16  bg_used_dirs_count;     /* Directories count */
          __le16  bg_pad;
         __le32  bg_reserved[3];
 };
 /*
  * Constants relative to the data blocks
 */
 #define EXT2_NDIR_BLOCKS                12
 #define EXT2_IND_BLOCK                  EXT2_NDIR_BLOCKS
 #define EXT2_DIND_BLOCK                 (EXT2_IND_BLOCK + 1)
 #define EXT2_TIND_BLOCK                 (EXT2_DIND_BLOCK + 1)
 #define EXT2_N_BLOCKS                   (EXT2_TIND_BLOCK + 1) 
/*
  * Structure of an inode on the disk
*/
 struct ext2_inode {
         __le16  i_mode;         /* File mode */
         __le16  i_uid;          /* Low 16 bits of Owner Uid */
         __le32  i_size;         /* Size in bytes */
         __le32  i_atime;        /* Access time */
         __le32  i_ctime;        /* Creation time */
         __le32  i_mtime;        /* Modification time */
         __le32  i_dtime;        /* Deletion Time */
         __le16  i_gid;          /* Low 16 bits of Group Id */
         __le16  i_links_count;  /* Links count */
         __le32  i_blocks;       /* Blocks count */
         __le32  i_flags;        /* File flags */
         union {
                 struct {
                         __le32  l_i_reserved1;
                 } linux1;
                 struct {
                         __le32  h_i_translator;
                 } hurd1;
                 struct {
                        __le32  m_i_reserved1;
                 } masix1;
         } osd1;                         /* OS dependent 1 */
	__le32  i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
 }; 
#define EXT2_NAME_LEN 255
struct ext2_dir_entry_2 {
         __le32  inode;                  /* Inode number */
         __le16  rec_len;                /* Directory entry length */
         __u8    name_len;               /* Name length */
         __u8    file_type;
         char    name[EXT2_NAME_LEN];    /* File name */
};

#if 0
#define PRINT(x)	printf(#x " = %d\n", x)
#define PRINTP(x)	printf(#x " = %s\n", x)
#else
#define PRINT(x)
#define PRINTP(x)
#endif

int inode_count = 0;
int block_count = 0;
int group_block = 0;
int inode_size = 0;
int block_size = 0;
char *p = NULL;

void myputns(char *buf, int len)
{
	int i = 0;
	
	for(i = 0; i < len; i++)
		putchar(buf[i]);
}

struct ext2_super_block *get_super_block(void)
{
	return (struct ext2_super_block *)(p + 0x400);
}

void *get_block(int block)
{
	return (void *)(p + block * block_size);
}

struct ext2_inode *get_inode(int inode)
{
	int inode_table_block;
	
	inode_table_block = ((struct ext2_group_desc *)get_block(group_block))->bg_inode_table;
	
	inode--;

	return (struct ext2_inode *)(get_block(inode_table_block) + inode * inode_size);
}

void ls(void)
{
	int len = 0;
        struct ext2_dir_entry_2 *pdir;

	pdir = (struct ext2_dir_entry_2 *)get_block(get_inode(2)->i_block[0]);

	while(len < block_size)
	{
		PRINT(pdir->inode);
		PRINT(pdir->rec_len);
		PRINT(pdir->name_len);
		myputns(pdir->name, pdir->name_len);
		putchar('\t');

		len += pdir->rec_len;

		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}
	
	putchar('\n');

}

int namei(char *filename)
{
	struct ext2_dir_entry_2 *pdir;
	int len = 0;

	pdir = (struct ext2_dir_entry_2	*)get_block(get_inode(2)->i_block[0]);

	while(len < block_size)
	{
		PRINTP(filename);
		PRINTP(pdir->name);
		PRINTP(pdir->name_len);
		PRINT(strlen(filename));
		
		if(strlen(filename) == pdir->name_len)
		{
			if(strncmp(filename, pdir->name, pdir->name_len) == 0)
				return pdir->inode;
		}

		len += pdir->rec_len;
		pdir = (struct ext2_dir_entry_2 *)((int)pdir + pdir->rec_len);
	}

}

void cat(char *filename)
{
	struct ext2_inode *inode;
	int i = 0;

	i = namei(filename);

	if(i > 0)
	{
		inode = get_inode(i);
		myputns(get_block(inode->i_block[0]), inode->i_size);
	}
	else
		printf(" %s not found\n", filename);

	return ;
}

int main(int argc, char *argv[])
{
	char *filename = "fs";
	int fd = 0;
	int tmp;

	if(argc >= 2)
		filename = argv[1];
	
	fd = open(filename, O_RDWR);
	
	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	PRINT(get_super_block()->s_inodes_count);
	PRINT(get_super_block()->s_blocks_count);
	PRINT(get_super_block()->s_inode_size);
	PRINT(get_super_block()->s_first_data_block);
	PRINT(get_super_block()->s_log_block_size);

	inode_size = get_super_block()->s_inode_size;
	block_size = 1024 << (get_super_block()->s_log_block_size);

	PRINT(inode_size); 
	PRINT(block_size);

	group_block = get_super_block()->s_first_data_block + 1;
	PRINT(group_block);

	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_block_bitmap);
	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_inode_bitmap);
	PRINT(((struct ext2_group_desc *)(get_block(group_block)))->bg_inode_table);

	PRINT(get_inode(2)->i_block[0]);

#if 0
	printf("<<<<<<<<<<1.ls()>>>>>>>>>>>>>>>\n");
	printf("<<<<<<<<<<2.cat()>>>>>>>>>>>>>>\n");

	scanf("%d", &tmp);
	printf("myshell: tmp\n");
	switch(tmp){


		case 1:	
				ls();
//		case 2:
//				cat();
		default:
				break;
	}
#endif	
	while(1)
	{
		char buf[64];
		char cmd[32];
		char arg[32];
		int ret = 0;

		printf("myshell # ");

		fgets(buf, 64, stdin);
		ret = sscanf(buf, "%s %s", cmd, arg);

		if(ret == 0)
			continue;

		PRINTP(cmd);
		PRINTP(arg);
	
		if(strcmp(cmd, "ls") == 0)
			ls();
		if(strcmp(cmd, "cat") == 0)
			cat(arg);
		if(strcmp(cmd, "exit") == 0)
			break;
		
	}
	
	munmap(p, 1024*1024);
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

struct inode
{
	short st_mode;
	short user;
	int size;
	int time[4];
	short group;
	short links;
	int blockcount;
	int flags;
	int os;
	int block[15];
};

struct dir_file
{
	int inode;
	short record_len;
	char name_len;
	char file_type;
	char name;
};
typedef struct dir_file dir_file_t;

void myputns(char *buf, int len)
{
	int i = 0;
	for(i = 0; i < len; i++ )
		putchar(buf[i]);
	putchar('\n');
}

int main(int argc, char *argv[])
{
	char *filename = "fs";
	int fd;
	dir_file_t *pdir;
	char *p;
	int *inode;
	struct inode *in;
	int len = 0;

	if(argc >= 2)
		filename = argv[1];
	fd = open(filename, O_RDWR);

	p = mmap(NULL, 1024*1024, PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

#define INODE_SIZE		128
#define INODE_TABLE_ADDR	0x2000
#define inode(x)		INODE_TABLE_ADDR + INODE_SIZE *(x - 1)


#define BLOCK_SIZE		1024
#define block(x)		BLOCK_SIZE * x	

#define PRINT(x)		printf(#x " = %d\n", x)

	pdir = (dir_file_t *) (p + 0x6000);
	
	while(len < 1024)
	{
		printf("inode : 0x%x\n", pdir->inode);
		printf("record_len : 0x%x\n", pdir->record_len);
		printf("name_len : 0x%x\n", pdir->name_len);
		printf("file_type : %d\n", pdir->file_type);
		myputns(&pdir->name, pdir->name_len);

		if(pdir->file_type == 1)
		{
			
			in =(struct inode *) (p + inode(pdir->inode));
			PRINT(in->size);
			PRINT(in->block[0]);
			myputns((char *)(p + block(in->block[0])), in->size);
		}
		
		len += pdir->record_len;

		pdir = (dir_file_t *)((int)pdir + pdir->record_len);
		 
	}

	munmap(p, 10);
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define OFFSET	0x34
#define SIZE	0x22

void put_hex(char c)
{
	char *hex = "0123456789abcdef";

	putchar(hex[((c >> 4) & 0x0F)]);
	putchar(hex[((c >> 0) & 0x0F)]);
	putchar (' ');

	return ;
}

int main(int argc, char *argv[])
{
	int fd = 0;
	char *filename = "read";
	int counter = 0;
	char c = 0;
	int ret = 0;

	if(argc >= 2)
	{
		filename = argv[1];
	}

	fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		perror("open filename");
		exit(1);
	}

	lseek(fd, OFFSET, SEEK_SET);

	while(1)
	{
		ret = read(fd, &c, 1);	
		if(ret == 0)
			break;

		put_hex(c);
		printf("%p\n", &c);		
		counter++;
		
		if(counter % 16 == 0)
			putchar('\n');
		if(counter == SIZE)
			break;
	}

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	char buf[10];
	int n = 0;

	n = read(STDIN_FILENO, buf, 10);
//	printf("n = %d\n", n);

	if(n < 0)
	{
		perror("read STDIN_FILENO");
		exit(1);
	}

	write(STDOUT_FILENO, buf, n);

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int flags = 0;
	char c = 'c';
	int ret = 0;
	int fd = 0;
	
#if 0	
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);

	flags |= O_NONBLOCK;
	printf("flags = %x\n", flags); 
#endif
//	fd = open("a.txt", O_RDONLY | O_CREAT);

	fcntl(0, F_SETFL, O_RDONLY | O_NONBLOCK);
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);

	ret = read(0, &c, 1);
/*	if(ret < 0)
	{
		perror("read 0");
		exit(1);
	}
*/	
	putchar(c);
	putchar('A');

	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int flags = 0;
	int ret = 0;
	int counter = 0;
	char buf[10];
	
	fcntl(0, F_SETFL, O_RDWR | O_NONBLOCK);
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);
		
tryagain:
	if(counter >5)
		goto end;

	ret = read(0, buf, 10);
	counter ++;
	
	if(ret < 0)
	{
		goto tryagain;
	}

	write(0, buf, ret);
end:
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
	int fd = 0;
	int n = 0;
	char buf[10];
	int counter = 0;

	fd = open("./a", O_RDONLY | O_NONBLOCK);
	if(fd < 0)
	{
		perror("open a.txt");
		exit(1);
	}

tryagain:
		
	if(counter >= 5)
		goto end;

	sleep(1);	
	n = read(fd, buf, 10);

	counter ++;

	if(n < 0)
	{
		printf("errno = %d\n", errno);
		printf("n = %d\n", n);
		goto tryagain;
	}

	n = write(1, buf, n);
end:
	return 0;
}

#include <stdio.h>
#include <unistd.h>

#define PRINT(x)	printf(#x " = %d\n", (int)x)

int main(void)
{
	PRINT(STDIN_FILENO);
	PRINT(STDOUT_FILENO);
	PRINT(STDERR_FILENO);

	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE	1024

int main(int argc, char *argv[])
{
	int fd = 0;
	char buf[BUFSIZE];	
	int ret = 0;
	
	if(argc >= 2)	
		fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0664 );	
	if(fd < 0)
	{
		perror("open argv[1]");
		exit(1);
	}

	while(1)
	{
		ret = read(0, buf, BUFSIZE);
		if(ret == 0)
			break;
			
		write(1, buf, ret);

		if(fd > 0)
			write(fd, buf, ret);
			
	}

	if(fd > 0)
		close(fd);

	return 0;
}


hello
ls
cd 
pwd
mkdir

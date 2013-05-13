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

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


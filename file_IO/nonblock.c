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


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



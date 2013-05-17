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

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

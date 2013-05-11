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

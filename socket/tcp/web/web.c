#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define SER_IP "127.0.0.1"
#define SER_PORT 8000
#define ROOT "./www"

#define REPLY_HEAD "HTTP/1.1 200 OK\r\nContent-Type:"
#define TEXT "text/html"
#define IMG "image/png"
#define REPLY_END "\r\n\r\n"


void err_sys(char *str)
{
	perror(str);
	exit(1);
}

int main(void)
{
	int lfd, cfd;
	int n;
	char buf[1024];
	struct sockaddr_in serv_addr;
	char *argv[2];

	lfd = socket(AF_INET, SOCK_STREAM, 0);
	if(lfd == -1)
	{
		err_sys("socket err");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SER_PORT);
	inet_pton(AF_INET, SER_IP, &serv_addr.sin_addr.s_addr);

	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	n = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	if(n == -1)
		err_sys("bine error");
	n = listen(lfd, 10);
	if(n == -1)
		err_sys("listen error");
	while(1)
	{
		char path[1024];
		char *p, *q, *filename, *type;
		int fd;

		cfd = accept(lfd, NULL, NULL);
		if(cfd == -1)
			err_sys("accept error");

		n = read(cfd, buf, 1024);
		buf[n] = '\0';
		p = strstr(buf, " ");
		q = strstr(p + 1, " ");
		*q = '\0';

		filename = p + 2;
		sprintf(path, "%s/%s", ROOT, filename);

		n = access(path, X_OK); /* 判断文件是否是cgi文件（可执行文件） */
		if(n == 0) /* cgi */
		{
			pid_t pid;

			write(cfd, REPLY_HEAD, strlen(REPLY_HEAD));
			write(cfd, TEXT, strlen(TEXT));
			write(cfd, REPLY_END, strlen(REPLY_END)); 
			
			pid = fork();	
			if(pid == 0) /* child */
			{
				dup2(cfd, STDOUT_FILENO);	
				argv[0] = path;
				argv[1] = NULL;
				execvp(argv[0], argv);
				//execlp(path, path, NULL);
				err_sys("execlp");
			}
			else /* parent */
			{
				close(cfd);
				wait(NULL);
			}

		}
		else
		{

			p = rindex(filename, '.');
			if(strcmp(p + 1, "png") == 0)
				type = IMG;
			else
				type = TEXT;

			printf("%s\n", filename);
			fd = open(path, O_RDONLY);
			if(fd != -1)
			{
				/* 底层传送几个数据包跟上层调用几次write无关，内核自己实现, 与发送的字节数相关 */
				write(cfd, REPLY_HEAD, strlen(REPLY_HEAD));
				write(cfd, type, strlen(type));
				write(cfd, REPLY_END, strlen(REPLY_END)); 

				while(n = read(fd, buf, 1024))
					write(cfd, buf, n);
				close(fd);
			}
			printf("%s\n", filename);
		}

		close(cfd);
	}

	return 0;
}

/* server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#define MAXLINE 80
#define SERV_PORT 8000

int main(void)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char buf[MAXLINE] = {0};
	char str[INET_ADDRSTRLEN];
	int i, n, len = 0;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
    
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(listenfd, 20);

	printf("Accepting connections ...\n");
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, 
				(struct sockaddr *)&cliaddr, &cliaddr_len);
//------------------------------------------------------------------------------
while(1)		
{
		puts("1");
		bzero(buf,sizeof(buf));
		if(read(connfd,&len,sizeof(len))<=0)
			break;
		puts("2");
		if(read(connfd,buf,len)<=0)
			break;

		puts(buf);
		int fd = open(buf,O_WRONLY|O_TRUNC|O_CREAT,0644);
		if(read(connfd,&len,sizeof(len))<=0)
			break;
		while(len > 0)
		{
			if(len > sizeof(buf))
				n = read(connfd,buf,sizeof(buf));
			else
				n = read(connfd,buf,len);	
			if(n > 0)
			write(fd,buf,n);
			else
			  break;
			len -= n;
		}

		close(fd);
		puts("file copied");
}
//------------------------------------------------------------------------------
		//n = read(connfd, buf, MAXLINE);
		//printf("received from %s at PORT %d\n",
		//       inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
		/////////       ntohs(cliaddr.sin_port));
    
		//for (i = 0; i < n; i++)
		//	buf[i] = toupper(buf[i]);
		//write(connfd, buf, n);
		close(connfd);
	}
}

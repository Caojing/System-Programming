#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>

#define MAXLINE 80
#define SERV_PORT 8000

int main(int argc, char* argv[])
{
	struct sockaddr_in servaddr;
	socklen_t cliaddr_len;
	int sockfd;
	char buf[MAXLINE]={0};
	int i, n;
	char *p=NULL;
	char *q = NULL;
	struct stat stbuf;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1],&servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	//write(sockfd, str,strlen(str));
	//n = read(sockfd,buf,MAXLINE);
	//printf("Response from server:\n");
	//write(1,buf,n);
//----------------------------------------------------------------------------
while(1)
{       puts("Input the file names,like \"1.txt new.txt\"");
	bzero(buf,sizeof(buf));
	gets(buf);
	puts(buf);
	if(!strncmp(buf,"exit",4))
		break;
	p = strtok(buf," ");
	puts(p);
	q = strtok(NULL," ");
	puts(q);
	if(!p || !q)
		break;
	n = strlen(q);
	write(sockfd,&n,sizeof(n));
	write(sockfd,q,strlen(q));
	stat(p,&stbuf);
	n = stbuf.st_size;
	write(sockfd,&n,sizeof(n));
	int fd = open(p,O_RDONLY);
	while((n = read(fd,buf,sizeof(buf)))>0)
	{
		write(sockfd,buf,n);
	}
	close(fd);
puts("transfer completed");
}
//----------------------------------------------------------------------------
	puts("exit...");

	close(sockfd);

	return 0;
}

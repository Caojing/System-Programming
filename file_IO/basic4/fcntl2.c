#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
	int flags = 0;
	int ret = 0;
	int counter = 0;
	char buf[10];
	
	fcntl(0, F_SETFL, O_RDWR | O_NONBLOCK);
	flags = fcntl(0, F_GETFL);
	printf("flags = %x\n", flags);
		
tryagain:
	if(counter >5)
		goto end;

	ret = read(0, buf, 10);
	counter ++;
	
	if(ret < 0)
	{
		goto tryagain;
	}

	write(0, buf, ret);
end:
	return 0;
}


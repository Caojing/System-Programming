#include <arpa/inet.h>
#include <stdio.h>

int main(void)
{
	int mm = 0x12345678;
	int b = 0;
	int a = 0;

	if((*(char *)&mm) == 0x78)
	{
		printf("this is xiaoduan\n");

		b = htonl(0x12345678);

		printf("%0x\n", b);
	}
	else
	{
		printf("this is daduan\n");
	}
	

	return 0;

}

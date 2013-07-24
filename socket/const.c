#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int main(void)
{
	char str[] = "192.168.0.55";
	char *token;

	token = strtok(str, ".");
	printf("%0x", ((int)(token)) / 16);
/*
	while(token = strtok(NULL, "."))
	{
		printf("%0x", (unsigned int)token);
	}
	
*/
	
}

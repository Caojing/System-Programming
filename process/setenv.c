#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("%s\n", getenv("PATH"));
	setenv("PATH", "myhello", 1);
	printf("%s\n", getenv("PATH"));


	return 0;
}

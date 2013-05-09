#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int counter = 0;
	
	alarm(1);
	for(counter = 0; ;)
	{
		printf("%d\n", ++counter);
	}

	return 0;
}

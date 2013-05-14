#include <stdio.h>
#include <unistd.h>

#define PRINT(x)	printf(#x " = %d\n", (int)x)

int main(void)
{
	PRINT(STDIN_FILENO);
	PRINT(STDOUT_FILENO);
	PRINT(STDERR_FILENO);

	return 0;
}

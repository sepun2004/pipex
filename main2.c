#include "pipex.h"

int	main(void)
{
	fork();
	fork();
	fork();
	printf("Hello\n");
	return (0);
}
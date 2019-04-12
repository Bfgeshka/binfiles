#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
	srand(time(NULL));

	int range = atoi(argv[1]);
	int result;

		if (range < 1)
			{result = 1;}
		else
			{result = rand()%range + 1;}

	printf("%d", result);
	return 0;
}

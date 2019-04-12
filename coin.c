#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	srand((unsigned)time(NULL));
	int coin;
	coin = rand()%2;

	if (coin == 1) 	printf("Heads\n"); else
					printf("Tails\n");

	return 0;
}	

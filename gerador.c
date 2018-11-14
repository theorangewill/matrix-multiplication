#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int n = 4;
	srand(time(NULL));
	printf("%d\n", n);
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++) printf("%d ", rand() % 1000);
		printf("\n");
	}
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++) printf("%d ", rand() % 1000);
		printf("\n");
	}
	return 0;
}

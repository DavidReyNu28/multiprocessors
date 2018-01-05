#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10

int main(int argc, char* argv[]) {
	int i;
	
	#pragma omp parallel 
	{
		int id = omp_get_thread_num();
		for (i = 0; i < N; i++) {
			printf("thread %i - i = %i\n", id, i);
		}
	}
	return 0;
}

#include <iostream>
#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include "utils/cppheader.h"

using namespace std;
using namespace tbb;

const int SIZE = 1e9;
const int GRAIN = 1e5;

class ArrayInitializer {
private:
	int *myArray;

public:
	ArrayInitializer(int *array) : myArray(array) {
	}
	
	void operator() (const blocked_range<int> &r) const {
		for (int i = r.begin(); i != r.end(); i++) {
			myArray[i] = (i % MAX_VALUE) + 1;
		}
	}
};

int main(int argc, char* argv[]) {
	Timer t;
	double ms;
	int *a = new int[SIZE];
	
	cout << "Starting...\n";
	ms = 0;
	for (int i = 0; i < N; i++) {
		t.start();
		parallel_for( blocked_range<int>(0, SIZE, GRAIN), ArrayInitializer(a) );
		ms += t.stop();
	}
	display_array("a:", a);
	cout << "avg time = " << (ms / N) << " ms\n";
	
	delete [] a;
	return 0;
}

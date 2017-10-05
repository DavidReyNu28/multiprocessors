#include <iostream>
#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include "utils/cppheader.h"

using namespace std;
using namespace tbb;

const int SIZE = 1000000000;
const int GRAIN = 100000;

class ParallelSum {
private:
	int *myArray;
	
public:
	long sum;
	
	ParallelSum(int *array) : myArray(array), sum(0) {}
	
	ParallelSum(ParallelSum &x, split)
		: myArray(x.myArray), sum(0) {}
		
	void operator() (const blocked_range<int> &r) {
		for (int i = r.begin(); i != r.end(); i++) {
			sum += myArray[i];
		}
	}
	
	void join(const ParallelSum & x) {
		sum += x.sum;
	}
};

int main(int argc, char* argv[]) {
	Timer t;
	double ms;
	long result;
	int *a = new int[SIZE];
	
	fill_array(a, SIZE);
	display_array("a", a);
	
	cout << "Starting...\n";
	ms = 0;
	for (int i = 0; i < N; i++) {
		t.start();
		ParallelSum obj(a);
		parallel_reduce( blocked_range<int>(0, SIZE, GRAIN),
							obj);
		result = obj.sum;
		ms += t.stop();
	}
	
	cout << "sum = " << result << endl;
	cout << "avg time = " << (ms/N) << " ms\n";
	
	delete [] a;
	
	return 0;
}

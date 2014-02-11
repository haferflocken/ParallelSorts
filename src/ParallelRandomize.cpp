#include <cstdlib>
#include <fstream>
#include "SizedArray.h"

using namespace std;

const unsigned int RAND_DATA_SIZE = 32768;
const int RAND_RANGE = 10000;
const int RAND_CUTOFF_SIZE = 1024;

SizedArray* array;	// The array to work with.

// Randomize an array in parallel.
void* pRandom(void* args) {
	Range* range = (Range*)args;
	
	// If we're smaller than the cutoff, just iterate through this section and randomize it.
	if (range->max - range->min <= RAND_CUTOFF_SIZE) {
		for (int i = range->min; i < range->max; i++) {
			array->set(i, rand() % RAND_RANGE);
		}
	}
	
	// Otherwise, divide and conquer.
	else {
		// Break the problem in half.
		Range* halves = range->makeHalves();
		
		// Branch another thread to work the left half and work the right half in this thread.
		pthread_t left;
		pthread_create(&left, NULL, pRandom, (void*)halves);
		pRandom((void*)&halves[1]);
		
		// Wait until we're finished.
		pthread_join(left, NULL);
		delete halves;
	}
}

int main() {
	// Randomize the data.
	array = new SizedArray(RAND_DATA_SIZE);
	Range fullRange = array->getFullRange();
	pRandom(&fullRange);
	
	// Output the randomized data.
	ofstream randFile;
	randFile.open("randomized.txt");
	array->print(randFile);
	randFile.close();
}

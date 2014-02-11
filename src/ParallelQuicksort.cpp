#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <fstream>
#include "SizedArray.h"

using namespace std;

// Any ranges smaller than CUTOFF_SIZE are processed by the current thread
// rather than branching off more threads.
const int CUTOFF_SIZE = 1024;

// The array to work with.
SizedArray* array;

// Moves everything smaller than the pivot to the left of the pivot and
// returns the new location of the pivot.
int pivotWork(Range* range) {
	int p = range->min;
	int temp;
	for (int i = range->min + 1; i < range->max; i++) {
		if (array->get(i) < array->get(range->min)) {
			p++;
			temp = array->get(i);
			array->set(i, array->get(p));
			array->set(p, temp);
		}
	}
	temp = array->get(p);
	array->set(p, array->get(range->min));
	array->set(range->min, temp);
	return p;
}

// A non parallel quicksort.
void quicksort(Range* range) {
	int pivotLoc = pivotWork(range);
	if (pivotLoc > range->min) {
		Range r;
		r.min = range->min;
		r.max = pivotLoc;
		quicksort(&r);
	}
	if (range->max - 1 > pivotLoc + 1) {
		Range r;
		r.min = pivotLoc + 1;
		r.max = range->max;
		quicksort(&r);
	}
}

// Quicksort in parallel.
void* pQuicksort(void* args) {
	Range* range = (Range*)args;
	
	// If we're <= the cutoff, just do the work.
	if (range->max - range->min <= CUTOFF_SIZE) {
		quicksort(range);
	}
	
	// Otherwise, do it in parallel.
	else {
		// Use the first element as the pivot.
		// Move everything smaller than the pivot to the left of the pivot.
		int pivotLoc = pivotWork(range);
	
		// Branch a thread to handle the left half of the array.
		// pQuicksort the right half.
		Range* split = range->splitAround(pivotLoc);
		pthread_t left;
		pthread_create(&left, NULL, pQuicksort, (void*)split);
		pQuicksort((void*)&split[1]);
	
		// Wait until we're finished.
		pthread_join(left, NULL);
		delete split;
	}
}

int main() {
	// Read in the file.
	ifstream randFile;
	randFile.open("randomized.txt");
	if (!randFile.is_open()) {
		return 1;
	}
	array = new SizedArray(randFile);
	randFile.close();
	
	// Make a range for the whole array.
	Range range = array->getFullRange();
	
	// Sort the array.
	pQuicksort((void*)&range);
	
	// Print the results.
	ofstream sortedFile;
	sortedFile.open("quicksorted.txt");
	array->print(sortedFile);
	sortedFile.close();
	
	// Clean up.
	delete array;
}



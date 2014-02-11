#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "SizedArray.h"

using namespace std;

const int CUTOFF = 64;

SizedArray* array;
int* mTemp;

void merge(int lo, int mid, int hi) {
	int a = lo, b = mid;
	for (int k = lo; k < hi; k++) {
		if (a < mid && (b >= hi || array->get(a) < array->get(b)))
			mTemp[k] = array->get(a++);
		else
			mTemp[k] = array->get(b++);
	}
	for (int k = lo; k < hi; k++) {
		array->set(k, mTemp[k]);
	}
}

void mergeSort(int lo, int hi) {
	if (lo < hi - 1) {
		int mid = (lo + hi) / 2;
		mergeSort(lo, mid);
		mergeSort(mid, hi);
		merge(lo, mid, hi);
	}
}

// Sort a range with thread powered divide and conquer.
void* sortRange(void* ptr) {
	Range* range = (Range*)ptr;
	int lo = range->min;
	int hi = range->max;
	
	// If hi - lo <= CUTOFF, sort that part of the array.
	if (hi - lo <= CUTOFF) {
		mergeSort(lo, hi);
	}
	// Otherwise, fork a thread that sorts half the problem and then merge.
	else {
		Range* halves = range->makeHalves();
		
		pthread_t leftThread;
		pthread_create(&leftThread, NULL, sortRange, &halves[0]);
		sortRange(&halves[1]);
		pthread_join(leftThread, NULL);
		
		merge(halves[0].min, halves[1].min, halves[1].max);
		
		delete[] halves;
	}
}

// Randomize the data and then sort it.
int main() {
	// Read in the file.
	ifstream randFile;
	randFile.open("randomized.txt");
	if (!randFile.is_open()) {
		return 1;
	}
	array = new SizedArray(randFile);
	randFile.close();
	
	// Make the temp array.
	mTemp = new int[array->getSize()];
	
	// Sort the data.
	Range fullRange = array->getFullRange();
	sortRange(&fullRange);
	
	// Output the sorted data.
	ofstream sortedFile;
	sortedFile.open("mergesorted.txt");
	array->print(sortedFile);
	sortedFile.close();
	
	// Clean up and exit.
	delete mTemp;
	return 0;
}













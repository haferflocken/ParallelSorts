#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Range.h"

using namespace std;

const int CUTOFF = 64;
const int DATA_SIZE = 32768;

int data[DATA_SIZE];
int mTemp[DATA_SIZE];

void printData(ostream& out) {
	int numRows = DATA_SIZE / CUTOFF;
	for (int r = 0; r < numRows; r++) {
		int rOff = r * CUTOFF;
		for (int c = 0; c < CUTOFF; c++) {
			out << data[rOff + c] << ", ";
		}
		out << endl;
	}
}

void merge(int lo, int mid, int hi) {
	int a = lo, b = mid;
	for (int k = lo; k < hi; k++) {
		if (a < mid && (b >= hi || data[a] < data[b]))
			mTemp[k] = data[a++];
		else
			mTemp[k] = data[b++];
	}
	for (int k = lo; k < hi; k++) {
		data[k] = mTemp[k];
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
		Range* halves = splitRange(range);
		
		pthread_t leftThread;
		pthread_create(&leftThread, NULL, sortRange, &halves[0]);
		sortRange(&halves[1]);
		pthread_join(leftThread, NULL);
		
		merge(halves[0].lo, halves[1].lo, halves[1].hi);
		
		delete[] halves;
	}
}

// Randomize a range with thread powered divide and conquer.
void* randomizeRange(void* ptr) {
	Range* range = (Range*)ptr;
	int lo = range->min;
	int hi = range->max;
	
	// If hi - lo <= CUTOFF, randomize that part of the array.
	if (hi - lo <= CUTOFF) {
		for (int i = lo; i < hi; ++i) {
			data[i] = rand() % 10000;
		}
	}
	// Otherwise, fork two threads that each randomize half the problem.
	else {
		Range* halves = splitRange(range);
		
		pthread_t leftThread;
		pthread_create(&leftThread, NULL, randomizeRange, &halves[0]);
		randomizeRange(&halves[1]);
		pthread_join(leftThread, NULL);
		
		delete[] halves;
	}
}

// Randomize the data and then sort it.
int main() {
	Range fullRange;
	fullRange.min = 0;
	fullRange.max = DATA_SIZE;

	// Randomize the data.
	pthread_t randThread;
	int randRet = pthread_create(&randThread, NULL, randomizeRange, &fullRange);
	pthread_join(randThread, NULL);
	cout << "Randomization returned " << randRet << endl;
	if (randRet != 0)
		return 1;
	
	// Output the randomized data.
	ofstream randFile;
	randFile.open("randomized.txt");
	printData(randFile);
	randFile.close();
	
	// Sort the data.
	pthread_t sortThread;
	int sortRet = pthread_create(&sortThread, NULL, sortRange, &fullRange);
	pthread_join(sortThread, NULL);
	cout << "Sort returned " << sortRet << endl;
	if (sortRet != 0)
		return 2;
	
	// Output the sorted data.
	ofstream sortedFile;
	sortedFile.open("sorted.txt");
	printData(sortedFile);
	sortedFile.close();
	
	// Clean up and exit.
	return 0;
}













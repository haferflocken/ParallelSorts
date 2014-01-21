#include "SizedArray.h"

struct PRArgs {
	Range* range;
	int* array;
	int randomRange;
};

// Randomize an array in parallel.
void* pRandom(void* args) {
	PRArgs* pRArgs = (PRArgs*)args;
	Range* range = pRArgs->range;
	int* array = pRArgs->array; 
	
	// If we're smaller than the cutoff, just iterate through this section and randomize it.
	if (range->max - range->min <= CUTOFF_SIZE) {
		for (int i = range->min; i < range->max; i++) {
			array[i] = rand() % pRArgs->randomRange;
		}
	}
	
	// Otherwise, divide and conquer.
	else {
		// Break the problem in half.
		Range* halves = range->makeHalves();
		PRArgs* subArgs = new PRArgs[2];
		subArgs[0].range = &halves[0];
		subArgs[0].array = array;
		subArgs[0].randomRange = pRArgs->randomRange;
		subArgs[1].range = &halves[1];
		subArgs[1].array = array;
		subArgs[1].randomRange = pRArgs->randomRange;
		
		// Branch another thread to work the left half and work the right half in this thread.
		pthread_t left;
		pthread_create(&left, NULL, pRandom, (void*)subArgs);
		pRandom((void*)&subArgs[1]);
		
		// Wait until we're finished.
		pthread_join(left, NULL);
		delete halves;
		delete subArgs;
	}
}

void randomize(SizedArray* sArray, int randomRange) {
	// Set up the arguments.
	PRArgs args;
	args.range = new Range;
	args.range->min = 0;
	args.range->max = sArray->size;
	args.array = sArray->array;
	args.randomRange = randomRange;
	
	// Go.
	pRandom(&args); 
}

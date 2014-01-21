#ifndef SIZED_ARRAY_H
#define SIZED_ARRAY_H

#include <iostream>
#include "Range.h"

struct SizedArray {

	unsigned int size;
	int* array;

	SizedArray(unsigned int);
	
	~SignedArray();
	
	void print(Range*, ostream&);
};

#endif /* SIZED_ARRAY_H */



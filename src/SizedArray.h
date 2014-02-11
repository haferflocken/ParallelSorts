#ifndef SIZED_ARRAY_H
#define SIZED_ARRAY_H

#include <iostream>
#include <fstream>
#include "Range.h"

using namespace std;

class SizedArray {

	unsigned int size;
	int* array;

public:
	SizedArray(unsigned int);
	
	SizedArray(istream&);
	
	~SizedArray();
	
	int get(int index);
	
	void set(int index, int val);
	
	unsigned int getSize();
	
	Range getFullRange();
	
	void print(ostream&);
	
	void print(Range*, ostream&);
	
	void print(int start, int end, ostream&);

};

#endif /* SIZED_ARRAY_H */



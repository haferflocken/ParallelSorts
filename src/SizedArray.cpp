#include "SizedArray.h"
#include <string>
#include <vector>
#include <cstdlib>

SizedArray::SizedArray(unsigned int s) {
	size = s;
	array = new int[size];
}

SizedArray::SizedArray(istream& in) {
	string line;
	vector<int> builder;
	while (getline(in, line, ' ')) {
		builder.push_back(atoi(line.c_str()));
	}
	size = builder.size();
	array = new int[size];
	for (int i = 0; i < size; ++i) {
		array[i] = builder[i];
	}
}

SizedArray::~SizedArray() {
	delete array;
}

Range SizedArray::getFullRange() {
	Range r;
	r.min = 0;
	r.max = size;
	return r;
}

void SizedArray::print(ostream& out) {
	print(0, size, out);
}

void SizedArray::print(Range* range, ostream& out) {
	int lo = (range->min > 0)? range->min : 0;
	int hi = (range->max < size)? range->max : size;
	print(lo, hi, out);
}

void SizedArray::print(int start, int end, ostream& out) {
	if (end - start > 0) {
		out << array[start];
		for (int i = start + 1; i < end; i++)
			out << " " << array[i];
	}
}

int SizedArray::get(int index) {
	return array[index];
}

void SizedArray::set(int index, int val) {
	array[index] = val;
}

unsigned int SizedArray::getSize() {
	return size;
}

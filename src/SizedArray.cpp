#include "SizedArray.h"

SizedArray::SizedArray(unsigned int s) {
	size = s;
	array = new int[size];
};

SizedArray::~SizedArray() {
	delete array;
};

SizedArray::print(Range* range, ostream& out) {
	int lo = (range->min > 0)? range->min : 0;
	int hi = (range->max < size)? range->max : size;
	if (hi - lo < 1) {
		out << "[]";
	}
	else {
		out << "[ " << array[lo];
		for (int i = lo + 1; i < hi; i++)
			out << ", " << array[i];
		out << " ]";
	}
};



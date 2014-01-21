#ifndef RANGE_H
#define RANGE_H

struct Range {
	int min;
	int max;
	
	Range* splitAround(int);
	
	Range* makeHalves();
};

#endif /* RANGE_H */



#include "Range.h"

Range* Range::split(int s) {
	Range* out = new Range[2];
	out[0].min = min;
	out[0].max = s;
	out[1].min = s + 1;
	out[1].max = max;
	return out;
};

Range* Range::makeHalves() {
	Range* pair = new Range[2];
	pair[0].lo = min;
	pair[0].hi = min + (max - min) / 2;
	pair[1].lo = pair[0].hi;
	pair[1].hi = max;
	return pair;
}

#include "testforge/is_equal_statdata.h"
#include <math.h>

// TODO: уточнить значение EPS
#define FLOAT_EPS 1e-4f

int isEqualStatData(const StatData *lhs, const StatData *rhs)
{
	if (lhs == NULL && rhs == NULL) {
		return 1;
	}
	else if (lhs == NULL || rhs == NULL){
		return 0;
	}
	if (lhs->id != rhs->id) {
		return 0;
	}
	if (lhs->count != rhs->count) {
		return 0;
	}
	if (fabsf(lhs->cost - rhs->cost) > FLOAT_EPS) {
		return 0;
	}
	if (lhs->primary != rhs->primary) {
		return 0;
	}
	if (lhs->mode != rhs->mode) {
		return 0;
	}

	return 1;
}

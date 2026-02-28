#ifndef TEST_CASE_H
#define TEST_CASE_H

#include "stat_data.h"
#include <stddef.h>

typedef struct TestCase {
	const char *name;

	const StatData *in_a;
	size_t in_a_size;

	const StatData *in_b;
	size_t in_b_size;

	const StatData *expected;
	size_t expected_size;

	const char *input_a_path;
	const char *input_b_path;
	const char *output_path;

} TestCase;

#endif //TEST_CASE_H
#include "stat_data.h"
#include "testforge/test_case.h"
#include "testforge/run_test_case.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static const TestCase tests[] = {
	{
		.name = "TC-01: basic merge and sort",
		.in_a = (const StatData[]) {
			{.id = 90889, .count = 13, .cost = 3.567f, .primary = 0, .mode = 3},
			{.id = 90089, .count = 1, .cost = 88.90f, .primary = 1, .mode = 0}
		},
		.in_a_size = 2,
		.in_b = (const StatData[]) {
			{.id = 90089, .count = 13, .cost = 0.011f, .primary = 0, .mode = 2},
			{.id = 90189, .count = 1000, .cost = 1.00003f, .primary = 1, .mode = 2}
		},
		.in_b_size = 2,
		.expected = (const StatData[]) {
			{.id = 90189, .count = 1000, .cost = 1.00003f, .primary = 1, .mode = 2},
			{.id = 90889, .count = 13, .cost = 3.567f, .primary = 0, .mode = 3},
			{.id = 90089, .count = 14, .cost = 88.911f, .primary = 0, .mode = 2}
		},
		.expected_size = 3,
		.input_a_path = "build/case_1_in_a.dump",
		.input_b_path = "build/case_1_in_b.dump",
		.output_path   = "build/case_1_out.dump"
	},
	{
		.name = "TC-02: both input files are empty",
		.in_a = NULL,
		.in_a_size = 0,
		.in_b = NULL,
		.in_b_size = 0,
		.expected = NULL,
		.expected_size = 0,
		.input_a_path = "build/case_2_in_a.dump",
		.input_b_path = "build/case_2_in_b.dump",
		.output_path   = "build/case_2_out.dump"
	},
	{
		.name = "TC-03: one file empty, one has data",
		.in_a = NULL,
		.in_a_size = 0,
		.in_b = (const StatData[]) {
			{.id = 3001, .count = 7, .cost = 50.0f, .primary = 1, .mode = 1},
			{.id = 3002, .count = 2, .cost = -1.5f, .primary = 0, .mode = 4},
			{.id = 3003, .count = 8, .cost = 2.25f, .primary = 1, .mode = 7},
			{.id = 3004, .count = 1, .cost = 0.0f, .primary = 1, .mode = 0},
			{.id = 3005, .count = 9, .cost = 10.0f, .primary = 0, .mode = 3}
		},
		.in_b_size = 5,
		.expected = (const StatData[]) {
			{.id = 3002, .count = 2, .cost = -1.5f, .primary = 0, .mode = 4},
			{.id = 3004, .count = 1, .cost = 0.0f, .primary = 1, .mode = 0},
			{.id = 3003, .count = 8, .cost = 2.25f, .primary = 1, .mode = 7},
			{.id = 3005, .count = 9, .cost = 10.0f, .primary = 0, .mode = 3},
			{.id = 3001, .count = 7, .cost = 50.0f, .primary = 1, .mode = 1}
		},
		.expected_size = 5,
		.input_a_path = "build/case_3_in_a.dump",
		.input_b_path = "build/case_3_in_b.dump",
		.output_path   = "build/case_3_out.dump"
	},
	{
		.name = "TC-05: no duplicate IDs",
		.in_a = (const StatData[]) {
			{.id = 1, .count = 10, .cost = 3.0f, .primary = 1, .mode = 1},
			{.id = 2, .count = 20, .cost = 1.0f, .primary = 0, .mode = 2},
			{.id = 3, .count = 30, .cost = 2.0f, .primary = 1, .mode = 3}
		},
		.in_a_size = 3,
		.in_b = (const StatData[]) {
			{.id = 4, .count = 40, .cost = 4.0f, .primary = 0, .mode = 4},
			{.id = 5, .count = 50, .cost = 0.5f, .primary = 1, .mode = 5},
			{.id = 6, .count = 60, .cost = 5.0f, .primary = 1, .mode = 6}
		},
		.in_b_size = 3,
		.expected = (const StatData[]) {
			{.id = 5, .count = 50, .cost = 0.5f, .primary = 1, .mode = 5},
			{.id = 2, .count = 20, .cost = 1.0f, .primary = 0, .mode = 2},
			{.id = 3, .count = 30, .cost = 2.0f, .primary = 1, .mode = 3},
			{.id = 1, .count = 10, .cost = 3.0f, .primary = 1, .mode = 1},
			{.id = 4, .count = 40, .cost = 4.0f, .primary = 0, .mode = 4},
			{.id = 6, .count = 60, .cost = 5.0f, .primary = 1, .mode = 6}
		},
		.expected_size = 6,
		.input_a_path = "build/case_5_in_a.dump",
		.input_b_path = "build/case_5_in_b.dump",
		.output_path   = "build/case_5_out.dump"
	},
	{
		.name = "TC-06: one ID repeated multiple times",
		.in_a = (const StatData[]) {
			{.id = 5, .count = 1, .cost = 1.0f, .primary = 1, .mode = 1},
			{.id = 5, .count = 2, .cost = 2.0f, .primary = 1, .mode = 2}
		},
		.in_a_size = 2,
		.in_b = (const StatData[]) {
			{.id = 5, .count = 3, .cost = 3.0f, .primary = 1, .mode = 3},
			{.id = 5, .count = 4, .cost = 4.0f, .primary = 1, .mode = 4}
		},
		.in_b_size = 2,
		.expected = (const StatData[]) {
			{.id = 5, .count = 10, .cost = 10.0f, .primary = 1, .mode = 4}
		},
		.expected_size = 1,
		.input_a_path = "build/case_6_in_a.dump",
		.input_b_path = "build/case_6_in_b.dump",
		.output_path   = "build/case_6_out.dump"
	},
	{
		.name = "TC-07: primary field priority (0 wins)",
		.in_a = (const StatData[]) {
			{.id = 7001, .count = 1, .cost = 10.0f, .primary = 1, .mode = 1},
			{.id = 7002, .count = 2, .cost = 20.0f, .primary = 1, .mode = 2},
			{.id = 7003, .count = 3, .cost = 30.0f, .primary = 0, .mode = 3},
			{.id = 7004, .count = 4, .cost = 40.0f, .primary = 1, .mode = 4},
			{.id = 7004, .count = 5, .cost = 50.0f, .primary = 1, .mode = 5}
		},
		.in_a_size = 5,
		.in_b = (const StatData[]) {
			{.id = 7001, .count = 6, .cost = 60.0f, .primary = 1, .mode = 6},
			{.id = 7002, .count = 7, .cost = 70.0f, .primary = 0, .mode = 7},
			{.id = 7003, .count = 8, .cost = 80.0f, .primary = 0, .mode = 1},
			{.id = 7004, .count = 9, .cost = 90.0f, .primary = 0, .mode = 2}
		},
		.in_b_size = 4,
		.expected = (const StatData[]) {
			{.id = 7001, .count = 7, .cost = 70.0f, .primary = 1, .mode = 6},
			{.id = 7002, .count = 9, .cost = 90.0f, .primary = 0, .mode = 7},
			{.id = 7003, .count = 11, .cost = 110.0f, .primary = 0, .mode = 3},
			{.id = 7004, .count = 18, .cost = 180.0f, .primary = 0, .mode = 5}
		},
		.expected_size = 4,
		.input_a_path = "build/case_7_in_a.dump",
		.input_b_path = "build/case_7_in_b.dump",
		.output_path   = "build/case_7_out.dump"
	},
	{
		.name = "TC-08: mode field takes maximum value",
		.in_a = (const StatData[]) {
			{.id = 8001, .count = 1, .cost = 10.0f, .primary = 1, .mode = 2},
			{.id = 8002, .count = 2, .cost = 20.0f, .primary = 1, .mode = 3},
			{.id = 8003, .count = 3, .cost = 30.0f, .primary = 1, .mode = 0},
			{.id = 8004, .count = 4, .cost = 40.0f, .primary = 1, .mode = 1},
			{.id = 8004, .count = 5, .cost = 50.0f, .primary = 1, .mode = 2}
		},
		.in_a_size = 5,
		.in_b = (const StatData[]) {
			{.id = 8001, .count = 6, .cost = 60.0f, .primary = 1, .mode = 5},
			{.id = 8002, .count = 7, .cost = 70.0f, .primary = 1, .mode = 3},
			{.id = 8003, .count = 8, .cost = 80.0f, .primary = 1, .mode = 7},
			{.id = 8004, .count = 9, .cost = 90.0f, .primary = 1, .mode = 4}
		},
		.in_b_size = 4,
		.expected = (const StatData[]) {
			{.id = 8001, .count = 7, .cost = 70.0f, .primary = 1, .mode = 5},
			{.id = 8002, .count = 9, .cost = 90.0f, .primary = 1, .mode = 3},
			{.id = 8003, .count = 11, .cost = 110.0f, .primary = 1, .mode = 7},
			{.id = 8004, .count = 18, .cost = 180.0f, .primary = 1, .mode = 4}
		},
		.expected_size = 4,
		.input_a_path = "build/case_8_in_a.dump",
		.input_b_path = "build/case_8_in_b.dump",
		.output_path   = "build/case_8_out.dump"
	},
	{
		.name = "TC-09: combined merge logic",
		.in_a = (const StatData[]) {
			{.id = 100, .count = 5, .cost = 10.0f, .primary = 1, .mode = 2},
			{.id = 100, .count = 3, .cost = 20.0f, .primary = 0, .mode = 5}
		},
		.in_a_size = 2,
		.in_b = (const StatData[]) {
			{.id = 100, .count = 2, .cost = 30.0f, .primary = 1, .mode = 1}
		},
		.in_b_size = 1,
		.expected = (const StatData[]) {
			{.id = 100, .count = 10, .cost = 60.0f, .primary = 0, .mode = 5}
		},
		.expected_size = 1,
		.input_a_path = "build/case_9_in_a.dump",
		.input_b_path = "build/case_9_in_b.dump",
		.output_path   = "build/case_9_out.dump"
	},
	{
		.name = "TC-10: already sorted by cost",
		.in_a = (const StatData[]) {
			{.id = 1010, .count = 1, .cost = 1.0f, .primary = 1, .mode = 1},
			{.id = 1011, .count = 2, .cost = 2.0f, .primary = 1, .mode = 2}
		},
		.in_a_size = 2,
		.in_b = (const StatData[]) {
			{.id = 1012, .count = 3, .cost = 3.0f, .primary = 0, .mode = 3},
			{.id = 1013, .count = 4, .cost = 4.0f, .primary = 1, .mode = 4},
			{.id = 1014, .count = 5, .cost = 5.0f, .primary = 0, .mode = 5}
		},
		.in_b_size = 3,
		.expected = (const StatData[]) {
			{.id = 1010, .count = 1, .cost = 1.0f, .primary = 1, .mode = 1},
			{.id = 1011, .count = 2, .cost = 2.0f, .primary = 1, .mode = 2},
			{.id = 1012, .count = 3, .cost = 3.0f, .primary = 0, .mode = 3},
			{.id = 1013, .count = 4, .cost = 4.0f, .primary = 1, .mode = 4},
			{.id = 1014, .count = 5, .cost = 5.0f, .primary = 0, .mode = 5}
		},
		.expected_size = 5,
		.input_a_path = "build/case_10_in_a.dump",
		.input_b_path = "build/case_10_in_b.dump",
		.output_path   = "build/case_10_out.dump"
	},
	{
		.name = "TC-11: reverse sorted input",
		.in_a = (const StatData[]) {
			{.id = 1110, .count = 1, .cost = 9.0f, .primary = 1, .mode = 1},
			{.id = 1111, .count = 2, .cost = 7.0f, .primary = 1, .mode = 2},
			{.id = 1112, .count = 3, .cost = 5.0f, .primary = 0, .mode = 3}
		},
		.in_a_size = 3,
		.in_b = (const StatData[]) {
			{.id = 1113, .count = 4, .cost = 3.0f, .primary = 1, .mode = 4},
			{.id = 1114, .count = 5, .cost = 1.0f, .primary = 0, .mode = 5}
		},
		.in_b_size = 2,
		.expected = (const StatData[]) {
			{.id = 1114, .count = 5, .cost = 1.0f, .primary = 0, .mode = 5},
			{.id = 1113, .count = 4, .cost = 3.0f, .primary = 1, .mode = 4},
			{.id = 1112, .count = 3, .cost = 5.0f, .primary = 0, .mode = 3},
			{.id = 1111, .count = 2, .cost = 7.0f, .primary = 1, .mode = 2},
			{.id = 1110, .count = 1, .cost = 9.0f, .primary = 1, .mode = 1}
		},
		.expected_size = 5,
		.input_a_path = "build/case_11_in_a.dump",
		.input_b_path = "build/case_11_in_b.dump",
		.output_path   = "build/case_11_out.dump"
	},
	{
		.name = "TC-12: all cost values are equal",
		.in_a = (const StatData[]) {
			{.id = 1200, .count = 1, .cost = 5.0f, .primary = 1, .mode = 1},
			{.id = 1200, .count = 2, .cost = 5.0f, .primary = 1, .mode = 2},
			{.id = 1200, .count = 3, .cost = 5.0f, .primary = 1, .mode = 3},
			{.id = 1200, .count = 4, .cost = 5.0f, .primary = 1, .mode = 4},
			{.id = 1200, .count = 5, .cost = 5.0f, .primary = 1, .mode = 5}
		},
		.in_a_size = 5,
		.in_b = (const StatData[]) {
			{.id = 1200, .count = 6, .cost = 5.0f, .primary = 1, .mode = 6},
			{.id = 1200, .count = 7, .cost = 5.0f, .primary = 1, .mode = 7},
			{.id = 1200, .count = 8, .cost = 5.0f, .primary = 1, .mode = 1},
			{.id = 1200, .count = 9, .cost = 5.0f, .primary = 1, .mode = 2},
			{.id = 1200, .count = 10, .cost = 5.0f, .primary = 1, .mode = 3}
		},
		.in_b_size = 5,
		.expected = (const StatData[]) {
			{.id = 1200, .count = 55, .cost = 50.0f, .primary = 1, .mode = 7}
		},
		.expected_size = 1,
		.input_a_path = "build/case_12_in_a.dump",
		.input_b_path = "build/case_12_in_b.dump",
		.output_path   = "build/case_12_out.dump"
	},
	{
		.name = "TC-13: negative and zero costs",
		.in_a = (const StatData[]) {
			{.id = 1300, .count = 10, .cost = -5.0f, .primary = 1, .mode = 1},
			{.id = 1301, .count = 20, .cost = 0.0f, .primary = 0, .mode = 2},
			{.id = 1302, .count = 30, .cost = 3.0f, .primary = 1, .mode = 3}
		},
		.in_a_size = 3,
		.in_b = (const StatData[]) {
			{.id = 1303, .count = 40, .cost = -10.0f, .primary = 1, .mode = 4},
			{.id = 1304, .count = 50, .cost = 2.0f, .primary = 0, .mode = 5}
		},
		.in_b_size = 2,
		.expected = (const StatData[]) {
			{.id = 1303, .count = 40, .cost = -10.0f, .primary = 1, .mode = 4},
			{.id = 1300, .count = 10, .cost = -5.0f, .primary = 1, .mode = 1},
			{.id = 1301, .count = 20, .cost = 0.0f, .primary = 0, .mode = 2},
			{.id = 1304, .count = 50, .cost = 2.0f, .primary = 0, .mode = 5},
			{.id = 1302, .count = 30, .cost = 3.0f, .primary = 1, .mode = 3}
		},
		.expected_size = 5,
		.input_a_path = "build/case_13_in_a.dump",
		.input_b_path = "build/case_13_in_b.dump",
		.output_path   = "build/case_13_out.dump"
	},
};

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <path_to_statforge>\n", argv[0]);
		return 1;
	}

	printf("========================================\n");
    printf("  Testforge testing utility\n");
    printf("========================================\n\n");

	const char *statforge_path = argv[1];

	struct timespec started_at;
	struct timespec ended_at;

	if (clock_gettime(CLOCK_MONOTONIC, &started_at) != 0) {
		perror("testforge: start clock_gettime failed");
		return 1;
	}

	size_t passed = 0;
	size_t total = sizeof(tests) / sizeof(tests[0]);

	for (size_t i = 0; i < total; ++i) {
		if (runTestCase(statforge_path, &tests[i])) {
			printf("%s: PASSED\n", tests[i].name);
			passed++;
		} else {
			printf("%s: FAILED\n", tests[i].name);
		}
	}

	if (clock_gettime(CLOCK_MONOTONIC, &ended_at) != 0) {
		perror("testforge: end clock_gettime failed");
		return 1;
	}

	double spent_time = (double)(ended_at.tv_sec - started_at.tv_sec) +
						 (double)(ended_at.tv_nsec - started_at.tv_nsec) / 1e9;

	if (passed != total) {
		fprintf(stderr, "testforge: %zu/%zu tests passed\n", passed, total);
		return 1;
	}

	printf("========================================\n");
	printf("  Test report:\n");
	printf(" - Summary: %zu/%zu\n", passed, total);
	printf(" - Execution time: %.6f sec\n", spent_time);
	printf("\n\n");

	return 0;
}

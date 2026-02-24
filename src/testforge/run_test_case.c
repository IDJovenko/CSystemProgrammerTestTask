#include "testforge/run_test_case.h"
#include "testforge/is_equal_statdata.h"
#include "stat_data.h"
#include "serializer/load_dump.h"
#include "serializer/store_dump.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define EXEC_ERROR 127
static int runStatforge(const char* statforge_path,
						 const char* input_a,
						 const char* input_b,
						 const char* output_file)
{
	pid_t pid = fork();
	if (pid == -1) {
		perror("testforge: fork failed");
		return -1;
	}

	if (pid == 0) {
    	// Перенаправляем stdout в /dev/null, чтобы подавить вывод statforge
    	if (freopen("/dev/null", "w", stdout) == NULL) {
    	    perror("testforge: freopen failed");
    	}

		char* const args[] = {
			(char*)statforge_path,
			(char*)input_a,
			(char*)input_b,
			(char*)output_file,
			NULL
		};
		execv(statforge_path, args);
		perror("testforge: execv failed");
		exit(EXEC_ERROR);
	}

	int status = 0;
	if (waitpid(pid, &status, 0) == -1) {
		perror("testforge: waitpid failed");
		return -1;
	}

	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		fprintf(stderr, "testforge: statforge exited with status %d\n",
				WIFEXITED(status) ? WEXITSTATUS(status) : -1);
		return -1;
	}

	return 0;
}

int runTestCase(const char *statforge_path, const TestCase *tc)
{
	/* write input A */
	if (StoreDump(tc->in_a, tc->in_a_size, tc->input_a_path) != (ssize_t)tc->in_a_size) {
		fprintf(stderr,
		        "testforge: %s failed to write %s\n",
		        tc->name,
		        tc->input_a_path);
		return 0;
	}

	/* write input B */
	if (StoreDump(tc->in_b, tc->in_b_size, tc->input_b_path) != (ssize_t)tc->in_b_size) {
		fprintf(stderr,
		        "testforge: %s failed to write %s\n",
		        tc->name,
		        tc->input_b_path);
		return 0;
	}

	/* run statforge */
	if (runStatforge(statforge_path,
	                  tc->input_a_path,
	                  tc->input_b_path,
	                  tc->output_path) != 0)
	{
		fprintf(stderr,
		        "testforge: %s failed to run statforge\n",
		        tc->name);
		return 0;
	}

	/* load result */
	StatData *actual = NULL;
	ssize_t actual_size = LoadDump(&actual, tc->output_path);

	if (actual_size != (ssize_t)tc->expected_size) {
		fprintf(stderr,
		        "testforge: %s expected %zu records, got %zd\n",
		        tc->name,
		        tc->expected_size,
		        actual_size);
		free(actual);
		return 0;
	}

	/* compare records */
	for (size_t i = 0; i < tc->expected_size; ++i) {

		if (!isEqualStatData(&actual[i], &tc->expected[i])) {

			fprintf(stderr,
			        "testforge: %s mismatch at index %zu\n"
			        "  actual:   id=%ld count=%d cost=%.6f primary=%u mode=%u\n"
			        "  expected: id=%ld count=%d cost=%.6f primary=%u mode=%u\n",
			        tc->name,
			        i,
			        actual[i].id,
			        actual[i].count,
			        actual[i].cost,
			        actual[i].primary,
			        actual[i].mode,
			        tc->expected[i].id,
			        tc->expected[i].count,
			        tc->expected[i].cost,
			        tc->expected[i].primary,
			        tc->expected[i].mode);

			free(actual);
			return 0;
		}
	}

	free(actual);
	return 1;
}

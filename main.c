#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <err.h>
#include <dlfcn.h>

#include "vector.h"
#include "problem.h"

#define NUM_PROBLEMS 25

struct box_set_node {
	unsigned int y_pos, y_len;
	struct interval_set *row_data;
};

struct box_set {
	struct box_set_node *root;
};

typedef void (*problem_fn)(FILE *, struct solution *);

void print_usage() {
	fprintf(stderr, "usage: aoc <problem #> <input file>\n");
}

void scan_symbols(problem_fn *solvers, size_t num_solvers) {
	void *dlhandle = dlopen(NULL, RTLD_NOW);
	if (!dlhandle) {
		return;
	}
	for (size_t i = 0; i < num_solvers; ++i) {
		char fun_name[32] = {0};
		problem_fn solver;
		sprintf(fun_name, "solve_problem%lu", i+1);
		if ((solver = (problem_fn) dlsym(dlhandle, fun_name)) != NULL) {
			solvers[i] = solver;
		}
	}
}

/// ENTRY POINT
int main(int argc, char *argv[]) {
	/* the problem input file */
	FILE *input_file;
	/* the selected problem */
	size_t problem_id;
	/* storage for the solution */
	struct solution my_solution = { .answered = 0 };
	/* array of pointers to solver functions */
	problem_fn solvers[NUM_PROBLEMS] = {0};

	/* dynamically load solvers */
	scan_symbols(solvers, NUM_PROBLEMS);

	if (argc != 3) {
		print_usage();
		exit(EX_USAGE);
	}
	if (sscanf(argv[1], "%zd", &problem_id) != 1 || problem_id < 1 || problem_id > NUM_PROBLEMS) {
		errx(EX_USAGE, "invalid id: %s", argv[1]);
	}
	if (solvers[problem_id-1] == NULL) {
		errx(EX_USAGE, "problem %zd not yet solved", problem_id);
	}
	if ((input_file = fopen(argv[2], "r")) == NULL) {
		errx(EX_NOINPUT, "unable to open file '%s'", argv[2]);
	}

	solvers[problem_id-1](input_file, &my_solution);
	print_solution(&my_solution);
	return 0;
}


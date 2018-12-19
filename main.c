#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <err.h>

#include "vector.h"

struct box_set_node {
	unsigned int y_pos, y_len;
	struct interval_set *row_data;
};

struct box_set {
	struct box_set_node *root;
};

typedef void (*problem_fn)(FILE *);
#define NUM_PROBLEMS 4

extern void solve_problem3(FILE *);
extern void solve_problem4(FILE *);

void print_usage() {
	fprintf(stderr, "usage: aoc <problem #> <input file>\n");
}

/// ENTRY POINT
int main(int argc, char *argv[]) {
	FILE *input_file;
	problem_fn solutions[NUM_PROBLEMS] = {0};
	solutions[2] = solve_problem3;
	solutions[3] = solve_problem4;
	size_t problem_id;
	if (argc != 3) {
		print_usage();
		exit(EX_USAGE);
	}
	if ((input_file = fopen(argv[2], "r")) == NULL) {
		err(EX_NOINPUT, "aoc: %s", argv[2]);
	}

	if (sscanf(argv[1], "%zd", &problem_id) != 1 || problem_id < 1 || problem_id > NUM_PROBLEMS) {
		err(EX_USAGE, "aoc: invalid id: %s", argv[1]);
	}
	if (solutions[problem_id-1] == NULL) {
		err(EX_USAGE, "aoc: problem %zd not yet solved\n", problem_id);
	}
	solutions[problem_id-1](input_file);
	return 0;
}


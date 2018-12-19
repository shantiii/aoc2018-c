#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <err.h>

#include "interval_set.h"
#include "vector.h"

struct box_set_node {
	unsigned int y_pos, y_len;
	struct interval_set *row_data;
};

struct box_set {
	struct box_set_node *root;
};

/* interval comparison states */
enum {
	/*   { lhs }
	 * [   rhs   ]
	 */
	IC_IN = 0, /* lhs is contained within rhs */
	/*   { lhs } [ rhs ] */
	IC_LESS = 1, /* lhs is strictly less than rhs */
	/*   [ rhs ] { lhs } */
	IC_MORE = 2, /* lhs is strictly greater than rhs */
	IC_OUTSIDE = 3, /* lhs surrounds rhs */
	IC_IN_LESS = 5, /* lhs partially overlaps rhs from below */
	IC_IN_MORE = 6, /* lhs partially overlaps rhs from above */
};

struct interval {
	unsigned int lower, upper;
};

unsigned int int_cmp(struct interval *lhs, struct interval *rhs) {
	if (lhs->upper < rhs->lower) {
		return IC_LESS;
	} else if (lhs->lower > rhs->upper) {
		return IC_MORE;
	}
	return 1; // TODO: finish this function?
}

typedef void (*problem_fn)(FILE *);
#define NUM_PROBLEMS 4

void solve_problem3(FILE *);
void solve_problem4(FILE *);

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


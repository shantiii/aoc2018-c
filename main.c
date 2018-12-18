#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <err.h>

#include "interval_set.h"
#include "vector.h"

struct box {
	unsigned int id;
	unsigned int x_pos, x_len;
	unsigned int y_pos, y_len;
};

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

void print_usage() {
	fprintf(stderr, "usage: aoc <input file>\n");
}

#define DUMB_LOC(x,y) (((y) << 10) + x)

int check_box(struct box *b, short *data) {
	for (int i = 0; i < b->x_len; ++i) {
		for (int j = 0; j < b->y_len; ++j) {
			if (data[DUMB_LOC(b->x_pos + i, b->y_pos + j)] != 1)
				return 0;
		}
	}
	return 1;
}

/// ENTRY POINT
int main(int argc, char *argv[]) {
	FILE *input_file;
	if (argc != 2) {
		print_usage();
		exit(EX_USAGE);
	}
	if ((input_file = fopen(argv[1], "r")) == NULL) {
		err(EX_NOINPUT, "aoc: %s", argv[1]);
	}
	struct vector boxes;
	struct box tmp_box;
	vector_init(&boxes, sizeof(struct box), 10);
	while (fscanf(input_file, "#%d @ %d,%d: %dx%d\n", &tmp_box.id, &tmp_box.x_pos, &tmp_box.y_pos, &tmp_box.x_len, &tmp_box.y_len) == 5) {
		//fprintf("Scanned box %d: [%d,%d) -> [%d,%d)\n", tmp_box.id, tmp_box.y_pos, tmp_box.y_len, tmp_box.x_pos, tmp_box.x_len);
		vector_push(&boxes, &tmp_box);
	}
	short data[1024*1024] = {0};
	VECTOR_FOREACH(struct box *, b, boxes) {
		//fprintf(stderr, "box: %d\n", b->id);
		for (int i = 0; i < b->x_len; ++i) {
			for (int j = 0; j < b->y_len; ++j) {
				data[DUMB_LOC(b->x_pos + i, b->y_pos + j)]++;
			}
		}
	}
	int total = 0;
	for(int i = 0; i<1024 * 1024; ++i) {
		total += (data[i] > 1);
	}

	printf("part1: %d\n", total);
	VECTOR_FOREACH(struct box *, b, boxes) {
		if (check_box(b, data)) {
			printf("part2: %d\n", b->id);
		}
	}
	// let seenIntervals = empty set of boxes
	// let occlusions = empty set of boxes
	// for each box in inputIntervals {
	//   let intersectingIntervals = intersections from seenIntervals
	//   let intersection = actual range intersections
	//   update occlusions by inserting intersections
	//   update seenIntervals by inserting box
	// }
	// for each box in occlusions {
	//   union into unionedOcclusions
	// }
	// for each item in unionedOcclusions {
	//   sum area
	// }
	return 0;
}


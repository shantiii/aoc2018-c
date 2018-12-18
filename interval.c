#include <stdlib.h>
#include <sysexits.h>
#include "interval_set.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

/* interval set node comparison states */
enum {
	ISNC_IN = 0, /* lhs is contained within rhs */
	ISNC_LESS = 1, /* lhs is strictly less than rhs */
	ISNC_MORE = 2, /* lhs is strictly greater than rhs */
	ISNC_OUTSIDE = 3, /* lhs surrounds rhs */
	ISNC_IN_LESS = 4, /* lhs partially overlaps rhs from below */
	ISNC_IN_MORE = 4, /* lhs partially overlaps rhs from above */
};

/* allocates and instantiates a node */
struct interval_node *node_create(unsigned int lower, unsigned int upper);
/* frees a node recursively */
void node_destroy(struct interval_node *node);
/* merges two overlapping nodes */
void node_merge(struct interval_node *node1, struct interval_node *node2);
/* returns 1 if the two nodes are adjoining, 0 otherwise */
int node_is_adjoining(struct interval_node *, struct interval_node *);

/// node function definitions

struct interval_node *node_create(unsigned int lower, unsigned int upper) {
	struct interval_node *ret = (struct interval_node *) malloc(sizeof(struct interval_node));
	ret->lower = lower;
	ret->upper = upper;
	ret->max_upper = upper;
	ret->left = ret->right = NULL;
	return ret;
}

void node_destroy(struct interval_node *node) {
	if (node) {
		node_destroy(node->left);
		node_destroy(node->right);
		free(node);
	}
}

/// interval_set function definitions

struct interval_set *is_create() {
	struct interval_set *ret = (struct interval_set *) malloc(sizeof(struct interval_set));
	ret->root = NULL;
	return ret;
}

void is_destroy(struct interval_set *set) {
	if (set) {
		node_destroy(set->root);
		free(set);
	}
}


void is_insert(struct interval_set *set, unsigned int lower, unsigned int upper) {
	if (set->root) {

	} else {
		set->root = node_create(lower, upper);
	}
}

struct interval_node_list *is_intersect(struct interval_set *, unsigned int lower, unsigned int upper);
void is_reduce(struct interval_set *, is_reducer, void *init_state);

/// interval_node_list function definitions

struct interval_node_list *inl_prepend(struct interval_node_list *in, struct interval_node *node) {
	struct interval_node_list *new = (struct interval_node_list *) malloc(sizeof(struct interval_node_list));
	if (new) {
		new->node = node;
		new->next = in;
		return new;
	} else {
		exit(EX_OSERR);
	}
}

void inl_destroy(struct interval_node_list *list) {
	struct interval_node_list *next;
	while (list) {
		next = list->next;
		free(list);
		list = next;
	}
}

#ifndef INTERVAL_SET_H
#define INTERVAL_SET_H
#pragma once

/* on-add, merge set */
struct interval_set {
	struct interval_node *root;
};

struct interval_node {
	unsigned int lower, upper;
	unsigned int max_upper;
	struct interval_node *left, *right;
};

struct interval_node_list {
	struct interval_node *node;
	struct interval_node_list *next;
};

typedef void (*is_reducer)(struct interval_node *, void *state);

/// interval_node_list function_declarations

struct interval_node_list *inl_prepend(struct interval_node_list *, struct interval_node *data);
void inl_destroy(struct interval_node_list *);

/// interval_set function declarations

struct interval_set *is_create();
void is_destroy(struct interval_set *);
void is_insert(struct interval_set *, unsigned int lower, unsigned int upper);
struct interval_node_list *is_intersect(struct interval_set *, unsigned int lower, unsigned int upper);
void is_reduce(struct interval_set *, is_reducer, void *init_state);

#endif

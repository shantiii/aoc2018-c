#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sysexits.h>
#include <sys/rbtree.h>

#include "vector.h"

enum event_type {
	EV_SLEEP,
	EV_WAKE,
	EV_GUARD_ID
};

struct event {
	int month, day, hour, minute;
	enum event_type type;
	int guard_id;
};

struct guard_entry {
	rb_node_t rbt_node; /* used for rbtree functions */
	int guard_id;
	unsigned int sleeps[60];
	unsigned long long int total_sleep;
};

/* guard entry compa for qsort */
int guard_entry_cmp(void *, const struct guard_entry *, const struct guard_entry *);
int guard_entry_key_cmp(void *, const struct guard_entry *, const int *);
/* event comparison for qsort */
int event_cmp(const struct event *, const struct event *);


rb_tree_ops_t guard_tree_ops = {
	.rbto_compare_nodes = (rbto_compare_nodes_fn) guard_entry_cmp,
	.rbto_compare_key = (rbto_compare_key_fn)guard_entry_key_cmp,
	.rbto_node_offset = 0,
	.rbto_context = 0,
};

void parse_events(FILE *input, struct vector *events);
struct guard_entry *lookup(rb_tree_t *tree, int guard_id);

void solve_problem4(FILE *input) {
	struct vector events;
	vector_init(&events, sizeof(struct event), 10);
	parse_events(input, &events);
	fclose(input);

	(void) qsort(events.data, events.size, events.width, (int (*)(const void *, const void *))event_cmp);
	struct guard_entry *current_guard = NULL;
	int current_day = 0;
	int tick = 0;
	enum event_type current_state = EV_WAKE;
	rb_tree_t guards;
	rb_tree_init(&guards, &guard_tree_ops);
	VECTOR_FOREACH(struct event *, event, events) {
		if (event->type == EV_GUARD_ID) {
			current_guard = lookup(&guards, event->guard_id);
			continue;
		} else {
			event->guard_id = current_guard->guard_id;
		}
		current_guard->total_sleep++;
		int today = event->month * 32 + event->day;
		if (current_day != today) {
			current_day = today;
			tick = 0;

		}
		printf("[%d-%d %d:%d][%d] %d\n", event->month, event->day, event->hour, event->minute, event->guard_id, (int) event->type);
		printf("[Guard %d]: %llu\n", current_guard->guard_id, current_guard->total_sleep);
	}
	// cleanup
	struct guard_entry *entry;
	while ((entry = RB_TREE_MIN(&guards)) != NULL) {
		rb_tree_remove_node(&guards, entry);
		free(entry);
	}
	vector_destroy(&events);
	fflush(stdin);
}

void parse_events(FILE *input, struct vector *events) {
	char date_text[12] = {0};
	char event_text[100] = {0};
	struct event tmp_event;
	while (fscanf(input, "[1518-%[^]]] %[^\n]\n", date_text, event_text) == 2) {
		tmp_event.guard_id = -1;
		if (sscanf(date_text, "%d-%d %d:%d", &tmp_event.month, &tmp_event.day, &tmp_event.hour, &tmp_event.minute) != 4) {
			exit(EX_DATAERR);
		}
		if (strcmp(event_text, "falls asleep") == 0) {
			tmp_event.type = EV_SLEEP;
		} else if (strcmp(event_text, "wakes up") == 0) {
			tmp_event.type = EV_WAKE;
		} else {
			tmp_event.type = EV_GUARD_ID;
			sscanf(event_text, "Guard #%d begins shift", &tmp_event.guard_id);
		}
		vector_push(events, &tmp_event);
	};
}

int event_cmp(const struct event *a, const struct event *b) {
	int diff[4] = {a->month - b->month, a->day - b->day, a->hour - b->hour, a->minute - b->minute};
	for (int i=0; i<4; ++i) {
		if (diff[i] != 0) {
			return diff[i];
		}
	}
	return 0;
}

struct guard_entry *lookup(rb_tree_t *tree, int guard_id) {
	struct guard_entry *ret = rb_tree_find_node(tree, &guard_id);
	if (ret) {
		return ret;
	} else {
		ret = (struct guard_entry *) calloc(1, sizeof(struct guard_entry));
		ret->guard_id = guard_id;
		if (!ret) {
			exit(EX_OSERR);
		}
		return rb_tree_insert_node(tree, ret);
	}
}

int guard_entry_cmp(void *ctx, const struct guard_entry *lhs, const struct guard_entry *rhs) {
	return guard_entry_key_cmp(ctx, lhs, &rhs->guard_id);
}
int guard_entry_key_cmp(void *ctx, const struct guard_entry *node, const int *key) {
	(void) ctx;
	return node->guard_id - *key;
}

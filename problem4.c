#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sysexits.h>

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

int event_cmp(const struct event *, const struct event *);

void solve_problem4(FILE *input) {
	char date_text[10] = {0};
	char event_text[100] = {0};
	struct event tmp_event;
	struct vector events;
	vector_init(&events, sizeof(struct event), 10);
	while (fscanf(input, "[1518-%[^]]] %[^\n]\n", date_text, event_text) == 2) {
		tmp_event.guard_id = -1;
		//printf("date: %s\n", date_text);
		//printf("foo: %s\n", event_text);
		if (sscanf(date_text, "%d-%d %d:%d", &tmp_event.month, &tmp_event.day, &tmp_event.hour, &tmp_event.minute) != 4) {
			return exit(EX_DATAERR);
		}
		if (strcmp(event_text, "falls asleep") == 0) {
			tmp_event.type = EV_SLEEP;
		} else if (strcmp(event_text, "wakes up") == 0) {
			tmp_event.type = EV_WAKE;
		} else {
			tmp_event.type = EV_GUARD_ID;
			sscanf(event_text, "Guard #%d begins shift", &tmp_event.guard_id);
		}
		vector_push(&events, &tmp_event);
	};
	fclose(input);
	(void) qsort(events.data, events.size, events.width, (int (*)(const void *, const void *))event_cmp);
	int current_guard_id = 0;
	VECTOR_FOREACH(struct event *, event, events) {
		if (event->type == EV_GUARD_ID) {
			current_guard_id = event->guard_id;
		} else {
			event->guard_id = current_guard_id;
		}
		printf("[%d-%d %d:%d][%d] %d\n", event->month, event->day, event->hour, event->minute, event->guard_id, (int) event->type);
	}
	vector_destroy(&events);
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

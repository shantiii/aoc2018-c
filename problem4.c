#ifndef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sysexits.h>
#include <sys/rbtree.h>

#include "problem.h"
#include "vector.h"

enum event_type
{
  EV_WAKE,
  EV_SLEEP,
  EV_GUARD_ID
};

struct event
{
  int month, day, hour, minute;
  enum event_type type;
  int guard_id;
};

struct guard_entry
{
  rb_node_t rbt_node;		/* used for rbtree functions */
  int guard_id;
  unsigned int sleeps[60];
  unsigned long long int total_sleep;
};

/* guard entry compa for qsort */
int guard_entry_cmp (void *, const struct guard_entry *,
		     const struct guard_entry *);
int guard_entry_key_cmp (void *, const struct guard_entry *, const int *);
/* event comparison for qsort */
int event_cmp (const struct event *, const struct event *);

rb_tree_ops_t guard_tree_ops = {
  .rbto_compare_nodes = (rbto_compare_nodes_fn) guard_entry_cmp,
  .rbto_compare_key = (rbto_compare_key_fn) guard_entry_key_cmp,
  .rbto_node_offset = 0,
  .rbto_context = 0,
};

void parse_events (FILE * input, struct vector *events);
struct guard_entry *lookup (rb_tree_t * tree, int guard_id);

void
finish_day (struct guard_entry *entry, enum event_type state, int second)
{
  if (state == EV_WAKE)
    {
      return;
    }
  while (second < 60)
    {
      entry->sleeps[second]++;
      entry->total_sleep++;
      ++second;
    }
}

void
solve_problem4 (FILE * input, struct solution *sol)
{
  struct vector events;
  vector_init (&events, sizeof (struct event), 10);
  parse_events (input, &events);
  fclose (input);

  (void) qsort (events.data, events.size, events.width,
		(int (*)(const void *, const void *)) event_cmp);
  struct guard_entry *current_guard = NULL;
  int current_day = 0;
  int tick = 0;
  enum event_type current_state = EV_WAKE;
  rb_tree_t guards;
  rb_tree_init (&guards, &guard_tree_ops);
  VECTOR_FOREACH (struct event *, event, events)
  {
    if (event->type == EV_GUARD_ID)
      {
	current_guard = lookup (&guards, event->guard_id);
	continue;
      }
    else
      {
	event->guard_id = current_guard->guard_id;
      }
    current_guard->total_sleep++;
    int today = event->month * 32 + event->day;
    if (current_day != today)
      {
	finish_day (current_guard, current_state, tick);
	tick = 0;
	current_day = today;
	current_state = EV_WAKE;
      }
    int add_val = (current_state == EV_SLEEP) ? 1 : 0;
    while (tick < event->minute)
      {
	current_guard->sleeps[tick] += add_val;
	current_guard->total_sleep += add_val;
	tick++;
      }
    current_state = event->type;
  }
  struct guard_entry *entry = NULL, *max_entry = NULL, *p2_entry = NULL;
  int max_id = 0, max_second = 0;
  int p2_id = 0, p2_second = 0;
  while ((entry = rb_tree_iterate (&guards, entry, RB_DIR_RIGHT)) != NULL)
    {
      if (max_entry == NULL || max_entry->total_sleep < entry->total_sleep)
	{
	  max_entry = entry;
	  max_id = max_entry->guard_id;
	}
      for (int i = 0; i < 60; ++i)
	{
	  if (p2_entry == NULL
	      || entry->sleeps[i] > p2_entry->sleeps[p2_second])
	    {
	      p2_entry = entry;
	      p2_id = entry->guard_id;
	      p2_second = i;
	    }
	}
    }
  for (int i = 0; i < 60; ++i)
    {
      if (max_entry->sleeps[i] > max_entry->sleeps[max_second])
	{
	  max_second = i;
	}
    }
  // cleanup
  while ((entry = RB_TREE_MIN (&guards)) != NULL)
    {
      rb_tree_remove_node (&guards, entry);
      free (entry);
    }
  vector_destroy (&events);
  answerf (sol, "%d", max_second * max_id);
  answerf (sol, "%d", p2_second * p2_id);
}

void
parse_events (FILE * input, struct vector *events)
{
  char date_text[12] = { 0 };
  char event_text[100] = { 0 };
  struct event tmp_event;
  while (fscanf (input, "[1518-%[^]]] %[^\n]\n", date_text, event_text) == 2)
    {
      tmp_event.guard_id = -1;
      if (sscanf
	  (date_text, "%d-%d %d:%d", &tmp_event.month, &tmp_event.day,
	   &tmp_event.hour, &tmp_event.minute) != 4)
	{
	  exit (EX_DATAERR);
	}
      if (strcmp (event_text, "falls asleep") == 0)
	{
	  tmp_event.type = EV_SLEEP;
	}
      else if (strcmp (event_text, "wakes up") == 0)
	{
	  tmp_event.type = EV_WAKE;
	}
      else
	{
	  tmp_event.type = EV_GUARD_ID;
	  sscanf (event_text, "Guard #%d begins shift", &tmp_event.guard_id);
	}
      vector_push (events, &tmp_event);
    };
}

int
event_cmp (const struct event *a, const struct event *b)
{
  int diff[4] = { a->month - b->month, a->day - b->day, a->hour - b->hour,
    a->minute - b->minute
  };
  for (int i = 0; i < 4; ++i)
    {
      if (diff[i] != 0)
	{
	  return diff[i];
	}
    }
  return 0;
}

struct guard_entry *
lookup (rb_tree_t * tree, int guard_id)
{
  struct guard_entry *ret = rb_tree_find_node (tree, &guard_id);
  if (ret)
    {
      return ret;
    }
  else
    {
      ret = (struct guard_entry *) calloc (1, sizeof (struct guard_entry));
      ret->guard_id = guard_id;
      if (!ret)
	{
	  exit (EX_OSERR);
	}
      return rb_tree_insert_node (tree, ret);
    }
}

int
guard_entry_cmp (void *ctx, const struct guard_entry *lhs,
		 const struct guard_entry *rhs)
{
  return guard_entry_key_cmp (ctx, lhs, &rhs->guard_id);
}

int
guard_entry_key_cmp (void *ctx, const struct guard_entry *node,
		     const int *key)
{
  (void) ctx;
  return node->guard_id - *key;
}
#else

#include <stdio.h>

#include "problem.h"

void
solve_problem4 (FILE * param, struct solution *sol)
{
  (void) param;
  (void) sol;
}

#endif

#include <stdio.h>

#include "problem.h"
#include "vector.h"
#include "heap.h"

int
charcmp (const char *lhs, const char *rhs)
{
  return *lhs - *rhs;
}

struct worker_entry
{
  char current;			/* current character being processed */
  int time_remaining;
};

int
early_worker (const struct worker_entry *lhs, const struct worker_entry *rhs)
{
  return lhs->time_remaining - rhs->time_remaining;
}

// problem code
void
solve_problem7 (FILE * input, struct solution *sol)
{
  /* data parse memory */
  char before, after;
  /* list of letters with no dependencies */
  struct heap chars;
  /* the count of how many dependencies each letter has, keyed on letter-A */
  int depcounts[26] = { 0 };
  /* the list of how many other letter depend on this one, keyed on letter-A */
  struct vector deplist[26];
  for (int i = 0; i < 26; ++i)
    {
      vector_init (&deplist[i], sizeof (char), 2);
    }
  heap_init (&chars, sizeof (char), (heap_cmp_fn) charcmp);
  while (fscanf
	 (input, "Step %c must be finished before step %c can begin.\n",
	  &before, &after) == 2)
    {
      //printf("%c -> %c\n", before, after);
      depcounts[after - 'A']++;
      vector_push (&deplist[before - 'A'], &after);
    }
  for (char i = 'A'; i <= 'Z'; ++i)
    {
      if (depcounts[(size_t) (i - 'A')] == 0)
	{
	  heap_insert (&chars, &i);
	}
    }
  size_t loc = 0;
  while (heap_size (&chars) > 0)
    {
      char out;
      heap_pop (&chars, &out);
      sol->answers[0][loc] = out;
      loc++;
      VECTOR_FOREACH (char *, dep, deplist[(size_t) (out - 'A')])
      {
	depcounts[*dep - 'A']--;
	if (depcounts[*dep - 'A'] == 0)
	  {
	    heap_insert (&chars, dep);
	  }
      }
    }
  sol->answered++;
  /// Part 2 ///
  struct heap workers;
  (void) workers;
//  while(heap_size(&chars > 0) && heap_size(&workers) > 0) {
//    while (free_char && free_workers) {
//      assign char to worker
//    }
//    advance time to first worker complete
//      // run the update for that entry
//  }

  /* cleanup */
  for (int i = 0; i < 26; ++i)
    {
      vector_destroy (&deplist[i]);
    }
  heap_destroy (&chars);
}

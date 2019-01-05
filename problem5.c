#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "problem.h"
#include "vector.h"

int
compatible (int cp1, int cp2)
{
  return abs (cp1 - cp2) == abs ('a' - 'A');
}

void
solve_problem5 (FILE * input, struct solution *sol)
{
  struct vector stack;
  int read;
  vector_init (&stack, sizeof (int), 1024);
  while ((read = fgetc (input)) != EOF && isalpha (read))
    {
      vector_push (&stack, &read);
      while (stack.size > 1
	     && compatible (((int *) stack.data)[stack.size - 1],
			    ((int *) stack.data)[stack.size - 2]) == 1)
	{
	  vector_pop (&stack, NULL);
	  vector_pop (&stack, NULL);
	}
    }
  answerf (sol, "%lu", stack.size);
  struct vector p2_stack;
  vector_init (&p2_stack, sizeof (char), stack.size);
  char dst;
  size_t min_score = stack.size;
  for (char taboo = 'a'; taboo <= 'z'; ++taboo)
    {
      p2_stack.size = 0;	/* hack to clear the stack */
      for (size_t i = 0; i < stack.size; ++i)
	{
	  dst = (char) (((int *) stack.data)[i]);
	  if (dst == taboo || dst - 'A' + 'a' == taboo)
	    {
	      continue;
	    }
	  vector_push (&p2_stack, &dst);
	  while (p2_stack.size > 1
		 &&
		 compatible (((char *) p2_stack.data)[p2_stack.size - 1],
			     ((char *) p2_stack.data)[p2_stack.size - 2]) ==
		 1)
	    {
	      vector_pop (&p2_stack, NULL);
	      vector_pop (&p2_stack, NULL);
	    }
	}
      min_score = (p2_stack.size < min_score) ? p2_stack.size : min_score;
      p2_stack.size = 0;	/* hack to clear the stack */
    }
  answerf (sol, "%lu", min_score);
  vector_destroy (&p2_stack);
  vector_destroy (&stack);
}

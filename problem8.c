#include <stdio.h>

#include "vector.h"

struct node
{
  struct vector children;
  struct vector meta;
};

size_t
parse_de_boy (struct node *node, int input[], size_t offset)
{
  size_t nchildren = input[offset], nmeta = input[offset + 1];
  vector_init (&node->children, sizeof (struct node), nchildren);
  size_t curr_offset = offset + 2;
  for (size_t i = 0; i < nchildren; ++i)
    {
      struct node boy;
      curr_offset = parse_de_boy (&boy, input, curr_offset);
      vector_push (&node->children, &boy);
    }
  vector_init (&node->meta, sizeof (int), nmeta);
  for (size_t i = 0; i < nmeta; ++i)
    {
      vector_push (&node->meta, &input[curr_offset]);
      curr_offset++;
    }
  return curr_offset;
}

typedef void (*traverse_fn) (struct node *, void *);

void
traverse (struct node *n, void *ctx, traverse_fn fun)
{
  fun (n, ctx);
  struct node *children = (struct node *) (n->children.data);
  for (size_t i = 0; i < vector_size (&n->children); ++i)
    {
      traverse (children + i, ctx, fun);
    }
}

void
sum_meta (struct node *x, int *acc)
{
  int tmp;
  for (size_t i = 0; i < vector_size (&x->meta); ++i)
    {
      vector_at (&x->meta, i, &tmp);
      *acc += tmp;
    }
}

int
value (struct node *x)
{
  size_t nkids = vector_size (&x->children);
  if (nkids == 0)
    {
      int total_meta = 0;
      VECTOR_FOREACH (int *, meta, x->meta)
      {
	total_meta += *meta;
      }
      return total_meta;
    }
  else
    {
      int total_value = 0;
      int values[nkids];
      for (size_t i = 0; i < nkids; ++i)
	{
	  values[i] = value (((struct node *) x->children.data) + i);
	}
      VECTOR_FOREACH (int *, meta, x->meta)
      {
	if (*meta > 0 && *meta <= (int) nkids)
	  {
	    total_value += values[*meta - 1];
	  }
      }
      return total_value;
    }
}

void
solve_problem8 (FILE * file)
{
  struct node root;
  struct vector input;
  vector_init (&input, sizeof (int), 2);
  int parse;
  while (fscanf (file, "%d", &parse) == 1)
    {
      vector_push (&input, &parse);
    }
  parse_de_boy (&root, input.data, 0);
  int meta_total = 0;
  traverse (&root, &meta_total, (traverse_fn) sum_meta);
  printf ("part1: %d\n", meta_total);
  printf ("part2: %d\n", value (&root));
  //TODO: manage memory better
}

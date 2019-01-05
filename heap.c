#include <stddef.h>
#include "vector.h"
#include "heap.h"

// heap impl

#define START_SIZE 4
#define PARENT(x) (((x)-1)>>1)
#define LEFT(x) (((x)<<1)+1)
#define RIGHT(x) (((x)+1)<<1)

void
swap (void *a, void *b, size_t bytes)
{
  char *ac = (char *) a, *bc = (char *) b;
  while (bytes--)
    {
      *ac ^= *bc;
      *bc ^= *ac;
      *ac ^= *bc;
      ac++;
      bc++;
    }
}

void
heap_init (struct heap *h, size_t width, heap_cmp_fn compare)
{
  vector_init (&h->storage, width, START_SIZE);
  h->cmp_fn = compare;
}

void
heap_destroy (struct heap *h)
{
  vector_destroy (&h->storage);
}

void
heap_insert (struct heap *h, const void *elem)
{
  vector_push (&h->storage, elem);
  size_t pos = vector_size (&h->storage) - 1;
  size_t width = vector_width (&h->storage);
  char *data = ((char *) (h->storage.data));
  while (pos > 0
	 && h->cmp_fn (data + width * pos, data + width * PARENT (pos)) < 0)
    {
      swap (data + width * pos, data + width * PARENT (pos), width);
      pos = PARENT (pos);
    }
}

void
heap_peek (struct heap *h, void *out)
{
  vector_at (&h->storage, 0, out);
}

void
heap_pop (struct heap *h, void *out)
{
  size_t width = vector_width (&h->storage);
  if (vector_size (&h->storage) > 1)
    {
      swap (h->storage.data,
	    ((char *) (h->storage.data)) + (vector_size (&h->storage) -
					    1) * width, width);
    }
  vector_pop (&h->storage, out);
  const size_t size = vector_size (&h->storage);
  char *const data = (char *) (h->storage.data);
  size_t pos = 0, smaller;
  /* find smaller child */
  while (1)
    {
      if (RIGHT (pos) < size)
	{
	  if (h->cmp_fn (data + LEFT (pos) * width,
			 data + RIGHT (pos) * width) < 0)
	    {
	      smaller = LEFT (pos);
	    }
	  else
	    {
	      smaller = RIGHT (pos);
	    }
	}
      else if (LEFT (pos) < size)
	{
	  smaller = LEFT (pos);
	}
      else
	{
	  return;
	}
      if (smaller < size
	  && h->cmp_fn (data + pos * width, data + smaller * width) < 0)
	{
	  /* we no longer violate the heap property */
	  return;
	}
      else
	{
	  swap (data + pos * width, data + smaller * width, width);
	  pos = smaller;
	}
    }
}

size_t
heap_size (struct heap *h)
{
  return vector_size (&h->storage);
}

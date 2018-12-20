#include <stdio.h>

#include "vector.h"

typedef int (*heap_cmp_fn)(const void *lhs, const void *rhs);

struct heap {
  struct vector storage;
  heap_cmp_fn cmp_fn;
};

void heap_init(struct heap *, size_t width, heap_cmp_fn compare);
void heap_destroy(struct heap *);
void heap_insert(struct heap *, const void *elem);
void heap_peek(struct heap *, void *out);
void heap_pop(struct heap *, void *out);

// heap impl

#define START_SIZE 4
#define PARENT(x) (((x)-1)>>1)
#define LEFT(x) (((x)<<1)+1)
#define RIGHT(x) (((x)+1)<<1)

void swap(void *a, void *b, size_t bytes) {
  char *ac = (char *) a, *bc = (char *) b;
  while(--bytes) {
    *ac ^= *bc;
    *bc ^= *ac;
    *ac ^= *bc;
    ac++;
    bc++;
  }
}

void heap_init(struct heap *h, size_t width, heap_cmp_fn compare) {
  vector_init(&h->storage, width, START_SIZE);
  h->cmp_fn = compare;
}

void heap_destroy(struct heap *h) {
  vector_destroy(&h->storage);
}

void heap_insert(struct heap *h, const void *elem) {
  vector_push(&h->storage, elem);
  size_t pos = vector_size(&h->storage)-1;
  size_t width = vector_width(&h->storage);
  char *data = ((char *)(h->storage.data));
  while(pos > 0 && h->cmp_fn(data + width * pos, data + width * PARENT(pos)) < 0) {
    swap(data + width * pos, data + width * PARENT(pos), width);
    pos = PARENT(pos);
  }
}

void heap_peek(struct heap *h, void *out) {
  vector_at(&h->storage, 0, out);
}

void heap_pop(struct heap *h, void *out) {
  size_t width = vector_width(&h->storage);
  if (vector_size(&h->storage) > 1) {
    swap(&h->storage, ((char *)(h->storage.data)) + (vector_size(&h->storage)-1) * width, width);
    //impl recursive downheap
  }
  vector_pop(&h->storage, out);
}

// problem code
void solve_problem6(FILE *input) {
  char before, after;
  while (fscanf("Step %c must be finished before step %c can begin.\n", &before, &after) == 2) {

}

  while (!empty(available)) {
    process(available.pop()) ->
      available.dependents.each {
        blockercount-- 
        if dependent.blockercount == 0
          available.add(dependent)
      }
  }
}

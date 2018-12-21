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
  while(bytes--) {
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
    swap(h->storage.data, ((char *)(h->storage.data)) + (vector_size(&h->storage)-1) * width, width);
  }
  vector_pop(&h->storage, out);
  const size_t size = vector_size(&h->storage);
  char * const data = (char *)(h->storage.data);
  size_t pos = 0, smaller;
  /* find smaller child */
  while(1) {
    if (RIGHT(pos) < size) {
      if (h->cmp_fn(data + LEFT(pos) * width, data + RIGHT(pos) * width) < 0) {
        smaller = LEFT(pos);
      } else {
        smaller = RIGHT(pos);
      }
    } else if (LEFT(pos) < size) {
      smaller = LEFT(pos);
    } else {
      return;
    }
    if (smaller < size && h->cmp_fn(data + pos * width, data + smaller * width) < 0) {
      /* we no longer violate the heap property */
      return;
    } else {
      swap(data + pos * width, data + smaller * width, width);
      pos = smaller;
    }
  }
}

int charcmp(const char *lhs, const char *rhs) {
  return *lhs - *rhs;
}

// problem code
void solve_problem7(FILE *input) {
  char before, after;
  struct heap chars;
  int depcounts[26] = {0};
  struct vector deplist[26];
  for(int i=0; i<26; ++i) {
    vector_init(&deplist[i], sizeof(char), 2);
  }
  heap_init(&chars, sizeof(char), (heap_cmp_fn)charcmp);
  while (fscanf(input, "Step %c must be finished before step %c can begin.\n", &before, &after) == 2) {
    //printf("%c -> %c\n", before, after);
    depcounts[after - 'A']++;
    vector_push(&deplist[before-'A'], &after);
  }
  for(char i='A'; i<='Z'; ++i) {
    if (depcounts[(size_t)(i-'A')] == 0) {
      heap_insert(&chars, &i);
    }
  }
  printf("part1: ");
  while (vector_size((struct vector *)&chars) > 0) {
    char out;
    heap_pop(&chars, &out);
    //printf("out: %c [%lu]\n", out, vector_size((struct vector *)&chars));
    putchar(out);
    VECTOR_FOREACH(char *, dep, deplist[(size_t)(out-'A')]) {
      depcounts[*dep-'A']--;
      if (depcounts[*dep-'A'] == 0) {
        heap_insert(&chars, dep);
      }
    }
  }
  putchar('\n');
  for(int i=0; i<26; ++i) {
    vector_destroy(&deplist[i]);
  }
  heap_destroy(&chars);
}

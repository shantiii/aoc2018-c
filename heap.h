#pragma once

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
size_t heap_size(struct heap *);



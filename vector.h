#pragma once

struct vector {
	size_t size;
	size_t capacity;
	size_t width;
	void *data;
};

void vector_init(struct vector *, size_t width, size_t capacity);
void vector_destroy(struct vector *);

void vector_push(struct vector *, void *elem);
void vector_pop(struct vector *, void *elem);
void vector_insert(struct vector *, size_t index, void *elem);
void vector_remove(struct vector *, size_t index, void *elem);
void vector_at(struct vector *v, size_t index, void *elem);

#define VECTOR_FOREACH(ptr_type, var, vec) \
	for (ptr_type var = (ptr_type)((vec).data); (var) < ((ptr_type)((vec).data)) + (vec).size; var++)

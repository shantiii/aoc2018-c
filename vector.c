#include <sys/types.h>
#include <sysexits.h>
#include <string.h>
#include <stdlib.h>

#include "vector.h"

#define GROWTH_FACTOR 1.5f
//#define SHRINK_THRESHOLD 0.5f

void vector_init(struct vector *v, size_t width, size_t capacity) {
	if (v) {
		v->size = 0;
		v->width = width;
		v->capacity = capacity;
		if ((v->data = malloc(width * capacity)) == NULL) {
			exit(EX_OSERR);
		}
	}
}

void vector_destroy(struct vector *v) {
	if (v) {
		free(v->data);
	}
}

void vector_push(struct vector *v, void *elem) {
	if (v->size == v->capacity) {
		register size_t new_capacity = v->capacity * GROWTH_FACTOR;
		v->data = reallocf(v->data, new_capacity * v->width);
		if (!v->data) {
			exit(EX_OSERR);
		}
		v->capacity = new_capacity;
	}
	(void) memcpy(((char *)v->data) + v->width * v->size, elem, v->width);
	v->size++;
}

void vector_pop(struct vector *v, void *elem) {
	if (v->size > 0) {
		v->size--;
		if (elem) {
			(void) memcpy(elem, ((char *)v->data) + v->width * v->size, v->width);
		}
	}
}

void vector_at(struct vector *v, size_t index, void *elem) {
	if (v->size > 0 && elem) {
		(void) memcpy(elem, ((char *)v->data) + v->width * index, v->width);
	}
}

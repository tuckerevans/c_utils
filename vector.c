#include "vector.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>

#define START_SIZE 64;

struct vector {
	void **base, **new, **ptr;
	int i, limit;
};

vec* vec_new()
{
	vec *root;

	root = malloc(sizeof(vec));
	assert(root);

	root->limit = START_SIZE;
	root->base = root->new = root->ptr = malloc(root->limit * sizeof(void*));
	assert(root->base);

	return root;
}

vec* vec_with_capacity(n)
int n;
{
	vec *root;

	root = malloc(sizeof(vec));
	assert(root);

	root->limit = n;
	root->base = root->new = root->ptr = malloc(root->limit * sizeof(void*));
	assert(root->base);

	return root;
}

int vec_size(root)
vec *root;
{
	if (!root) {
		return -1;
	}
	return (root->new - root->ptr);
}

void vec_resize(root)
vec *root;
{
	if (root->ptr != root->base) {
		memmove(root->base, root->ptr, root->new - root->ptr);
		root->new = root->base + vec_size(root);
		root->ptr = root->base;
	} else {
		root->base = malloc(root->limit * 2 * sizeof(void*));
		assert(root->base);


		root->base = memcpy(root->base, root->ptr, root->limit * sizeof(void*));
		assert(root->base);


		root->new = root->base + vec_size(root);
		root->limit = root->limit * 2;

		free(root->ptr);
		root->ptr = root->base;
	}
}

void vec_push(root, item)
vec *root;
void *item;
{
	if (!root) {
		return;
	}
	if (root->new == root->base + root->limit) {
		vec_resize(root);
	}

	*(root->new++) = item;
}

void* vec_rmfirst(root)
vec *root;
{
	void* tmp;
	if (!root || root->ptr == root->new) {
		return NULL;
	}

	return tmp = *(root->ptr++);
}

void* vec_index(root, index)
vec *root;
int index;
{
	if (!root || root->ptr + index >= root->new) {
		return NULL;
	}

	return root->ptr[index];
}

void* vec_pop(root)
vec *root;
{
	void* tmp;
	if (!root || root->new == root->ptr) {
		return NULL;
	}

	return tmp = *(--root->new);
}

void vec_free(root)
vec *root;
{
	free(root->base);
	root->base = NULL;
	root->new = NULL;
	root->ptr = NULL;

	free(root);
}

void vec_print(root)
vec *root;
{
	void **tmp;

	fprintf(stderr, "VEC[b: %p, p: %p, n:%p]:\n\t ",
			root->base, root->ptr, root->new);
	for (tmp = root->ptr; tmp < root->new; tmp++){
		fprintf(stderr, "[%p]", *tmp);
	}
	fprintf(stderr, "\n");
}

vec* vec_cp(root)
vec *root;
{
	vec *copy;

	copy = vec_with_capacity(root->limit);

	copy->base = memcpy(copy->base, root->ptr,
			vec_size(root) * sizeof(void*));
	assert(copy->base);

	copy->ptr = copy->base;
	copy->new = copy->base + vec_size(root);
}

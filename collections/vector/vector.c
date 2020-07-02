#include "vector.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>

#define START_SIZE 64;

struct vector {
	void **base;
	int end, limit;
};

vec* vec_new()
{
	vec *root;

	root = malloc(sizeof(vec));
	assert(root);

	root->limit = START_SIZE;
	root->base = malloc(root->limit * sizeof(void*));
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
	root->base = malloc(root->limit * sizeof(void*));
	assert(root->base);

	return root;
}

int vec_size(root)
vec *root;
{
	if (!root) {
		return -1;
	}
	return root->end;
}

void vec_resize(root)
vec *root;
{
	if (!root)
		return;

	root->base = reallocarray(root->base, root->limit * 2, sizeof(void*));
	assert(root->base);
}

void vec_push(root, item)
vec *root;
void *item;
{
	if (!root) {
		return;
	}

	if (root->end >= root->limit) {
		vec_resize(root);
	}

	root->base[root->end++] = item;
}

void* vec_index(root, index)
vec *root;
int index;
{
	if (!root || index >= root->end || index < 0) {
		return NULL;
	}

	return root->base[index];
}

void* vec_pop(root)
vec *root;
{
	if (!root || root->end == 0) {
		return NULL;
	}

	return root->base[--root->end];
}

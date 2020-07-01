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


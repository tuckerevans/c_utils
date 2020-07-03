#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define START_SIZE 64;

struct vector {
	void **base;
	int end, limit;
};

void vec_debug_print(root)
vec *root;
{
	int i;

	fprintf(stderr, "VEC[base: %p, end: %p, limit:%p]:\n\t ",
			root->base, root->end, root->limit);
	for (i=0; i < root->end; i++){
		fprintf(stderr, "[%p]", vec_index(root,i));
	}
	fprintf(stderr, "\n");
}

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

vec* vec_cp(root)
vec *root;
{
	vec *copy;

	if (!root)
		return NULL;

	copy = vec_with_capacity(root->limit);

	copy->base = memcpy(copy->base, root->base,
			vec_size(root) * sizeof(void*));
	assert(copy->base);

	copy->end = root->end;
	copy->limit = root->limit;

	return copy;
}

void vec_print(root, to_string)
vec *root;
char* to_string(void*);
{
	int i;

	printf("[");
	for(i = 0; i < root->end; i++) {
		printf("%s", to_string(vec_index(root, i)));
	}
	printf("\b]\n");

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

void* vec_pop(root)
vec *root;
{
	if (!root || root->end == 0) {
		return NULL;
	}

	return root->base[--root->end];
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

void vec_clear(root)
vec *root;
{
	int i;

	for (i = 0; i < root->end; i++) {
		free(vec_index(root, i));
	}

	root->end = 0;
}

void vec_free(root)
vec *root;
{
	free(root->base);
	root->base = NULL;

	free(root);
}

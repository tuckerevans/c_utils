#include "double_ended_queue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>

#define START_SIZE 64;

struct double_ended_queeu {
	void **base, **new, **ptr;
	int i, limit;
};

deq* deq_new()
{
	deq *root;

	root = malloc(sizeof(deq));
	assert(root);

	root->limit = START_SIZE;
	root->base = root->new = root->ptr = malloc(root->limit * sizeof(void*));
	assert(root->base);

	return root;
}

deq* deq_with_capacity(n)
int n;
{
	deq *root;

	root = malloc(sizeof(deq));
	assert(root);

	root->limit = n;
	root->base = root->new = root->ptr = malloc(root->limit * sizeof(void*));
	assert(root->base);

	return root;
}

int deq_size(root)
deq *root;
{
	if (!root) {
		return -1;
	}
	return (root->new - root->ptr);
}

void deq_resize(root)
deq *root;
{
	if (root->ptr != root->base) {
		memmove(root->base, root->ptr, root->new - root->ptr);
		root->new = root->base + deq_size(root);
		root->ptr = root->base;
	} else {
		root->base = malloc(root->limit * 2 * sizeof(void*));
		assert(root->base);


		root->base = memcpy(root->base, root->ptr, root->limit * sizeof(void*));
		assert(root->base);


		root->new = root->base + deq_size(root);
		root->limit = root->limit * 2;

		free(root->ptr);
		root->ptr = root->base;
	}
}

void deq_push(root, item)
deq *root;
void *item;
{
	if (!root) {
		return;
	}
	if (root->new == root->base + root->limit) {
		deq_resize(root);
	}

	*(root->new++) = item;
}

void* deq_rmfirst(root)
deq *root;
{
	void* tmp;
	if (!root || root->ptr == root->new) {
		return NULL;
	}

	return tmp = *(root->ptr++);
}

void* deq_index(root, index)
deq *root;
int index;
{
	if (!root || root->ptr + index >= root->new) {
		return NULL;
	}

	return root->ptr[index];
}

void* deq_pop(root)
deq *root;
{
	void* tmp;
	if (!root || root->new == root->ptr) {
		return NULL;
	}

	return tmp = *(--root->new);
}

void deq_free(root)
deq *root;
{
	free(root->base);
	root->base = NULL;
	root->new = NULL;
	root->ptr = NULL;

	free(root);
}

void deq_print(root)
deq *root;
{
	void **tmp;

	fprintf(stderr, "VEC[b: %p, p: %p, n:%p]:\n\t ",
			root->base, root->ptr, root->new);
	for (tmp = root->ptr; tmp < root->new; tmp++){
		fprintf(stderr, "[%p]", *tmp);
	}
	fprintf(stderr, "\n");
}

deq* deq_cp(root)
deq *root;
{
	deq *copy;

	copy = deq_with_capacity(root->limit);

	copy->base = memcpy(copy->base, root->ptr,
			deq_size(root) * sizeof(void*));
	assert(copy->base);

	copy->ptr = copy->base;
	copy->new = copy->base + deq_size(root);
}

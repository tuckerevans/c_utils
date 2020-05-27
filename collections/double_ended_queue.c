#include "double_ended_queue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>

#define START_SIZE 64;

/*TODO add returned error codes for current void functions
 * (resize, push, etc.)
 */

struct double_ended_queue {
	void **base, **end, **beg;
	int limit;
};

deq* deq_new()
{
	deq *root;

	root = malloc(sizeof(deq));
	assert(root);

	root->limit = START_SIZE;
	root->base = root->end = root->beg = malloc(root->limit * sizeof(void*));
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
	root->base = root->end = root->beg = malloc(root->limit * sizeof(void*));
	assert(root->base);

	return root;
}

int deq_size(root)
deq *root;
{
	if (!root) {
		return -1;
	}

	if (root->beg <= root->end) {
		return (root->end - root->beg);
	}

	return (root->base + root->limit - root->beg) + (root->end - root->base);
}

void deq_resize(root)
deq *root;
{
	if (root->beg != root->base) {
		memmove(root->base, root->beg, root->end - root->beg);
		root->end = root->base + deq_size(root);
		root->beg = root->base;
	} else {
		root->base = malloc(root->limit * 2 * sizeof(void*));
		assert(root->base);


		root->base = memcpy(root->base, root->beg, root->limit * sizeof(void*));
		assert(root->base);


		root->end = root->base + deq_size(root);
		root->limit = root->limit * 2;

		free(root->beg);
		root->beg = root->base;
	}
}

void* deq_index(root, index)
deq *root;
int index;
{
	void **tmp;

	if (!root) {
		return NULL;
	}

	tmp = root->base + ((root->beg + index - root->base) % root->limit);
	if (tmp > root->end) {
		return NULL;
	}

	return *tmp;
}

void deq_push_back(root, item)
deq *root;
void *item;
{
	void **tmp;

	if (!root) {
		return;
	}

	tmp = (root->base + (root->end - root->base) % root->limit);
	if (tmp == root->beg) {
		deq_resize(root);
		tmp = (root->base + (root->end - root->base) % root->limit);
	}

	*(root->end = tmp) = item;
}

void deq_push_front(root, item)
deq *root;
void *item;
{
	void **tmp;

	if (!root) {
		return;
	}

	tmp = (root->base + ((root->beg - 1) - root->base) % root->limit);
	if (tmp == root->beg) {
		deq_resize(root);
		tmp = (root->base + ((root->beg - 1) - root->base) % root->limit);
	}

	*(root->beg = tmp) = item;
}

void* deq_pop_front(root)
deq *root;
{
	void* tmp;
	if (!root || root->beg == root->end) {
		return NULL;
	}

	tmp = *(root->beg++);
	root->beg = (root->base + (root->beg - root->base) % root->limit);

	return tmp;
}

void* deq_pop_back(root)
deq *root;
{
	void* tmp;
	if (!root || root->end == root->beg) {
		return NULL;
	}

	tmp = *(root->end--);
	root->end = (root->base + (root->end - root->base) % root->limit);

	return tmp;
}

void deq_free(root)
deq *root;
{
	free(root->base);
	root->base = NULL;
	root->end = NULL;
	root->beg = NULL;

	free(root);
}

void deq_print(root, to_string)
deq *root;
char* to_string(void*);
{
	int i, size;;

	size = deq_size(root);

	printf("[");
	for (i = 0; i < size; i++) {
		printf("%s,", to_string(deq_index(root, i)));
	}
	printf("\b]\n");
}

void deq_debug_print(root)
deq *root;
{
	void **tmp;

	fprintf(stderr, "VEC[b: %p, p: %p, n:%p]:\n\t ",
			root->base, root->beg, root->end);
	for (tmp = root->beg; tmp < root->end; tmp++){
		fprintf(stderr, "[%p]", *tmp);
	}
	fprintf(stderr, "\n");
}

deq* deq_cp(root)
deq *root;
{
	deq *copy;

	copy = deq_with_capacity(root->limit);

	copy->base = memcpy(copy->base, root->beg,
			deq_size(root) * sizeof(void*));
	assert(copy->base);

	copy->beg = copy->base;
	copy->end = copy->base + deq_size(root);
}

void deq_swap(root, i, j)
deq *root;
int i, j;
{
	int len;
	void *tmp;

	if (!root) {
		return;
	}

	len = deq_size(root);

	if (i >= len || j >= len) {
		return;
	}

	tmp = root->end[i];
	root->end[j] = root->end[i];
	root->end[i] = tmp;
}

void deq_truncate(root, size)
deq *root;
int size;
{
	if ((!root) || size > deq_size(root)) {
		return;
	}

	root->end = root->beg + size;
}

void* deq_front(root)
deq *root;
{
	if (!root) {
		return NULL;
	}

	return *root->beg;
}

void* deq_back(root)
deq *root;
{
	if (!root) {
		return NULL;
	}

	return *root->end;
}

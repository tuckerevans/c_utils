#include "double_ended_queue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>

/*Checks bounds for index (y) in queue*/
#define DEQ_IN_BOUNDS(x, y) (y < deq_size(x)) \

#define START_SIZE 64;

/*TODO add returned error codes for current void functions
 * (resize, push, etc.)
 *
 * Should all functions return a status code and take in a dest argument?
 */

/* Double ended queue as a circular buffer
 * base is pointer to buffer.
 * beg: postiton of the first element is stored.
 * end: position for the next element to be stored.
 * limit: the maximum number of elements.
 */

struct double_ended_queue {
	void **base;
	int beg, end, limit;
};

deq* deq_new()
{
	deq *root;

	root = malloc(sizeof(deq));
	assert(root);

	root->limit = START_SIZE;
	root->beg = root->end = 0;
	root->base = malloc(root->limit * sizeof(void*));
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
	root->beg = root->end = 0;
	root->base = malloc(root->limit * sizeof(void*));
	assert(root->base);

	return root;
}

int deq_size(root)
deq *root;
{
	if (!root)
		return -1;

	if (root->beg <= root->end)
		return (root->end - root->beg);

	return (root->limit - root->beg) + (root->end);
}

void deq_resize(root)
deq *root;
{
	void **tmp;
	int size;

	tmp = malloc(root->limit * 2 * sizeof(void*));
	assert(root->base);

	if (root->beg <= root->end) {
		tmp = memcpy(tmp, root->base + root->beg,
				deq_size(root) * sizeof(void*));
	} else {
		/*copy beg<->limit*/
		size = root->limit - root->beg;
		tmp = memcpy(tmp, root->base + root->beg, size * sizeof(void*));
		assert(tmp);

		/*copy base<->end*/
		tmp = memcpy(tmp + size, root->base,
				root->end * sizeof(void*));
		assert(tmp);
	}

	root->limit *= 2;

	free(root->base);
	root->base = tmp;
}

void* deq_index(root, index)
deq *root;
int index;
{
	void **tmp;

	if (!root || index > root->limit
			|| (index >= root->end && index < root->beg))
		return NULL;

	index = (root->beg + index) % root->limit;
	if (index >= root->end)
		return NULL;

	return root->base[index];
}

void deq_push_back(root, item)
deq *root;
void *item;
{
	void **tmp;

	if (!root)
		return;

	if (root->end >= root->limit)
		deq_resize(root);

	root->base[root->end++] = item;
	root->end %= root->limit;

	if (root->end == root->beg)
		root->end = root->limit;
}

void deq_push_front(root, item)
deq *root;
void *item;
{
	void **tmp;

	if (!root)
		return;

	if (root->end >= root->limit)
		deq_resize(root);

	--root->beg;
	root->beg %= root->limit;

	root->base[root->beg] = item;

	if (root->end == root->beg)
		root->end = root->limit;
}

void* deq_pop_front(root)
deq *root;
{
	void* tmp;
	if (!root || root->beg == root->end)
		return NULL;

	tmp = root->base[root->beg];

	++root->beg;
	root->beg %= root->limit;

	return tmp;
}

void* deq_pop_back(root)
deq *root;
{
	if (!root || root->end == root->beg)
		return NULL;

	--root->end;
	root->end %= root->limit;

	return root->base[root->end];
}

void* deq_swap_rm_front(root, index)
deq *root;
int index;
{
	if (!root || !DEQ_IN_BOUNDS(root,index))
		return NULL;

	deq_swap(root, 0, index);
	return deq_pop_front(root);
}

void* deq_swap_rm_back(root, index)
deq *root;
int index;
{
	if (!root || !DEQ_IN_BOUNDS(root,index))
		return NULL;

	deq_swap(root,deq_size(root) - 1,index);
	return deq_pop_back(root);
}

void deq_remove(root, index)
deq *root;
int index;
{
	if (!root || !DEQ_IN_BOUNDS(root,index));
		return;

	index = (root->beg + index) % root->limit;

	root->base[index] = NULL;

	if (root->beg < root->end || index >= root->beg) {
		memmove(root->base + root->beg + 1,
				root->base + root->beg,
				(index - root->beg) * sizeof(void*));
		++root->beg;
	} else {
		memmove(root->base + index, root->base + index + 1,
				(--root->end - index) * sizeof(void*));
	}
}

/* Note: Elements are not freed
 * deq_clear should be called before if they are no longer needed.
 */
void deq_free(root)
deq *root;
{
	free(root->base);
	root->base = NULL;

	free(root);
}

void deq_clear(root)
deq *root;
{
	int i, size;

	size = deq_size(root);
	for (i = 0; i < size; i++) {
		free(deq_index(root, i));
	}
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

	fprintf(stderr, "VEC[b: %p, beg: %d, end:%d]:\n\t ",
			root->base, root->beg, root->end);
	for (tmp = root->base; tmp < root->base + root->limit; tmp++){
		fprintf(stderr, "[%p]", *tmp);
	}
	fprintf(stderr, "\n");
}

deq* deq_cp(root)
deq *root;
{
	deq *copy;

	copy = deq_with_capacity(root->limit);
	assert(copy->base);

	copy->base = memcpy(copy->base, root->base,
			root->limit * sizeof(void*));
	assert(copy->base);

	copy->beg = root->beg;
	copy->end = root->end;

	return copy;
}

void deq_swap(root, i, j)
deq *root;
int i, j;
{
	int len;
	void *tmp;

	if (!root)
		return;

	len = deq_size(root);

	if (i >= len || j >= len)
		return;

	i += root->beg;
	i %= root->limit;

	j += root->beg;
	j %= root->limit;

	tmp = root->base[i];
	root->base[i] = root->base[j];
	root->base[j] = tmp;
}

/*Note: Does not currently reduce memory footprint*/
void deq_truncate(root, size)
deq *root;
int size;
{
	if (!root || size > deq_size(root))
		return;

	root->end = (root->beg + size) % root->limit;
}

void* deq_front(root)
deq *root;
{
	if (!root)
		return NULL;

	return root->base[root->beg];
}

void* deq_back(root)
deq *root;
{
	if (!root)
		return NULL;

	return root->base[(root->end - 1) % root->limit];
}

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rope.h"

struct rope_s {
	size_t len;
	char *str;

	struct rope_s *left, *right, *parent;
};


rope* rope_new()
{
	rope *tmp;

	tmp = malloc(sizeof(rope));
	assert(tmp);

	tmp->len = 0;
	tmp->str = NULL;
	tmp->left = NULL;
	tmp->right = NULL;
	tmp->parent = NULL;

	return tmp;
}

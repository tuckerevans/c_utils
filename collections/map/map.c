#include "map.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct map_node {
	void *key, *val;
	cmp_func cmp;

	struct map_node *left, *right, *parent;
};

map* map_new(cmp)
cmp_func cmp;
{
	map *tmp;

	tmp = malloc(sizeof(map));
	assert(tmp);

	tmp->cmp = cmp;
	tmp->key = tmp->val = tmp->left = tmp->right = tmp->parent = NULL;

	return tmp;
}

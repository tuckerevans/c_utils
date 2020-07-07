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

int map_height(root)
map *root;
{
	int l, r;

	if (!root || !root->key)
		return 0;

	l = map_height(root->left);
	r = map_height(root->right);

	return 1 + ( l > r ? l : r);
}

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

map* map_new_from_parent(root)
map *root;
{
	map *tmp;

	tmp = map_new(root->cmp);
	tmp->parent = root;
	return tmp;
}

int map_size(root)
map *root;
{
	if (!root || !root->key)
		return 0;

	return map_size(root->left) + map_size(root->right) + 1;
}

int map_insert(root, key, val)
map *root;
void *key, *val;
{
	int cmp;

	if (!root)
		return -1;

	if (!root->key) {
		root->key = key;
		root->val = val;
		return 0;
	}

	cmp = root->cmp(root->key, key);

	if (cmp == 0 && root->key == key) {
		root->val = val;
	} else if (cmp < 0) {

		if (!root->left)
			root->left = map_new_from_parent(root);

		map_insert(root->left, key, val);
	} else if (cmp > 0) {
		if (!root->right)
			root->right = map_new_from_parent(root);

		map_insert(root->right, key, val);
	} else {
		return -1;
	}

	return 0;
}

void* map_reset_key(root, key)
map *root;
void *key;
{
	void *tmp;
	int cmp;

	if (!root || !key)
		return NULL;

	cmp = root->cmp(root->key, key);

	if (cmp < 0)
		return map_reset_key(root->left, key);

	if (cmp > 0)
		return map_reset_key(root->right, key);

	tmp = root->key;
	root->key = key;
	return tmp;
}

void* map_index(root, key)
map *root;
void *key;
{
	int cmp;
	if (!root || !root->key)
		return NULL;

	cmp = root->cmp(root->key, key);

	if (cmp < 0)
		return map_index(root->left, key);

	if (cmp > 0)
		return map_index(root->right, key);

	return root->val;
}

void map_clear(root)
map *root;
{
	map *l, *r;

	l = root->left;
	r = root->right;

	if (!root)
		return;

	if (root->parent) {
		free(root->key);
		root->key = NULL;

		free(root->val);
		root->val = NULL;

		root->parent = NULL;

		free(root);
	}

	map_clear(l);
	map_clear(r);
}

void map_free(root)
map *root;
{
	if (!root)
		return;

	root->key = NULL;

	map_free(root->left);
	root->left = NULL;

	map_free(root->right);
	root->right = NULL;

	free(root);
}

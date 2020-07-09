#include "map.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct map_root {
	struct map_node *root;

	cmp_func cmp;
};

struct map_node {
	void *key, *val;

	struct map_node *left, *right, *parent;
};

int map_height(root)
struct map_node *root;
{
	int l, r;

	if (!root)
		return 0;

	l = map_height(root->left);
	r = map_height(root->right);

	return 1 + ( l > r ? l : r);
}

int map_bal_factor(root)
struct map_node *root;
{
	return map_height(root->right) - map_height(root->left);
}

map* map_new(cmp)
cmp_func cmp;
{
	map *tmp;

	tmp = malloc(sizeof(map));
	assert(tmp);

	tmp->cmp = cmp;
	tmp->root = NULL;

	return tmp;
}

struct map_node* map_new_node()
{
	struct map_node *tmp;

	assert(tmp = malloc(sizeof(struct map_node)));
	tmp->parent = tmp->left = tmp-> right = tmp-> key = tmp->val = NULL;
	return tmp;
}

int map_size_aux(root)
struct map_node *root;
{
	if (!root)
		return 1;

	return map_size_aux(root->left) + map_size_aux(root->right) + 1;
}

int map_size(root)
map *root;
{
	if (!root || !root->root)
		return 0;

	return map_size_aux(root->root);
}

int map_insert_aux(root, cmpf, key, val)
struct map_node *root;
cmp_func cmpf;
void *key, *val;
{
	int cmp;

	if (!root)
		return -1;

	cmp = cmpf(key, root->key);

	if (cmp < 0) {

		if (!root->left) {
			root->left = map_new_node(root);

			root->left->parent = root;
			root->left->key = key;
			root->left->val = val;
			return 0;
		}

		return map_insert_aux(root->left, cmpf, key, val);
	}

	if (cmp > 0) {
		if (!root->right) {
			root->right = map_new_node(root);

			root->right->parent = root;
			root->right->key = key;
			root->right->val = val;
			return 0;
		}

		return map_insert_aux(root->right, cmpf, key, val);
	}

	return -1;
}

int map_insert(root, key, val)
map *root;
void *key, *val;
{
	int cmp;

	if (!root)
		return -1;

	if (!root->root) {
		root->root = map_new_node();
		root->root->key = key;
		root->root->val = val;
		return 0;
	}

	return map_insert_aux(root->root, root->cmp, key, val);
}

void* map_set_val_aux(root, cmpf, key, val)
struct map_node *root;
cmp_func cmpf;
void *key, *val;
{
	int cmp;
	void *tmp;

	if (!root)
		return NULL;

	cmp = cmpf(key, root->key);

	if (cmp < 0)
		return map_set_val_aux(root->left, cmpf, key, val);

	if (cmp > 0)
		return map_set_val_aux(root->right, cmpf, key, val);

	tmp = root->val;
	root->val = val;
	return tmp;
}

void* map_set_val(root, key, val)
map *root;
void *key, *val;
{
	if (!root)
		return NULL;

	return map_set_val_aux(root->root, root->cmp, key, val);
}

int map_check_key_ptr_aux(root, cmpf, key)
struct map_node *root;
cmp_func cmpf;
void *key;
{
	int cmp;

	if(!root)
		return 0;

	cmp = cmpf(key, root->key);

	if (cmp < 0)
		return map_check_key_ptr_aux(root->left, cmpf, key);
	if (cmp > 0)
		return map_check_key_ptr_aux(root->right, cmpf, key);

	return root->key == key;
}

int map_check_key_ptr(root, key)
map *root;
void *key;
{
	if (!root)
		return 0;

	return map_check_key_ptr_aux(root->root, root->cmp, key);
}

void* map_set_key_aux(root, cmpf, key)
struct map_node *root;
cmp_func cmpf;
void *key;
{
	void *tmp;
	int cmp;

	if (!root)
		return NULL;

	cmp = cmpf(key, root->key);

	if (cmp < 0)
		return map_set_key_aux(root->left, cmpf, key);

	if (cmp > 0)
		return map_set_key_aux(root->right, cmpf, key);

	tmp = root->key;
	root->key = key;
	return tmp;
}

void* map_set_key(root, key)
map *root;
void *key;
{

	if (!root)
		return NULL;

	return map_set_key_aux(root->root, root->cmp, key);
}

void* map_index_aux(root, cmpf, key)
struct map_node *root;
cmp_func cmpf;
void *key;
{
	int cmp;

	if (!root)
		return NULL;

	cmp = cmpf(key, root->key);

	if (cmp < 0)
		return map_index_aux(root->left, cmpf, key);

	if (cmp > 0)
		return map_index_aux(root->right, cmpf, key);

	return root->val;
}

void* map_index(root, key)
map *root;
void *key;
{
	if (!root)
		return NULL;

	return map_index_aux(root->root, root->cmp, key);
}

void map_clear_aux(root)
struct map_node *root;
{
	if (!root)
		return;

	map_clear_aux(root->left);
	root->left = NULL;

	map_clear_aux(root->right);
	root->right= NULL;


	free(root->key);
	root->key = NULL;

	free(root->val);
	root->val = NULL;

	root->parent = NULL;
	free(root);

	return;
}

void map_clear(root)
map *root;
{
	if (!root)
		return;

	map_clear_aux(root->root);
	root->root = NULL;
}

void map_free_aux(root)
struct map_node *root;
{
	if (!root)
		return;

	map_free_aux(root->left);
	root->left = NULL;

	map_free_aux(root->right);
	root->right= NULL;

	root->parent = NULL;

	free(root);
	return;
}

void map_free(root)
map *root;
{
	if (!root)
		return;

	map_free_aux(root->root);

	free(root);
	return;
}

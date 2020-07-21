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
	if (!root)
		return 0;

	return map_height(root->right) - map_height(root->left);
}

void map_rotate_left(node)
struct map_node **node;
{
	struct map_node *tmp;

	tmp = *node;
	*node = tmp->right;
	tmp->right = (*node)->left;
	(*node)->left = tmp;

	(*node)->parent = (*node)->left->parent;
	(*node)->left->parent = *node;
	
	if ((*node)->left->right)
		(*node)->left->right->parent = (*node)->left;

	return;
}

void map_rotate_right(node)
struct map_node **node;
{
	struct map_node *tmp;

	tmp = *node;
	*node = tmp->left;
	tmp->left = (*node)->right;
	(*node)->right = tmp;

	(*node)->parent = (*node)->right->parent;
	(*node)->right->parent = *node;

	if ((*node)->right->left)
		(*node)->right->left->parent = (*node)->right;

	return;
}

void map_rebal_tree(root)
struct map_node **root;
{
	struct map_node *tmp;

	if(! *root)
		return;

	if (map_height((*root)->left) > map_height((*root)->right)) {
		if(map_bal_factor((*root)->left) >= 0) {
			map_rotate_right(root);
		} else {
			map_rotate_right(&(*root)->left);
			map_rotate_left(root);
		}
	} else {
		if (map_bal_factor((*root)->right) >= 0) {
			map_rotate_left(root);
		} else {
			map_rotate_left(&(*root)->right);
			map_rotate_right(root);
		}
	}

	return;
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
		return 0;

	return map_size_aux(root->left) + map_size_aux(root->right) + 1;
}

int map_size(root)
map *root;
{
	if (!root || !root->root)
		return 0;

	return map_size_aux(root->root);
}

int map_insert_aux(root, cmpf, key, val, into)
struct map_node *root, **into;
cmp_func cmpf;
void *key, *val;
{
	int cmp;

	if (!root)
		return -1;

	cmp = cmpf(key, root->key);

	if (cmp < 0) {

		if (!root->left) {
			root->left = *into = map_new_node(root);

			root->left->parent = root;
			root->left->key = key;
			root->left->val = val;

			return 0;
		}

		return map_insert_aux(root->left, cmpf, key, val, into);
	}

	if (cmp > 0) {
		if (!root->right) {
			root->right = *into = map_new_node(root);

			root->right->parent = root;
			root->right->key = key;
			root->right->val = val;

			return 0;
		}

		return map_insert_aux(root->right, cmpf, key, val, into);
	}

	return -1;
}

int map_insert(root, key, val)
map *root;
void *key, *val;
{
	int cmp, ret, bal;
	struct map_node *new;

	if (!root)
		return -1;

	if (!root->root) {
		root->root = map_new_node();
		root->root->key = key;
		root->root->val = val;
		return 0;
	}

	ret = map_insert_aux(root->root, root->cmp, key, val, &new);

	if (ret != 0)
		return ret;


	do {
		if (!new->parent)
			return 0;

		new = new->parent;
		bal = map_bal_factor(new);
	} while (bal > -2 && bal < 2);


	if (!new->parent)
		map_rebal_tree(&(root->root));
	else if (new == new->parent->left)
		map_rebal_tree(&(new->parent->left));
	else if (new == new->parent->right)
		map_rebal_tree(&(new->parent->right));
	else
		assert(0);

	return 0;
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

void* map_remove_aux(root, cmpf, key)
struct map_node **root;
cmp_func cmpf;
void *key;
{
	struct map_node *next, *next_parent, *tmp;
	void *ret;
	int cmp, bal;

	if (!*root)
		return NULL;

	cmp = cmpf(key, (*root)->key);

	if (cmp < 0)
		return map_remove_aux(&(*root)->left, cmpf, key);

	if (cmp > 0)
		return map_remove_aux(&(*root)->right, cmpf, key);

	tmp = *root;

	if (tmp->left && tmp->right ) {
		for (next = tmp->right; next->left; next = next->left);
	} else if (tmp->left || tmp->right){
		next = tmp->left ? tmp->left : tmp->right;
	} else {
		next = NULL;
	}

	if (next) {
		next_parent = next->parent;

		if (next_parent == tmp) {
			if (next->right)
			next->right->parent = next_parent;

			next_parent->right = next->right;
			next->right = NULL;
		} else if (next_parent) {
			if (next->right) {
				next->right->parent = next_parent;

				next_parent->left = next->right;
				next->right = NULL;
			} else {
				next_parent->left = NULL;
			}
		}
		next->parent = tmp->parent;
		next->right = tmp->right;
		next->left = tmp->left;

		if (next->left)
			next->left->parent = next;
		if (next->right)
			next->right->parent = next;

	} else {
		next_parent = tmp->parent;
		next = NULL;
	}

	ret = tmp->val;

	tmp->parent = tmp->right = tmp->left = NULL;
	free(tmp);

	*root = next;

	//CHECK BALANCE
	return ret;
}

void* map_remove(root, key)
map *root;
void *key;
{
	if (!root)
		return NULL;

	return map_remove_aux(&root->root, root->cmp, key);
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

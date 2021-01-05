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

void rope_free(root)
rope *root;
{
	if (root->str)
		free(root->str);
	if (root->left)
		free(root->left);
	if (root->right)
		free(root->right);

	free(root);

	return;
}

void rope_debug_print_aux(root, s)
rope *root;
int s;
{
	int i;

	for (i = 0; i < s; i++)
		printf("|    ");

	if (!root) {
		printf("\n");
		return;
	}

#ifdef ROPE_DEBUG_PARENT
	printf("[%p]\n", root->parent);
#endif

	printf("{len:%d,str:\'%s\'}[%p]\n", root->len, root->str, root);

	if (root->str)
		return;

	rope_debug_print_aux(root->left, ++s);
	rope_debug_print_aux(root->right, s);

}

void rope_debug_print(root)
rope *root;
{
	rope_debug_print_aux(root, 0);
}

void rope_print(root)
rope *root;
{
	if (!root)
		return;

	if (root->str)
		printf("%s", root->str);

	rope_print(root->left);
	rope_print(root->right);
}

size_t rope_len(root)
rope *root;
{
	if (!root)
		return 0;

	if (root->str)
		return root->len;

	return rope_len(root->left) + rope_len(root->right);
}

size_t rope_relen(root)
rope **root;
{
	rope *tmp;

	if (!*root)
		return 0;

	tmp = *root;

	if (tmp->str) {
		return tmp->len;
	} else {
		if (tmp->left && !tmp->right) {
			tmp->left->parent = tmp->left;
			*root = tmp->left;

			tmp->left = NULL;
			rope_free(tmp);
		} else if (tmp->right && !tmp->left) {
			tmp->right->parent = tmp->right;
			*root = tmp->right;

			tmp->right = NULL;
			rope_free(tmp);
		}
	}

	return (*root)->len = rope_relen(&(*root)->left) + rope_relen(&(*root)->right);
}

rope* str_to_rope(str)
char *str;
{
	rope *tmp;

	if (!str)
		return NULL;

	tmp = rope_new();
	tmp->len = strlen(str);
	tmp->str = strdup(str);

	return tmp;
}

rope* rope_concat(node1, node2)
rope *node1, *node2;
{
	rope *tmp;

	if (!node1 || !node2)
		return node1 ? node1 : node2;

	tmp = rope_new();
	tmp->len = rope_len(node1);
	tmp->left = node1;
	tmp->right = node2;
	node1->parent = tmp;
	node2->parent = tmp;

	return tmp;
}

rope* rope_split(root, i)
rope **root;
size_t i;
{
	rope **tmp, *split, *cur;

	rope *orig = *root;

	if (!root )
		return NULL;

	if (!i) {
		split = *root;
		*root = NULL;
		return split;
	}


	tmp = root;
			printf("i: %d\n", i);
	while (!(*tmp)->str) {
		if ((*tmp)->len <= i && (*tmp)->right) {
			i -= (*tmp)->len;
			printf("i: %d\n", i);
			tmp = &(*tmp)->right;
		} else if ((*tmp)->left) {
			tmp = &(*tmp)->left;
		}
	}

	if (i != 0) {
		split = str_to_rope((*tmp)->str + i);
		(*tmp)->str[i] = '\0';
			/*Maybe want to strdup this and free to reduce memory usage*/

		cur = (*tmp)->parent;
		*tmp = rope_concat(*tmp, split);
		(*tmp)->parent = cur;

		split = (*tmp)->right;
		(*tmp)->right = NULL;
	} else {
		split = *tmp;
	}


	for(cur = split; cur->parent; cur = cur->parent) {
		if (cur->parent->right == cur)
			continue;

		split = rope_concat(split, cur->parent->right);
		cur->parent->right = NULL;
	}

	if (cur && cur->right == split)
		cur->right = NULL;
	*root = cur;

	/*TODO should be rebalancing rather than just recalulating lengths*/
	rope_relen(&split);
	rope_relen(root);

	return split;
}

rope* rope_insert_str(root, str)
rope *root;
char *str;
size_t idx;
{
	return rope_insert(root, str_to_rope(str), idx);
}

rope* rope_insert(root, ins, idx)
rope *root, *ins;
size_t idx;
{
	rope* tmp;
	
	tmp = rope_split(&root, idx+1);
	root = rope_concat(root, ins);
	return rope_concat(root, tmp);
}
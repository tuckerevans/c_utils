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

size_t rope_len(root)
rope *root;
{
	if (!root)
		return 0;

	if (root->str)
		return strlen(root->str);

	return rope_len(root->left) + rope_len(root->right);
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

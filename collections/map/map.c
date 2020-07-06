#include "map.h"

#include <string.h>
#include <stdio.h>

struct map_node {
	void *key, *val;
	cmp_func cmp;

	struct map_node *left, *right, *parent;
};

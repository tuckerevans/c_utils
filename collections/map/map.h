#ifndef MAP_H
#define MAP_H

typedef struct map_node map;
typedef int (*cmp_func)(void*, void*);

/*constructors*/
map* map_new(cmp_func);

/*management*/
int map_size(map*);

#endif

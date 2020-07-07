#ifndef MAP_H
#define MAP_H

typedef struct map_node map;
typedef int (*cmp_func)(void*, void*);

/*constructors*/
map* map_new(cmp_func);

/*management*/
int map_size(map*);

/*data*/
int map_insert(map*, void*, void*);
void* map_reset_key(map*, void*);

/*memory*/
void map_clear(map*);
void map_free(map*);
#endif

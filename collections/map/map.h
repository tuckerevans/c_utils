#ifndef MAP_H
#define MAP_H

typedef struct map_root map;
typedef int (*cmp_func)(void*, void*);

/*constructors*/
map* map_new(cmp_func);

/*management*/
int map_size(map*);

/*data*/
int map_insert(map*, void*, void*);
void* map_index(map*, void*);
void* map_set_val(map*, void*, void*);

int map_check_key_ptr(map*, void*);
void* map_set_key(map*, void*);


/*memory*/
void map_clear(map*);
void map_free(map*);
#endif

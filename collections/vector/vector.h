#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector vec;

/*constructors*/
vec* vec_new();
vec* vec_with_capacity(int);

/*management*/
int vec_size(vec*);
int vec_capacity(vec*);
vec* vec_cp(vec*);
void vec_print(vec*, char* (void*));

/*data*/
void vec_push(vec*, void*);
void* vec_pop(vec*);
void* vec_back(vec*);

void vec_set(vec*, int, void*);
void* vec_index(vec*, int);

void vec_swap(vec*, int, int);
void* vec_swap_pop(vec*, int);


/*memory*/
void vec_clear(vec*);
void vec_free(vec*);
#endif

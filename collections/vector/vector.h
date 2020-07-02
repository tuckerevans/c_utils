#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector vec;

vec* vec_new();
vec* vec_with_capacity(int);
int vec_size(vec*);
void vec_push(vec*, void*);
#endif

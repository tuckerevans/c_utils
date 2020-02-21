#ifndef VECTOR_H
#define VECTOR_H

typedef struct double_ended_queue deq;

/*constructors*/
deq* deq_new();
deq* deq_with_capacity(int);

/*management*/
int deq_size(deq*);
int deq_capacity(deq*);
deq* deq_cp(deq*);
void deq_free(deq*);
void deq_print(vec*);

/*data*/
void deq_push(deq*, void*);
void* deq_rmfirst(deq*);
void* deq_index(deq*, int);



/*
 * swap
 * resevee
 * truncate
 * front
 * back
 * push/pop front
 * push/pop back
 * swap_rm_front/back
 * insert
 * remove
 */
#endif

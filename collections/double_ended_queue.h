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

/* Note: Elements are not freed
 * deq_clear should be called before if they are no longer needed.*/
void deq_free(deq*);

/*Free all elements within queue*/
void deq_clear()

/*data*/
void deq_push_back(deq*, void*);
void* deq_pop_front(deq*);
void* deq_index(deq*, int);
void* deq_pop_back(deq*);

void deq_swap(deq*, int, int);

/*Note: Does not currently reduce memory footprint*/
void deq_truncate(deq*, int);

void* deq_front(deq*);
void* deq_back(deq*);

void remove(deq*, int);


/*
 * resevee
 * push back
 * swap_rm_front/back
 * insert
 */
#endif

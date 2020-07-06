#ifndef DOUBLE_ENDED_QUEUE_H
#define DOUBLE_ENDED_QUEUE_H

typedef struct double_ended_queue deq;

/*constructors*/
deq* deq_new();
deq* deq_with_capacity(int);

/*management*/
int deq_size(deq*);
int deq_capacity(deq*);
deq* deq_cp(deq*);
void deq_print(deq*, char* (void*));

/*data*/
void deq_push_front(deq*, void*);
void deq_push_back(deq*, void*);
void* deq_front(deq*);
void* deq_back(deq*);
void* deq_pop_front(deq*);
void* deq_pop_back(deq*);

void deq_set(deq*, int, void*);
void* deq_index(deq*, int);

void deq_insert(deq*, int, void*);
void deq_remove(deq*, int);

void deq_swap(deq*, int, int);
void* deq_swap_rm_front(deq*, int);
void* deq_swap_rm_back(deq*, int);

/*memory*/
void deq_truncate(deq*, int);
	/*Note: Does not currently reduce memory footprint*/
void deq_reserve(deq*, int);

void deq_clear(deq*);
	/*Free all elements within queue*/
void deq_free(deq*);
	/* Note: Elements are not freed
	 * deq_clear should be called before if they are no longer needed.*/

#endif

#ifndef ROPE_H
#define ROPE_H

typedef struct rope_s rope;

rope* rope_new();
rope* str_to_rope(char*);
rope* rope_concate(rope*, rope*);

#endif

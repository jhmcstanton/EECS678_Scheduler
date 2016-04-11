/** @file libpriqueue.h
 */

#ifndef LIBPRIQUEUE_H_
#define LIBPRIQUEUE_H_


/**
   Node data structure internal to the linked list implementation of our queue.
 */
typedef struct _node_t
{
    void *element;
    struct _node_t *next;
} node_t;


/**
 *  A simplified name for comparison functions
 */
typedef int(*comp_t)(const void *, const void *);

/**
 * A function the can be applied to an element of the queue. 
 */
typedef void(*map_apply_t)(void *);

/**
  Priqueue Data Structure
*/
typedef struct _priqueue_t
{
    node_t *head;
    node_t *tail;
    node_t *index;
    comp_t comparer;
    int size;
} priqueue_t;



void   priqueue_init     (priqueue_t *q, int(*comparer)(const void *, const void *));

int    priqueue_offer    (priqueue_t *q, void *ptr);
void * priqueue_peek     (priqueue_t *q);
void * priqueue_poll     (priqueue_t *q);
void * priqueue_at       (priqueue_t *q, int index);
int    priqueue_remove   (priqueue_t *q, void *ptr);
void * priqueue_remove_at(priqueue_t *q, int index);
int    priqueue_size     (priqueue_t *q);

void   priqueue_destroy  (priqueue_t *q);

void   priqueue_mut_map  (priqueue_t *q, map_apply_t ap);

#endif /* LIBPQUEUE_H_ */

/** @file libpriqueue.h
 */

#ifndef LIBPRIQUEUE_H_
#define LIBPRIQUEUE_H_

/**
  Information regarding each job for maintenance
*/
typedef struct _primeta_t
{
    int time_remaining;
    int priority;
    int arrival_time;
    int run_time;
    int job_id;
} primeta_t;


/**
   Node data structure internal to the linked list implementation of our queue.
 */
typedef struct _node_t
{
    void *element;
    struct _node_t *next;
} node_t;


typedef int(*comp_t)(const void *, const void *);

/**
  Priqueue Data Structure
*/
typedef struct _priqueue_t
{
    //primeta_t *jobs;
    node_t *head;
    node_t *tail;
    node_t *index;
    //    int(*comparer)(const void *, const void *);
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

#endif /* LIBPQUEUE_H_ */

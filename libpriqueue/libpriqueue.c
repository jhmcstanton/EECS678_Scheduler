/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.
  
  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
//void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
void priqueue_init(priqueue_t *q, comp_t comparer)
{
    q->head     = NULL;
    q->tail     = q->head;
    q->index    = q->head;
    q->comparer = comparer;
    q->size     = 0;
}

/**
   Inserts element into internal linked list.
 */
int insert_with(node_t *head, node_t* new_elem, comp_t comparer){
    // the current node has >= priority to the next element, new_element recurses
    if(comparer(head->element, new_elem->element) >= 0){
	// there's nothing left, slap it on the end
	if(head->next == NULL) {
	    head->next = new_elem;
	    return 1;
	} else { // there is something left, more comparisons need to be done
	    return 1 + insert_with(head->next, new_elem, comparer);    
	}
    } else { // the new_elem has highest priority, displace the current head of the queue
	new_elem->next = head;
	head           = new_elem;
	return 0;
    }
}

/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
    int index = 0;
    node_t *new_elem = (node_t *) malloc(1 * sizeof(node_t));
    new_elem->element = ptr;
    new_elem->next    = NULL;
    if(q->size == 0){
	q->head = new_elem;
	q->head->next = NULL;	
    } else {
	index = insert_with(q->head, new_elem, q->comparer);
    }
    q->size++;
    return index;
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
	return NULL;
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
	return NULL;
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
	return NULL;
}


/**
  Removes all instances of ptr from the queue. 
  
  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
	return 0;
}


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
	return 0;
}


/**
  Returns the number of elements in the queue.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
	return 0;
}


/**
  Destroys and frees all the memory associated with q.
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{

}

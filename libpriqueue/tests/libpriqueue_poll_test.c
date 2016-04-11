#include "libpriqueue_poll_test.h"
#include "../../test_utils.h"
#include "../libpriqueue.h"
#include <stdio.h>

int post_comparer(const void* a, const void* b){
    return *(int*) a < *(int*)b;
}

bool poll_no_element_test(){
    priqueue_t q;
    int *elem;
    bool result;
    printf("Poll: No Elements\n");
    priqueue_init(&q, &post_comparer);

    elem   = priqueue_poll(&q);
    result = elem == NULL && q.size == 0;
    print_prefix(result);
    printf("elem is %s (expected NULL), size is %d (expected 0)\n", 
	   elem == NULL ? "NULL" : "NOT NULL",
	   q.size);

    priqueue_destroy(&q);    
    

    return result;
}

bool poll_one_element_test(){
    priqueue_t q;
    int *elem, single_val = 0;;
    bool result;
    char helper_buffer[10];

    printf("Poll: One Element\n");

    priqueue_init(&q, &post_comparer);
    priqueue_offer(&q, &single_val);

    elem   = priqueue_poll(&q);
    result = elem != NULL &&
	*elem == 0
	&& q.size == 0;
    
    print_prefix(result);

    if(elem == NULL){
	sprintf(helper_buffer, "NULL");
    } else {
	sprintf(helper_buffer, "%d", *elem);
    }

    printf("elem is %s (expected 0), size is %d (expected 0)\n", 
	   helper_buffer,
	   q.size);

    priqueue_destroy(&q);

    return result;
}

bool poll_one_double_test(){
    priqueue_t q;
    int *elem, single_val = 0;
    bool result;

    printf("Poll: One Element Double Poll\n");

    priqueue_init(&q, &post_comparer);

    priqueue_offer(&q, &single_val);
    priqueue_poll(&q);
    elem   = priqueue_poll(&q);
    result = elem == NULL && q.size == 0;

    print_prefix(result);
    printf("elem is %s, expected NULL, size is %d, expected 0\n",
	   elem == NULL ? "NULL": "NOT NULL",
	   q.size);

    priqueue_destroy(&q);
    return result;
}

bool poll_multiple_val_single_poll_test(){
    priqueue_t q;
    const int num_elems = 10;
    int elems[num_elems];
    int *elem, i;
    bool result;
    char helper_buffer[10];

    printf("Poll: Multiple Elements Single Remove\n");

    priqueue_init(&q, &post_comparer);
    for(i = 0; i < num_elems; i++){
        elems[i] = i;
	priqueue_offer(&q, &elems[i]);
    }
    elem   = priqueue_poll(&q);

    result = elem != NULL 
	&& *elem  == num_elems - 1
	&& q.size == num_elems - 1;

    print_prefix(result);
    if(elem == NULL){
	sprintf(helper_buffer, "NULL");
    } else {
	sprintf(helper_buffer, "%d", *elem);
    }

    printf("elem is %s (expected %d), size is %d (expected %d)\n",
	   helper_buffer, num_elems - 1,
	   q.size, num_elems - 1);

    priqueue_destroy(&q);
    return result;
}

bool poll_multiple_val_multiple_poll_test(){
    priqueue_t q;
    const int num_elems = 10;
    int elems[num_elems];
    int *elem, i, num_removes = 3;
    bool result;
    char helper_buffer[10];

    printf("Poll: Multiple Elements Multiple Remove\n");

    priqueue_init(&q, &post_comparer);
    for(i = 0; i < num_elems; i++){
	elems[i] = i;
	priqueue_offer(&q, &elems[i]);
    }
    for(i = 0; i < num_removes; i++){
	elem   = priqueue_poll(&q);
    }

    result = elem != NULL 
	&& *elem  == num_elems - num_removes  
	&& q.size == num_elems - num_removes;

    print_prefix(result);

    if(elem == NULL){
	sprintf(helper_buffer, "NULL");
    } else {
	sprintf(helper_buffer, "%d", *elem);
    }

    printf("elem is %s (expected %d), size is %d (expected %d)\n",
	   helper_buffer, num_elems - num_removes + 1, 
	   q.size, num_elems - num_removes);

    priqueue_destroy(&q);
    return result;
}

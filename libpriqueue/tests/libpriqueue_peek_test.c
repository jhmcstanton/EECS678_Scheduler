#include "libpriqueue_peek_test.h"
#include "../../test_utils.h"
#include <stdio.h>

int peek_comparer(const void* a, const void* b){
    return *(int*) a < *(int*)b;
}

bool no_element_test(){
    priqueue_t q;
    int *elem;
    bool result;
    priqueue_init(&q, &peek_comparer);

    printf("Peek: No Elements\n");
    elem = priqueue_peek(&q);
    printf("asdasd\n");
    result = (elem == NULL) && (q.size == 0);
    print_prefix(result);
    printf("elem is %s, size is %d\n", 
	   elem == NULL ? "NULL" : "NOT NULL",
	   q.size);
    
    priqueue_destroy(&q);

    // expect NULL, size 0 
    return result;
}

bool multiple_element_test(){
    priqueue_t q;
    int *elem, i, num_elems = 10;
    bool result;
    char helper_buffer[10];
    priqueue_init(&q, &peek_comparer);
    printf("Peek: %d Elements; Increasing\n", num_elems);
    for(i = 0; i < num_elems; i++){
	priqueue_offer(&q, &i);	
    }

    elem   = priqueue_peek(&q);
    result = (elem != NULL) && 
	(*elem == 10) && 
	(q.size == num_elems);

    print_prefix(result);

    if(elem != NULL){
	sprintf(helper_buffer, "%d", *elem); 
    } else {
	sprintf(helper_buffer, "NULL");
    }

    printf("elem is %s NULL, elem value is %s, size is %d\n",
	   elem != NULL ? "NOT": "",
	   helper_buffer,
	   q.size);

    priqueue_destroy(&q);
    return result;
}

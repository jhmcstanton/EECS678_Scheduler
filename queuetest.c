/** @file queuetest.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "libpriqueue/libpriqueue.h"
#include "libpriqueue/tests/libpriqueue_tests.h"
#include "test_utils.h"

int compare1(const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}

int compare2(const void * a, const void * b)
{
	return ( *(int*)b - *(int*)a );
}

void test_destroy(priqueue_t *q){
    printf("Testing destroy function\n");
    
    const int num_vals   = 3;
    int values[num_vals]; 
    int i, temp;

    printf("Destroy test 1\n");
    for(i = 0; i < num_vals; i++){
	values[i] = i;
	priqueue_offer(q, &values[i]);
    } 

    node_t *temp_q = q->head;
    i = 0;
    printf("Inspecting queue pre destroy\n");
    while(temp_q != NULL){
	i++;
	printf("Found element: %d, num found: %d\n", *(int*)(temp_q->element), i);
	temp_q = temp_q->next;
    }

    priqueue_destroy(q);

    temp_q = q->head;
    i = 0;
    printf("Inspecting queue post destroy\n");
    while(temp_q != NULL){
	i++;
	printf("Found element: %d, num found: %d\n", *(int*)(temp_q->element), i);
	temp_q = temp_q->next;
    }

    printf("Destroy test 2, empty case\n");
    priqueue_destroy(q);

    printf("Destroy test 3, all unique\n");
    for(i = 0; i < num_vals; i++){
	priqueue_offer(q, &i);
    } 
    priqueue_destroy(q);
    
    printf("Finished destroy test\n");
}

void print_queue(priqueue_t *q){
    node_t *cursor = q->head;
    int index = 0;
    printf("Trying to print queue\n");
    while(cursor != NULL){
	if(cursor->element == NULL){
	    printf("adasdqweasd\n");
	}
	printf("index : %d, elem: %d\n", index, *(int*)cursor->element);
	index++;
	cursor = cursor->next;
    }
    printf("Printed the queue\n");
}

int main()
{
    printf("Running new unit tests\n");

    int i, success_count = 0;
    for(i = 0; i < NUM_TESTS; i++){
	printf("Test %d\n", i+1);
	if(queue_tests[i]()){
	    success_count++;
	}
    }

    printf("%s Total Number of Tests    : %s%d\n", KBLU, KWHT, NUM_TESTS);
    printf("%s Total Number of Successes: %s%d%s\n", 
	   KBLU, 
	   success_count == NUM_TESTS ? KWHT : KRED, 
	   NUM_TESTS, RESET);

    printf("Done with unit tests, running provided tests \n\n\n");

	priqueue_t q, q2;

	priqueue_init(&q, compare1);
	priqueue_init(&q2, compare2);

	printf("New tests \n");

	//	test_destroy(&q);
	
	printf("Provided tests starting ... \n");

	/* Pupulate some data... */
	int *values = malloc(100 * sizeof(int));

	for (i = 0; i < 100; i++)
		values[i] = i;

	/* Add 5 values, 3 unique. */
	priqueue_offer(&q, &values[12]);
	priqueue_offer(&q, &values[13]);
	priqueue_offer(&q, &values[14]);
	priqueue_offer(&q, &values[12]);
	priqueue_offer(&q, &values[12]);
	printf("Total elements: %d (expected 5).\n", priqueue_size(&q));

	print_queue(&q);

	int val = *((int *)priqueue_poll(&q));
	printf("Top element: %d (expected 12).\n", val);
	printf("Total elements: %d (expected 4).\n", priqueue_size(&q));	

	print_queue(&q);

	int vals_removed = priqueue_remove(&q, &values[12]);	
	printf("Elements removed: %d (expected 2).\n", vals_removed);
	printf("Total elements: %d (expected 2).\n", priqueue_size(&q));

	print_queue(&q);

	printf("Adding to q\n");
	priqueue_offer(&q, &values[10]);
	print_queue(&q);
	priqueue_offer(&q, &values[30]);
	print_queue(&q);
	priqueue_offer(&q, &values[20]);
	print_queue(&q);

	printf("Adding to q2\n");
	priqueue_offer(&q2, &values[10]);
	priqueue_offer(&q2, &values[30]);
	priqueue_offer(&q2, &values[20]);


	printf("Elements in order queue (expected 10 13 14 20 30): ");
	for (i = 0; i < priqueue_size(&q); i++)
		printf("%d ", *((int *)priqueue_at(&q, i)) );
	printf("\n");

	printf("Elements in reverse order queue (expected 30 20 10): ");
	for (i = 0; i < priqueue_size(&q2); i++)
		printf("%d ", *((int *)priqueue_at(&q2, i)) );
	printf("\n");

	printf("destroy 1\n");
	priqueue_destroy(&q);
	printf("destroy 2\n");
	priqueue_destroy(&q2);

	free(values);

	return 0;
}

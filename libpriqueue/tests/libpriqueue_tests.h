#ifndef ALL_LIBPRIQUEUE_TESTS_H_
#define ALL_LIBPRIQUEUE_TESTS_H_

#include "../../test_utils.h"
#include "libpriqueue_peek_test.h"


#define NUM_TESTS 2
unit_test queue_tests[NUM_TESTS];
unit_test queue_tests[NUM_TESTS] = {
    no_element_test,
    multiple_element_test
    };

#endif

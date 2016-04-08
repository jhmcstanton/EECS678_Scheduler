#ifndef ALL_LIBPRIQUEUE_TESTS_H_
#define ALL_LIBPRIQUEUE_TESTS_H_

#include "../../test_utils.h"
#include "libpriqueue_peek_test.h"
#include "libpriqueue_poll_test.h"


#define NUM_TESTS 7
unit_test queue_tests[NUM_TESTS];
unit_test queue_tests[NUM_TESTS] = {
peek_no_element_test,
    peek_multiple_element_test,
    poll_no_element_test,
    poll_one_element_test,
    poll_one_double_test,
    poll_multiple_val_single_poll_test,
    poll_multiple_val_multiple_poll_test,
    };

#endif

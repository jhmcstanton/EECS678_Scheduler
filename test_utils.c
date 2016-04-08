#include <stdio.h>
#include "test_utils.h"

/**
   Prints a suffix to a test case result

   @param result is the result of the test, used
          to determine what color and suffix test to use   
 */
void print_prefix(bool result){
    if(result){
	printf("%s%s " RESET, KGRN, "[SUCCESS]:");
    }else {
	printf("%s%s " RESET, KRED, "[FAILURE]:");
    }
}


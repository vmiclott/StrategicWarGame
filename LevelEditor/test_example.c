#include "min_unit.h"

#include "test_example.h"

#include <stdlib.h>
#include <stdio.h> 

#ifdef _WIN32
#include <process.h>
#endif


static char* test_example_pass()
{
    mu_assert(true);

    return NULL;
}

static char* test_example_fail()
{
    mu_assert(false);

    return NULL;
}


static char* all_tests()
{
    mu_run_test(test_example_pass);
    mu_run_test(test_example_fail);

    return NULL;
}


char* all_tests_example()
{
    printf("-------------------------------------------------------------------------------\n");
    printf("--- EXAMPLE -------------------------------------------------------------------\n");
    printf("-------------------------------------------------------------------------------\n\n");
    printf("Running tests:\n\n");
    char *result = all_tests();
    if (result != NULL) {
        printf("%s\n", result);
        printf("\nAt least one test failed\n\n");
    }
    else {
        printf("\nAll tests passed\n\n");
    }
    printf("\n");

    return result;
}
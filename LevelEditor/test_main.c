#include "min_unit.h"

#include "test_example.h"
#include "test_common.h"
#include "test_level.h"
#include "test_pqueue.h"
#include "test_dwgraph.h"
#include "test_pathfinder.h"
#include "debug_file.h"

#include <stdlib.h>
#include <stdio.h> 

#ifdef _WIN32
#include <process.h>
#endif


static char* all_tests()
{
    char* result_example = NULL;
    char* result_common = NULL;
    char* result_level = NULL;
    char* result_pqueue = NULL;
    char* result_dwgraph = NULL;
    char* result_pathfinder = NULL;

    //result_example = all_tests_example();
    result_common = all_tests_common();
    result_level = all_tests_level();
    result_pqueue = all_tests_pqueue();
    result_dwgraph = all_tests_dwgraph();
    result_pathfinder = all_tests_pathfinder();

    if (   result_example == NULL
        && result_common == NULL
        && result_level == NULL
        && result_pqueue == NULL
        && result_dwgraph == NULL
        && result_pathfinder == NULL)
    {
        return NULL;
    }
    else
    {
        return "failed";
    }
}


int main(int argc, char **argv)
{
    // automatically check for memory leaks
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    char *result = all_tests();

    printf("*******************************************************************************\n\n");
    printf("RESULT: %d TESTS RUN\n\n", mu_tests_run);
    if (result == NULL)
    {
        printf("        ALL TESTS PASSED\n\n");
    }
    else
    {
        printf("        AT LEAST ONE TEST FAILED\n\n");
    }
    printf("*******************************************************************************\n\n");

#ifdef _WIN32
    system("pause");
#endif

    return result != NULL;
}


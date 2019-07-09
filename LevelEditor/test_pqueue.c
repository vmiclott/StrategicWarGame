#include "min_unit.h"

#include "test_pqueue.h"
#include "pqueue.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h> 

#ifdef _WIN32
#include <process.h>
#endif


/*** Function: pqueue_alloc() *************************************************/

static char* test_pqueue_alloc()
{
    PriorityQueue* pqueue = pqueue_alloc();

    mu_assert(pqueue != NULL);
    mu_assert(pqueue->items != NULL);
    mu_assert(pqueue->item_count == 0);
    mu_assert(pqueue->item_max_count >= 0);

    pqueue_free(pqueue);

    return NULL;
}


/*** Function: pqueue_update() *************************************************/

static char* test_pqueue_update()
{
    PriorityQueue* pqueue = pqueue_alloc();
    char item1 = 'a';

    pqueue_update(pqueue, &item1, 10);
    
    mu_assert(pqueue->item_count == 1);
    mu_assert(pqueue_remove_first(pqueue) == &item1);

    pqueue_free(pqueue);

    return NULL;
}

static char* test_pqueue_update_many_items()
{
    int strlen = 26 * 5;
    char* string = allocate_strlen(strlen);

    for (int i = 0; i < strlen; i++)
    {
        string[i] = 'a' + (i % 26);
    }

    PriorityQueue* pqueue = pqueue_alloc();

    for (int j = 0; j < strlen; j++)
    {
        pqueue_update(pqueue, &string[j], j % 26);
    }

    mu_assert(pqueue->item_count == strlen);

    char* temp_item;
    for (int k = 0; k < 26; k++)
    {
        for (int l = 0; l < 5; l++)
        {
            temp_item = pqueue_remove_first(pqueue);
            mu_assert((char)*temp_item == 'a' + k);
        }
    }

    free(string);
    pqueue_free(pqueue);

    return NULL;
}


/*** Run tests ****************************************************************/

static char* all_tests()
{
    mu_run_test(test_pqueue_alloc);
    mu_run_test(test_pqueue_update);
    mu_run_test(test_pqueue_update_many_items);

    return NULL;
}


char* all_tests_pqueue()
{
    printf("-------------------------------------------------------------------------------\n");
    printf("--- PQUEUE --------------------------------------------------------------------\n");
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
#include "min_unit.h"

#include "test_common.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h> 

#ifdef _WIN32
#include <process.h>
#endif


/*** Function: cell_type_is_unit() ********************************************/

static char* test_cell_type_is_unit()
{
    mu_assert(cell_type_is_unit(GROUND) == 0);
    mu_assert(cell_type_is_unit(WATER) == 0);
    mu_assert(cell_type_is_unit(ROCK) == 0);
    mu_assert(cell_type_is_unit(HEADQUARTER) == 0);
    mu_assert(cell_type_is_unit(BRIDGE) == 0);
    mu_assert(cell_type_is_unit(UNIT_1) == 1);
    mu_assert(cell_type_is_unit(UNIT_2) == 1);
    mu_assert(cell_type_is_unit(UNIT_3) == 1);

    return NULL;
}


/*** Function: cell_type_is_player_owned() ************************************/

static char* test_cell_type_is_player_owned()
{
    mu_assert(cell_type_is_player_owned(GROUND) == 0);
    mu_assert(cell_type_is_player_owned(WATER) == 0);
    mu_assert(cell_type_is_player_owned(ROCK) == 0);
    mu_assert(cell_type_is_player_owned(HEADQUARTER) == 1);
    mu_assert(cell_type_is_player_owned(BRIDGE) == 0);
    mu_assert(cell_type_is_player_owned(UNIT_1) == 1);
    mu_assert(cell_type_is_player_owned(UNIT_2) == 1);
    mu_assert(cell_type_is_player_owned(UNIT_3) == 1);

    return NULL;
}


/*** Function: pos_init() *****************************************************/

static char* test_pos_init()
{
    int row = 3;
    int col = 5;

    Pos pos = pos_init(row, col);

    mu_assert(pos.row == row);
    mu_assert(pos.col == col);

    return NULL;
}


/*** Function: path_alloc() ***************************************************/

static char* test_path_alloc()
{
    int step_count = 5;
    int distance = 8;

    Path* path = path_alloc(step_count, distance);

    // test if path has been initialized correctly
    mu_assert(path != NULL);
    mu_assert(path->steps != NULL);
    mu_assert(path->step_count == step_count);
    mu_assert(path->distance == distance);

    // test if steps array has been allocated correctly
    int i;
    for (i = 0; i < step_count; i++)
    {
        path->steps[i] = pos_init(i+1, 5);
    }
    mu_assert(path->steps[4].row == 5);

    path_free(path);

    return NULL;
}


/*** Function: path_free() ****************************************************/

// #TODO: is het überhaupt mogelijk om free() te testen?


/*** Function: allocate_strlen() **********************************************/

static char* test_allocate_strlen()
{
    int strlen = 50;

    char* string = allocate_strlen(strlen);

    // test if string has been allocated correctly
    for (int i = 0; i < strlen; i++)
    {
        string[i] = 'A';
    }
    mu_assert(string[strlen - 1] == 'A');
    mu_assert(string[strlen] == '\0');

    free(string);

    return NULL;
}


/*** Run tests ****************************************************************/

static char* all_tests()
{
    mu_run_test(test_cell_type_is_unit);
    mu_run_test(test_cell_type_is_player_owned);
    mu_run_test(test_pos_init);
    mu_run_test(test_path_alloc);
    mu_run_test(test_allocate_strlen);

    return NULL;
}


char* all_tests_common()
{
    printf("-------------------------------------------------------------------------------\n");
    printf("--- COMMON --------------------------------------------------------------------\n");
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
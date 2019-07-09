#include "min_unit.h"

#include "test_dwgraph.h"
#include "test_level.h"
#include "dwgraph.h"
#include "gui.h"

#include <stdlib.h>
#include <stdio.h> 

#include <allegro5/allegro.h>

#ifdef _WIN32
#include <process.h>
#endif


/*** Function: create_empty_dwgraph() *****************************************/

static char* test_create_empty_dwgraph()
{
    int amount_of_nodes = 10;

    DWGraph* graph = create_empty_dwgraph(amount_of_nodes);

    // test if graph is correctly initialized
    mu_assert(graph != NULL);
    mu_assert(graph->amount_of_nodes == amount_of_nodes);

    // test if node array is correctly initialized
    mu_assert(graph->node_array != NULL);
    int i;
    for (i = 0; i < graph->amount_of_nodes; i++)
    {
        mu_assert(graph->node_array[i].amount_of_neighbors == 0);
    }

    free_dwgraph(graph);

    return NULL;
}


/*** Function: create_dwgraph() ***********************************************/

static char* test_create_dwgraph_empty_level()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);

    DWGraph* graph = create_dwgraph(level);

    // test if graph is correctly initialized
    mu_assert(graph != NULL);
    mu_assert(graph->level == level);
    mu_assert(graph->amount_of_nodes == level->width * level->height);

    // test if node array is correctly initialized
    mu_assert(graph->node_array != NULL);
    int i;
    for (i = 0; i < graph->amount_of_nodes; i++)
    {
        mu_assert(graph->node_array[i].cell == &level->cells[i / level->width][i % level->width]);
    }

    free_dwgraph(graph);
    level_free(level);

    return NULL;
}

static char* test_create_dwgraph_non_empty_level()
{
    char* level_name = "test-around-obstacles.world";

    ALLEGRO_PATH* file_path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(file_path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(file_path, level_name);
    const char* filename = al_path_cstr(file_path, ALLEGRO_NATIVE_PATH_SEP);

    Level* level = level_alloc_read_from_file(filename);

    DWGraph* graph = create_dwgraph(level);

    // test if graph is correctly initialized
    mu_assert(graph != NULL);
    mu_assert(graph->level == level);
    mu_assert(graph->amount_of_nodes == level->width * level->height);

    // test if node array is correctly initialized
    mu_assert(graph->node_array != NULL);
    int i;
    for (i = 0; i < graph->amount_of_nodes; i++)
    {
        mu_assert(graph->node_array[i].cell == &level->cells[i / level->width][i % level->width]);
    }

    free_dwgraph(graph);
    al_destroy_path(file_path);
    level_free(level);

    return NULL;
}


/*** Run tests ****************************************************************/

static char* all_tests()
{
    mu_run_test(test_create_empty_dwgraph);
    mu_run_test(test_create_dwgraph_empty_level);
    mu_run_test(test_create_dwgraph_non_empty_level);

    return NULL;
}


char* all_tests_dwgraph()
{
    printf("-------------------------------------------------------------------------------\n");
    printf("--- DWGRAPH -------------------------------------------------------------------\n");
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
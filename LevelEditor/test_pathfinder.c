#include "min_unit.h"

#include "test_pathfinder.h"
#include "test_level.h"
#include "pathfinder.h"
#include "level.h"
#include "gui.h"

#include <stdlib.h>
#include <stdio.h> 

#include <allegro5/allegro.h>

#ifdef _WIN32
#include <process.h>
#endif


/*** Helper functions *********************************************************/

static int test_is_path_start_and_end_step_correct(Path* path, Cell* start_cell, Cell* target_cell)
{
    return path->steps[0].row == start_cell->row
        && path->steps[0].col == start_cell->col
        && path->steps[path->step_count - 1].row == target_cell->row
        && path->steps[path->step_count - 1].col == target_cell->col;
}

static int test_is_path_walkable(Path* path, Level* level)
{
    // test if all steps are over terrain that unit in start_cell can walk over
    Cell start_cell = level->cells[path->steps[0].row][path->steps[0].col];
    Cell step_cell;
    int i;
    for (i = 0; i < path->step_count; i++)
    {
        step_cell = level->cells[path->steps[i].row][path->steps[i].col];
        if (!level_can_walk_over(&start_cell, &step_cell))
        {
            return 0;
        }
    }

    return 1;
}


/*** Function: find_path() ****************************************************/

static char* test_find_path_empty_level_single_step()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[3][3];
    Cell* target_cell = start_cell;

    Path* path = find_path(level, start_cell, target_cell);

    // test path
    mu_assert(path != NULL);
    mu_assert(path->distance == 0);
    mu_assert(path->step_count == 1);

    // test single step
    mu_assert(path->steps != NULL);
    mu_assert(path->steps[0].row == start_cell->row);
    mu_assert(path->steps[0].col == start_cell->col);

    path_free(path);
    level_free(level);

    return NULL;
}

static char* test_find_path_empty_level_horizontal()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[2][1];
    Cell* target_cell = &level->cells[2][17];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path != NULL);
    mu_assert(path->distance == 12 * (17 - 1));
    mu_assert(path->step_count == 17);

    mu_assert(path->steps != NULL);
    mu_assert(test_is_path_start_and_end_step_correct(path, start_cell, target_cell));
    mu_assert(test_is_path_walkable(path, level) == 1);

    int i;
    for (i = 1; i < path->step_count; i++)
    {
        mu_assert(path->steps[i].row == start_cell->row);
        mu_assert(path->steps[i].col == start_cell->col + i);
    }

    path_free(path);
    level_free(level);

    return NULL;
}

static char* test_find_path_empty_level_vertical()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[1][5];
    Cell* target_cell = &level->cells[10][5];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path != NULL);
    mu_assert(path->distance == 12 * (10 - 1));
    mu_assert(path->step_count == 10);

    mu_assert(path->steps != NULL);
    mu_assert(test_is_path_start_and_end_step_correct(path, start_cell, target_cell));
    mu_assert(test_is_path_walkable(path, level) == 1);

    int i;
    for (i = 1; i < path->step_count; i++)
    {
        mu_assert(path->steps[i].row == start_cell->row + i);
        mu_assert(path->steps[i].col == start_cell->col);
    }

    path_free(path);
    level_free(level);

    return NULL;
}

static char* test_find_path_empty_level_diagonal()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[1][2];
    Cell* target_cell = &level->cells[9][10];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path != NULL);
    mu_assert(path->distance == 17 * (9 - 1));
    mu_assert(path->step_count == 9);

    mu_assert(path->steps != NULL);
    mu_assert(test_is_path_start_and_end_step_correct(path, start_cell, target_cell));
    mu_assert(test_is_path_walkable(path, level) == 1);

    int i;
    for (i = 1; i < path->step_count; i++)
    {
        mu_assert(path->steps[i].row == start_cell->row + i);
        mu_assert(path->steps[i].col == start_cell->col + i);
    }

    path_free(path);
    level_free(level);

    return NULL;
}

static char* test_find_path_empty_level_short()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[3][3];
    Cell* target_cell = &level->cells[4][5];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path != NULL);
    mu_assert(path->distance == 12 + 17);
    mu_assert(path->step_count == 3);

    mu_assert(path->steps != NULL);
    mu_assert(test_is_path_start_and_end_step_correct(path, start_cell, target_cell));
    mu_assert(test_is_path_walkable(path, level) == 1);

    int i;
    for (i = 1; i < path->step_count - 1; i++)
    {
        mu_assert(path->steps[i].row >= start_cell->row);
        mu_assert(path->steps[i].col >= start_cell->col);
        mu_assert(path->steps[i].row <= target_cell->row);
        mu_assert(path->steps[i].col <= target_cell->col);
    }

    path_free(path);
    level_free(level);

    return NULL;
}


static char* test_find_path_empty_level_short_reverse()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[4][5];
    Cell* target_cell = &level->cells[3][3];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path != NULL);
    mu_assert(path->distance == 12 + 17);
    mu_assert(path->step_count == 3);

    mu_assert(path->steps != NULL);
    mu_assert(test_is_path_start_and_end_step_correct(path, start_cell, target_cell));
    mu_assert(test_is_path_walkable(path, level) == 1);

    int i;
    for (i = 1; i < path->step_count - 1; i++)
    {
        mu_assert(path->steps[i].row <= start_cell->row);
        mu_assert(path->steps[i].col <= start_cell->col);
        mu_assert(path->steps[i].row >= target_cell->row);
        mu_assert(path->steps[i].col >= target_cell->col);
    }

    path_free(path);
    level_free(level);

    return NULL;
}


static char* test_find_path_empty_level_long()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[1][1];
    Cell* target_cell = &level->cells[10][23];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path != NULL);
    mu_assert(path->distance == 12 * 13 + 17 * 9);
    mu_assert(path->step_count == 23);

    mu_assert(path->steps != NULL);
    mu_assert(test_is_path_start_and_end_step_correct(path, start_cell, target_cell));
    mu_assert(test_is_path_walkable(path, level) == 1);

    int i;
    for (i = 1; i < path->step_count - 1; i++)
    {
        mu_assert(path->steps[i].row >= start_cell->row);
        mu_assert(path->steps[i].col >= start_cell->col);
        mu_assert(path->steps[i].row <= target_cell->row);
        mu_assert(path->steps[i].col <= target_cell->col);
    }

    path_free(path);
    level_free(level);

    return NULL;
}

static char* test_find_path_empty_level_long_reverse()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[10][23];
    Cell* target_cell = &level->cells[1][1];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path != NULL);
    mu_assert(path->distance == 12 * 13 + 17 * 9);
    mu_assert(path->step_count == 23);

    mu_assert(path->steps != NULL);
    mu_assert(test_is_path_start_and_end_step_correct(path, start_cell, target_cell));
    mu_assert(test_is_path_walkable(path, level) == 1);

    int i;
    for (i = 1; i < path->step_count - 1; i++)
    {
        mu_assert(path->steps[i].row <= start_cell->row);
        mu_assert(path->steps[i].col <= start_cell->col);
        mu_assert(path->steps[i].row >= target_cell->row);
        mu_assert(path->steps[i].col >= target_cell->col);
    }

    path_free(path);
    level_free(level);

    return NULL;
}

static char* test_find_path_empty_level_corner_to_corner()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[11][0];
    Cell* target_cell = &level->cells[0][24];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path != NULL);
    mu_assert(path->distance == 12 * 13 + 17 * 11);
    mu_assert(path->step_count == 25);

    mu_assert(path->steps != NULL);
    mu_assert(test_is_path_start_and_end_step_correct(path, start_cell, target_cell));
    mu_assert(test_is_path_walkable(path, level) == 1);

    int i;
    for (i = 1; i < path->step_count - 1; i++)
    {
        mu_assert(path->steps[i].row <= start_cell->row);
        mu_assert(path->steps[i].col >= start_cell->col);
        mu_assert(path->steps[i].row >= target_cell->row);
        mu_assert(path->steps[i].col <= target_cell->col);
    }

    path_free(path);
    level_free(level);

    return NULL;
}

static char* test_find_path_empty_level_invalid_start()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell start_cell = { 12, 0, UNIT_1, OWNER_HUMAN };
    Cell target_cell = { 0, 0, GROUND, OWNER_NONE };

    Path* path = find_path(level, &start_cell, &target_cell);

    mu_assert(path == NULL);

    level_free(level);

    return NULL;
}

static char* test_find_path_empty_level_invalid_end()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    mu_assert(level != NULL);
    Cell start_cell = { 0, 0, UNIT_1, OWNER_HUMAN };
    Cell target_cell = { 0, 25, GROUND, OWNER_NONE };

    Path* path = find_path(level, &start_cell, &target_cell);

    mu_assert(path == NULL);

    level_free(level);

    return NULL;
}

static char* test_find_path_non_empty_level_around_obstacles()
{
    char* level_name = "test-around-obstacles.world";

    ALLEGRO_PATH* file_path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(file_path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(file_path, level_name);
    const char* filename = al_path_cstr(file_path, ALLEGRO_NATIVE_PATH_SEP);

    Level* level = level_alloc_read_from_file(filename);

    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[0][0];
    Cell* target_cell = &level->cells[11][24];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path != NULL);
    mu_assert(path->distance == 12 * 95 + 17 * 26);
    mu_assert(path->step_count == 122);

    mu_assert(path->steps != NULL);
    mu_assert(test_is_path_start_and_end_step_correct(path, start_cell, target_cell));
    mu_assert(test_is_path_walkable(path, level) == 1);

    // test a few steps
    mu_assert(path->steps[5].row == 5);
    mu_assert(path->steps[5].col == 1);
    mu_assert(path->steps[11].row == 11);
    mu_assert(path->steps[11].col == 2);
    mu_assert(path->steps[65].row == 1);
    mu_assert(path->steps[65].col == 12);
    mu_assert(path->steps[119].row == 11);
    mu_assert(path->steps[119].col == 22);

    path_free(path);
    al_destroy_path(file_path);
    level_free(level);

    return NULL;
}

static char* test_find_path_non_empty_level_unreachable_cell()
{
    char* level_name = "test-unreachable-cell.world";

    ALLEGRO_PATH* file_path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(file_path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(file_path, level_name);
    const char* filename = al_path_cstr(file_path, ALLEGRO_NATIVE_PATH_SEP);

    Level* level = level_alloc_read_from_file(filename);

    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[0][0];
    Cell* target_cell = &level->cells[1][23];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path == NULL);

    al_destroy_path(file_path);
    level_free(level);

    return NULL;
}

static char* test_find_path_non_empty_level_start_in_unwalkable_cell()
{
    char* level_name = "test-unreachable-cell.world";

    ALLEGRO_PATH* file_path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(file_path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(file_path, level_name);
    const char* filename = al_path_cstr(file_path, ALLEGRO_NATIVE_PATH_SEP);

    Level* level = level_alloc_read_from_file(filename);

    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[1][22];
    Cell* target_cell = &level->cells[0][0];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path == NULL);

    al_destroy_path(file_path);
    level_free(level);

    return NULL;
}

static char* test_find_path_non_empty_level_end_in_unwalkable_cell()
{
    char* level_name = "test-unreachable-cell.world";

    ALLEGRO_PATH* file_path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(file_path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(file_path, level_name);
    const char* filename = al_path_cstr(file_path, ALLEGRO_NATIVE_PATH_SEP);

    Level* level = level_alloc_read_from_file(filename);

    mu_assert(level != NULL);
    Cell* start_cell = &level->cells[0][0];
    Cell* target_cell = &level->cells[1][22];

    Path* path = find_path(level, start_cell, target_cell);

    mu_assert(path == NULL);

    al_destroy_path(file_path);
    level_free(level);

    return NULL;
}


/*** Run tests ****************************************************************/

static char* all_tests()
{
    mu_run_test(test_find_path_empty_level_single_step);
    mu_run_test(test_find_path_empty_level_horizontal);
    mu_run_test(test_find_path_empty_level_vertical);
    mu_run_test(test_find_path_empty_level_diagonal);
    mu_run_test(test_find_path_empty_level_short);
    mu_run_test(test_find_path_empty_level_short_reverse);
    mu_run_test(test_find_path_empty_level_long);
    mu_run_test(test_find_path_empty_level_long_reverse);
    mu_run_test(test_find_path_empty_level_corner_to_corner);
    mu_run_test(test_find_path_empty_level_invalid_start);
    mu_run_test(test_find_path_empty_level_invalid_end);

    mu_run_test(test_find_path_non_empty_level_around_obstacles);
    mu_run_test(test_find_path_non_empty_level_unreachable_cell);
    mu_run_test(test_find_path_non_empty_level_start_in_unwalkable_cell);
    mu_run_test(test_find_path_non_empty_level_end_in_unwalkable_cell);

    return NULL;
}


char* all_tests_pathfinder()
{
    printf("-------------------------------------------------------------------------------\n");
    printf("--- PATHFINDER ----------------------------------------------------------------\n");
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
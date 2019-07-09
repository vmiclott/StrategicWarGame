#include "min_unit.h"

#include "test_level.h"

#include "level.h"
#include "gui.h"

#include <stdlib.h>
#include <stdio.h> 

#include <allegro5/allegro.h>

#ifdef _WIN32
#include <process.h>
#endif


/*** Function: cell_init() ****************************************************/

static char* test_cell_init()
{
    CellType type = UNIT_1;
    Owner owner = OWNER_HUMAN;
    int row = 3;
    int col = 5;

    Cell cell = cell_init(type, owner, row, col);

    mu_assert(cell.type == type);
    mu_assert(cell.owner == owner);
    mu_assert(cell.row == row);
    mu_assert(cell.col == col);

    return NULL;
}


/*** Function: level_symbol_to_cell_type() ************************************/

static char* test_level_symbol_to_cell_type()
{
    mu_assert(level_symbol_to_cell_type('*') == GROUND);
    mu_assert(level_symbol_to_cell_type('W') == WATER);
    mu_assert(level_symbol_to_cell_type('R') == ROCK);
    mu_assert(level_symbol_to_cell_type('B') == BRIDGE);
    mu_assert(level_symbol_to_cell_type('h') == HEADQUARTER);
    mu_assert(level_symbol_to_cell_type('H') == HEADQUARTER);
    mu_assert(level_symbol_to_cell_type('1') == UNIT_1);
    mu_assert(level_symbol_to_cell_type('2') == UNIT_2);
    mu_assert(level_symbol_to_cell_type('3') == UNIT_3);
    mu_assert(level_symbol_to_cell_type('7') == UNIT_1);
    mu_assert(level_symbol_to_cell_type('8') == UNIT_2);
    mu_assert(level_symbol_to_cell_type('9') == UNIT_3);

    return NULL;
}

static char* test_level_symbol_to_cell_type_invalid_symbol()
{
    mu_assert(level_symbol_to_cell_type('x') == -1);

    return NULL;
}


/*** Function: level_symbol_to_owner() ****************************************/

static char* test_level_symbol_to_owner()
{
    mu_assert(level_symbol_to_owner('*') == OWNER_NONE);
    mu_assert(level_symbol_to_owner('W') == OWNER_NONE);
    mu_assert(level_symbol_to_owner('R') == OWNER_NONE);
    mu_assert(level_symbol_to_owner('B') == OWNER_NONE);
    mu_assert(level_symbol_to_owner('h') == OWNER_HUMAN);
    mu_assert(level_symbol_to_owner('H') == OWNER_AI);
    mu_assert(level_symbol_to_owner('1') == OWNER_HUMAN);
    mu_assert(level_symbol_to_owner('2') == OWNER_HUMAN);
    mu_assert(level_symbol_to_owner('3') == OWNER_HUMAN);
    mu_assert(level_symbol_to_owner('7') == OWNER_AI);
    mu_assert(level_symbol_to_owner('8') == OWNER_AI);
    mu_assert(level_symbol_to_owner('9') == OWNER_AI);

    return NULL;
}

static char* test_level_symbol_to_owner_invalid_symbol()
{
    mu_assert(level_symbol_to_owner('x') == -1);

    return NULL;
}


/*** Function: cell_to_symbol() ***********************************************/

static char* test_cell_to_symbol()
{
    Cell cell_ground = { 1, 1, GROUND, OWNER_NONE };
    Cell cell_water = { 1, 1, WATER, OWNER_NONE };
    Cell cell_rock = { 1, 1, ROCK, OWNER_NONE };
    Cell cell_bridge = { 1, 1, BRIDGE, OWNER_NONE };
    Cell cell_headquarter_h = { 1, 1, HEADQUARTER, OWNER_HUMAN };
    Cell cell_headquarter_ai = { 1, 1, HEADQUARTER, OWNER_AI };
    Cell cell_unit_1_h = { 1, 1, UNIT_1, OWNER_HUMAN };
    Cell cell_unit_1_ai = { 1, 1, UNIT_1, OWNER_AI };
    Cell cell_unit_2_h = { 1, 1, UNIT_2, OWNER_HUMAN };
    Cell cell_unit_2_ai = { 1, 1, UNIT_2, OWNER_AI };
    Cell cell_unit_3_h = { 1, 1, UNIT_3, OWNER_HUMAN };
    Cell cell_unit_3_ai = { 1, 1, UNIT_3, OWNER_AI };

    mu_assert(cell_to_symbol(&cell_ground) == '*');
    mu_assert(cell_to_symbol(&cell_water) == 'W');
    mu_assert(cell_to_symbol(&cell_rock) == 'R');
    mu_assert(cell_to_symbol(&cell_bridge) == 'B');
    mu_assert(cell_to_symbol(&cell_headquarter_h) == 'h');
    mu_assert(cell_to_symbol(&cell_headquarter_ai) == 'H');
    mu_assert(cell_to_symbol(&cell_unit_1_h) == '1');
    mu_assert(cell_to_symbol(&cell_unit_1_ai) == '7');
    mu_assert(cell_to_symbol(&cell_unit_2_h) == '2');
    mu_assert(cell_to_symbol(&cell_unit_2_ai) == '8');
    mu_assert(cell_to_symbol(&cell_unit_3_h) == '3');
    mu_assert(cell_to_symbol(&cell_unit_3_ai) == '9');

    return NULL;
}

static char* test_cell_to_symbol_invalid_cell()
{
    Cell cell_invalid = { 1, 1, WATER, OWNER_HUMAN };

    mu_assert(cell_to_symbol(&cell_invalid) == 'I');

    return NULL;
}


/*** Function: cell_type_and_owner_to_symbol() ********************************/

static char* test_cell_type_and_owner_to_symbol()
{
    mu_assert(cell_type_and_owner_to_symbol(GROUND, OWNER_NONE) == '*');
    mu_assert(cell_type_and_owner_to_symbol(WATER, OWNER_NONE) == 'W');
    mu_assert(cell_type_and_owner_to_symbol(ROCK, OWNER_NONE) == 'R');
    mu_assert(cell_type_and_owner_to_symbol(BRIDGE, OWNER_NONE) == 'B');
    mu_assert(cell_type_and_owner_to_symbol(HEADQUARTER, OWNER_HUMAN) == 'h');
    mu_assert(cell_type_and_owner_to_symbol(HEADQUARTER, OWNER_AI) == 'H');
    mu_assert(cell_type_and_owner_to_symbol(UNIT_1, OWNER_HUMAN) == '1');
    mu_assert(cell_type_and_owner_to_symbol(UNIT_2, OWNER_HUMAN) == '2');
    mu_assert(cell_type_and_owner_to_symbol(UNIT_3, OWNER_HUMAN) == '3');
    mu_assert(cell_type_and_owner_to_symbol(UNIT_1, OWNER_AI) == '7');
    mu_assert(cell_type_and_owner_to_symbol(UNIT_2, OWNER_AI) == '8');
    mu_assert(cell_type_and_owner_to_symbol(UNIT_3, OWNER_AI) == '9');

    return NULL;
}

static char* test_cell_type_and_owner_to_symbol_invalid_cell_type_and_owner()
{
    mu_assert(cell_type_and_owner_to_symbol(WATER, OWNER_AI) == 'I');

    return NULL;
}


/*** Function: level_can_walk_over() ******************************************/

static char* test_level_can_walk_over()
{
    Cell cell_ground = { 1, 1, GROUND, OWNER_NONE };
    Cell cell_water = { 1, 1, WATER, OWNER_NONE };
    Cell cell_rock = { 1, 1, ROCK, OWNER_NONE };
    Cell cell_bridge = { 1, 1, BRIDGE, OWNER_NONE };
    Cell cell_headquarter_h = { 1, 1, HEADQUARTER, OWNER_HUMAN };
    Cell cell_headquarter_ai = { 1, 1, HEADQUARTER, OWNER_AI };
    Cell cell_unit_1_h = { 1, 1, UNIT_1, OWNER_HUMAN };
    Cell cell_unit_1_ai = { 1, 1, UNIT_1, OWNER_AI };
    Cell cell_unit_2_h = { 1, 1, UNIT_2, OWNER_HUMAN };
    Cell cell_unit_2_ai = { 1, 1, UNIT_2, OWNER_AI };
    Cell cell_unit_3_h = { 1, 1, UNIT_3, OWNER_HUMAN };
    Cell cell_unit_3_ai = { 1, 1, UNIT_3, OWNER_AI };

    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_ground) == 1);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_water) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_rock) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_bridge) == 1);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_headquarter_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_headquarter_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_unit_1_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_unit_1_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_unit_2_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_unit_2_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_unit_3_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_h, &cell_unit_3_ai) == 0);

    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_ground) == 1);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_water) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_rock) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_bridge) == 1);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_headquarter_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_headquarter_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_unit_1_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_unit_1_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_unit_2_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_unit_2_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_unit_3_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_1_ai, &cell_unit_3_ai) == 0);

    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_ground) == 1);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_water) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_rock) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_bridge) == 1);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_headquarter_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_headquarter_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_unit_1_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_unit_1_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_unit_2_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_unit_2_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_unit_3_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_h, &cell_unit_3_ai) == 0);

    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_ground) == 1);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_water) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_rock) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_bridge) == 1);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_headquarter_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_headquarter_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_unit_1_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_unit_1_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_unit_2_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_unit_2_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_unit_3_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_2_ai, &cell_unit_3_ai) == 0);

    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_ground) == 1);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_water) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_rock) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_bridge) == 1);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_headquarter_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_headquarter_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_unit_1_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_unit_1_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_unit_2_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_unit_2_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_unit_3_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_h, &cell_unit_3_ai) == 0);

    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_ground) == 1);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_water) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_rock) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_bridge) == 1);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_headquarter_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_headquarter_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_unit_1_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_unit_1_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_unit_2_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_unit_2_ai) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_unit_3_h) == 0);
    mu_assert(level_can_walk_over(&cell_unit_3_ai, &cell_unit_3_ai) == 0);

    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_ground) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_water) == 0);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_rock) == 0);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_bridge) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_headquarter_h) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_headquarter_ai) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_unit_1_h) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_unit_1_ai) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_unit_2_h) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_unit_2_ai) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_unit_3_h) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_h, &cell_unit_3_ai) == 1);

    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_ground) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_water) == 0);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_rock) == 0);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_bridge) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_headquarter_h) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_headquarter_ai) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_unit_1_h) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_unit_1_ai) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_unit_2_h) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_unit_2_ai) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_unit_3_h) == 1);
    mu_assert(level_can_walk_over(&cell_headquarter_ai, &cell_unit_3_ai) == 1);

    return NULL;
}


/*** Function: level_is_valid_pos() *******************************************/

static char* test_level_is_valid_pos()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);

    int row_top = 0;
    int row_middle = LEVEL_TEST_HEIGHT / 2;
    int row_bottom = LEVEL_TEST_HEIGHT - 1;
    int row_negative = -1;
    int row_out_of_bounds = LEVEL_TEST_HEIGHT;
    int col_left = 0;
    int col_middle = LEVEL_TEST_WIDTH / 2;
    int col_right = LEVEL_TEST_WIDTH - 1;
    int col_negative = -1;
    int col_out_of_bounds = LEVEL_TEST_WIDTH;

    mu_assert(level_is_valid_pos(level, row_middle, col_middle) == 1);
    mu_assert(level_is_valid_pos(level, row_top, col_left) == 1);
    mu_assert(level_is_valid_pos(level, row_top, col_middle) == 1);
    mu_assert(level_is_valid_pos(level, row_top, col_right) == 1);
    mu_assert(level_is_valid_pos(level, row_middle, col_right) == 1);
    mu_assert(level_is_valid_pos(level, row_bottom, col_right) == 1);
    mu_assert(level_is_valid_pos(level, row_bottom, col_middle) == 1);
    mu_assert(level_is_valid_pos(level, row_bottom, col_left) == 1);
    mu_assert(level_is_valid_pos(level, row_middle, col_left) == 1);
    mu_assert(level_is_valid_pos(level, row_negative, col_middle) == 0);
    mu_assert(level_is_valid_pos(level, row_out_of_bounds, col_middle) == 0);
    mu_assert(level_is_valid_pos(level, row_middle, col_negative) == 0);
    mu_assert(level_is_valid_pos(level, row_middle, col_out_of_bounds) == 0);

    level_free(level);

    return NULL;
}


/*** Function: level_alloc_empty() ********************************************/

static char* test_level_alloc_empty()
{
    Level* level = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);

    // test if level is correctly initialized
    mu_assert(level != NULL);
    mu_assert(level->width == LEVEL_TEST_WIDTH);
    mu_assert(level->height == LEVEL_TEST_HEIGHT);

    // test if cells are correctly initialized
    int i;
    int j;
    for (i = 0; i < level->height; i++) // rows
    {
        for (j = 0; j < level->width; j++) // cols
        {
            mu_assert(level->cells[i][j].row == i);
            mu_assert(level->cells[i][j].col == j);
            mu_assert(level->cells[i][j].type == GROUND);
            mu_assert(level->cells[i][j].owner == OWNER_NONE);
        }
    }

    level_free(level); 

    return NULL;
}


/*** Function: level_alloc_read_from_file() ***********************************/

static char* test_level_alloc_read_from_file()
{
    char* level_name = "test.world";

    ALLEGRO_PATH* path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(path, level_name);
    const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

    Level* level = level_alloc_read_from_file(filename);

    // test if level is correctly initialized & read from file
    mu_assert(level != NULL);
    mu_assert(level->width == 11);
    mu_assert(level->height == 10);

    // test if cells are correctly initialized
    int i;
    int j;
    for (i = 0; i < level->height; i++) // rows
    {
        for (j = 0; j < level->width; j++) // cols
        {
            mu_assert(level->cells[i][j].row == i);
            mu_assert(level->cells[i][j].col == j);
        }
    }

    // test if cells are correctly read from file
    // row 1
    mu_assert(level->cells[0][0].owner == OWNER_NONE);
    mu_assert(level->cells[0][0].type == ROCK);
    mu_assert(level->cells[0][1].owner == OWNER_NONE);
    mu_assert(level->cells[0][1].type == WATER);
    mu_assert(level->cells[0][2].owner == OWNER_NONE);
    mu_assert(level->cells[0][2].type == BRIDGE);
    mu_assert(level->cells[0][3].owner == OWNER_NONE);
    mu_assert(level->cells[0][3].type == WATER);
    mu_assert(level->cells[0][4].owner == OWNER_NONE);
    mu_assert(level->cells[0][4].type == GROUND);
    mu_assert(level->cells[0][5].owner == OWNER_NONE);
    mu_assert(level->cells[0][5].type == GROUND);
    mu_assert(level->cells[0][6].owner == OWNER_NONE);
    mu_assert(level->cells[0][6].type == GROUND);
    mu_assert(level->cells[0][7].owner == OWNER_NONE);
    mu_assert(level->cells[0][7].type == GROUND);
    mu_assert(level->cells[0][8].owner == OWNER_NONE);
    mu_assert(level->cells[0][8].type == GROUND);
    mu_assert(level->cells[0][9].owner == OWNER_NONE);
    mu_assert(level->cells[0][9].type == GROUND);
    mu_assert(level->cells[0][10].owner == OWNER_NONE);
    mu_assert(level->cells[0][10].type == GROUND);
    // row 2
    mu_assert(level->cells[1][0].owner == OWNER_HUMAN);
    mu_assert(level->cells[1][0].type == UNIT_1);
    mu_assert(level->cells[1][1].owner == OWNER_HUMAN);
    mu_assert(level->cells[1][1].type == UNIT_2);
    mu_assert(level->cells[1][2].owner == OWNER_HUMAN);
    mu_assert(level->cells[1][2].type == UNIT_3);
    mu_assert(level->cells[1][3].owner == OWNER_HUMAN);
    mu_assert(level->cells[1][3].type == HEADQUARTER);
    mu_assert(level->cells[1][4].owner == OWNER_HUMAN);
    mu_assert(level->cells[1][4].type == HEADQUARTER);
    mu_assert(level->cells[1][5].owner == OWNER_NONE);
    mu_assert(level->cells[1][5].type == GROUND);
    mu_assert(level->cells[1][6].owner == OWNER_NONE);
    mu_assert(level->cells[1][6].type == GROUND);
    mu_assert(level->cells[1][7].owner == OWNER_NONE);
    mu_assert(level->cells[1][7].type == GROUND);
    mu_assert(level->cells[1][8].owner == OWNER_NONE);
    mu_assert(level->cells[1][8].type == GROUND);
    mu_assert(level->cells[1][9].owner == OWNER_NONE);
    mu_assert(level->cells[1][9].type == GROUND);
    mu_assert(level->cells[1][10].owner == OWNER_NONE);
    mu_assert(level->cells[1][10].type == GROUND);
    // row 3
    mu_assert(level->cells[2][0].owner == OWNER_AI);
    mu_assert(level->cells[2][0].type == HEADQUARTER);
    mu_assert(level->cells[2][1].owner == OWNER_AI);
    mu_assert(level->cells[2][1].type == HEADQUARTER);
    mu_assert(level->cells[2][2].owner == OWNER_NONE);
    mu_assert(level->cells[2][2].type == GROUND);
    mu_assert(level->cells[2][3].owner == OWNER_HUMAN);
    mu_assert(level->cells[2][3].type == HEADQUARTER);
    mu_assert(level->cells[2][4].owner == OWNER_HUMAN);
    mu_assert(level->cells[2][4].type == HEADQUARTER);
    mu_assert(level->cells[2][5].owner == OWNER_NONE);
    mu_assert(level->cells[2][5].type == GROUND);
    mu_assert(level->cells[2][6].owner == OWNER_NONE);
    mu_assert(level->cells[2][6].type == GROUND);
    mu_assert(level->cells[2][7].owner == OWNER_NONE);
    mu_assert(level->cells[2][7].type == GROUND);
    mu_assert(level->cells[2][8].owner == OWNER_NONE);
    mu_assert(level->cells[2][8].type == GROUND);
    mu_assert(level->cells[2][9].owner == OWNER_NONE);
    mu_assert(level->cells[2][9].type == GROUND);
    mu_assert(level->cells[2][10].owner == OWNER_NONE);
    mu_assert(level->cells[2][10].type == GROUND);
    // row 4
    mu_assert(level->cells[3][0].owner == OWNER_AI);
    mu_assert(level->cells[3][0].type == HEADQUARTER);
    mu_assert(level->cells[3][1].owner == OWNER_AI);
    mu_assert(level->cells[3][1].type == HEADQUARTER);
    mu_assert(level->cells[3][2].owner == OWNER_AI);
    mu_assert(level->cells[3][2].type == UNIT_1);
    mu_assert(level->cells[3][3].owner == OWNER_AI);
    mu_assert(level->cells[3][3].type == UNIT_2);
    mu_assert(level->cells[3][4].owner == OWNER_AI);
    mu_assert(level->cells[3][4].type == UNIT_3);
    mu_assert(level->cells[3][5].owner == OWNER_NONE);
    mu_assert(level->cells[3][5].type == GROUND);
    mu_assert(level->cells[3][6].owner == OWNER_NONE);
    mu_assert(level->cells[3][6].type == GROUND);
    mu_assert(level->cells[3][7].owner == OWNER_NONE);
    mu_assert(level->cells[3][7].type == GROUND);
    mu_assert(level->cells[3][8].owner == OWNER_NONE);
    mu_assert(level->cells[3][8].type == GROUND);
    mu_assert(level->cells[3][9].owner == OWNER_NONE);
    mu_assert(level->cells[3][9].type == GROUND);
    mu_assert(level->cells[3][10].owner == OWNER_NONE);
    mu_assert(level->cells[3][10].type == GROUND);
    // other rows
    int k;
    int l;
    for (k = 4; k < level->height; k++) // rows
    {
        for (l = 0; l < level->width; l++) // cols
        {
            mu_assert(level->cells[k][l].owner == OWNER_NONE);
            mu_assert(level->cells[k][l].type == GROUND);
        }
    }

    al_destroy_path(path);
    level_free(level);

    return NULL;
}

static char* test_level_alloc_read_from_file_read_invalid_file()
{
    char* level_name = "test-invalid-level.world";

    ALLEGRO_PATH* path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(path, level_name);
    const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

    Level* level = level_alloc_read_from_file(filename);

    mu_assert(level == NULL);

    al_destroy_path(path);
    level_free(level);

    return NULL;
}

static char* test_level_alloc_read_from_file_read_invalid_dimensions()
{
    char* level_name = "test-invalid-dimensions.world";

    ALLEGRO_PATH* path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(path, level_name);
    const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

    Level* level = level_alloc_read_from_file(filename);

    mu_assert(level == NULL);

    al_destroy_path(path);
    level_free(level);

    return NULL;
}

static char* test_level_alloc_read_from_file_read_invalid_level_size_for_dimensions()
{
    char* level_name = "test-invalid-level-size-for-dimensions.world";

    ALLEGRO_PATH* path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(path, level_name);
    const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

    Level* level = level_alloc_read_from_file(filename);

    mu_assert(level == NULL);

    al_destroy_path(path);
    level_free(level);

    return NULL;
}


/*** Function: level_free() ***************************************************/

// #TODO?


/*** Function: level_write_to_file() ******************************************/

static char* test_level_write_to_file()
{
    char* level_name = "test-write.world";

    ALLEGRO_PATH* path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(path, level_name);
    const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

    // create new test level
    Level* level_new = level_alloc_empty(LEVEL_TEST_WIDTH, LEVEL_TEST_HEIGHT);
    level_new->cells[3][4] = cell_init(UNIT_1, OWNER_HUMAN, 3, 4);

    // write level to file
    level_write_to_file(level_new, filename);
    level_free(level_new);

    // read level from file 
    // (this function is assumed to work correctly
    // because it has been tested earlier)
    Level* level = level_alloc_read_from_file(filename);

    // test if level was correctly written
    mu_assert(level != NULL);
    mu_assert(level->width == LEVEL_TEST_WIDTH);
    mu_assert(level->height == LEVEL_TEST_HEIGHT);

    // test if cells were correctly written
    int i;
    int j;
    for (i = 0; i < level->height; i++) // rows
    {
        for (j = 0; j < level->width; j++) // cols
        {
            mu_assert(level->cells[i][j].row == i);
            mu_assert(level->cells[i][j].col == j);
        }
    }

    mu_assert(level->cells[0][0].type == GROUND);
    mu_assert(level->cells[0][0].owner == OWNER_NONE);
    mu_assert(level->cells[3][4].type == UNIT_1);
    mu_assert(level->cells[3][4].owner == OWNER_HUMAN);

    al_destroy_path(path);
    level_free(level);

    return NULL;
}


/*** Run tests ****************************************************************/

static char* all_tests()
{
    mu_run_test(test_cell_init);
    mu_run_test(test_level_symbol_to_cell_type);
    mu_run_test(test_level_symbol_to_cell_type_invalid_symbol);
    mu_run_test(test_level_symbol_to_owner);
    mu_run_test(test_level_symbol_to_owner_invalid_symbol);
    mu_run_test(test_cell_to_symbol);
    mu_run_test(test_cell_to_symbol_invalid_cell);
    mu_run_test(test_cell_type_and_owner_to_symbol);
    mu_run_test(test_cell_type_and_owner_to_symbol_invalid_cell_type_and_owner);
    mu_run_test(test_level_can_walk_over);
    mu_run_test(test_level_is_valid_pos);
    mu_run_test(test_level_alloc_empty);
    mu_run_test(test_level_alloc_read_from_file);
    mu_run_test(test_level_alloc_read_from_file_read_invalid_file);
    mu_run_test(test_level_alloc_read_from_file_read_invalid_dimensions);
    mu_run_test(test_level_alloc_read_from_file_read_invalid_level_size_for_dimensions);
    mu_run_test(test_level_write_to_file);

    return NULL;
}


char* all_tests_level()
{
    printf("-------------------------------------------------------------------------------\n");
    printf("--- LEVEL ---------------------------------------------------------------------\n");
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
#ifndef LEVEL_H
#define LEVEL_H

#include "common.h"

#define LEVEL_MAX_WIDTH 35		
#define LEVEL_MAX_HEIGHT 17
#define LEVEL_MIN_DIMENSION 10 //No levels should be created with a width or height less than this macro.
#define HORIZONTAL_VERTICAL_DISTANCE 12
#define DIAGONAL_DISTANCE 17
#define ROAD_DISTANCE 6
#define INPUT_LENGTH 5 //The length the input string should have as command line argument.

/*
 * This struct represents a single cell in a level.
 * Each cell has a position (row, col), an owner and a type.
 */
typedef struct Cell 
{
    int row;
    int col;
    CellType type;
    Owner owner;
} Cell;

/*
 * This struct represents a Level
 *
 * Note that no pointer is used for the "cells" array.
 * This means that the cells are all stored inside the struct itself.
 * In other words, the memory used to store the Level struct, 
 * contains all the cells directly, it doesn't point to other memory 
 * containing the cells.
 */
typedef struct Level 
{
    Cell cells[LEVEL_MAX_HEIGHT][LEVEL_MAX_WIDTH];
    int width;
    int height;
} Level;

/*
* This function converts a string to an array with width and height. A string should be given of exactly 5 characters in the following format 25|12.
* Only digits and the separator  '|' are allowed. The functions allows dimension from 10x10 to 35x17.
* When an invalid string is given, the maximum dimensions are returned.
* Returns a pointer to an array with 3 integers. The third integer is 1 when a valid string is given and 0 otherwise.
*/
int* create_dimensions_from_string(const char* input);
/*
 * This function converts the textual representation of a Cell,
 * which is used in the Level file format,
 * to a CellType.
 *
 * For example, it converts the char '*' to the CellType GROUND
 */
CellType level_symbol_to_cell_type(char symbol);
/*
 * This function converts the textual representation of a Cell,
 * which is used in the Level file format,
 * to an owner.
 *
 * For example, it converts the char 'h' to the Owner HUMAN_PLAYER
 */
Owner level_symbol_to_owner(char symbol);

/*
 * This function converts a Cell to the textual representation of 
 * that cell in the level file format.
 */
char cell_to_symbol(Cell* cell);

/*
 * This function converts a combination of an owner and cell_type
 * to the textual representation of the corresponding cell in the 
 * level file format.
 *
 * for example, it converts the combination UNIT_2, OWNER_AI
 * to the character '8'
 */
char cell_type_and_owner_to_symbol(CellType cell_type, Owner owner);

/*
 * This functions returns true if the unit in a given Cell can
 * move over the given target Cell. 
 * A special exception is made if the unit is a HEADQUARTER.
 * In that case, the headquerter must be considered to be able to 
 * cross units. This is convenient when checking if a path between 2
 * headquarters exists.
 *
 * When a non ownable "unit" is passed, the output of this function is undefined.
 *
 * (No pathfinding is used here, only the CellType is important)
 *
 * Some examples:
 *   return | unit CellType | target CellType
 *     0    |      UNIT_2   |     ROCK
 *     1    |      UNIT_3   |     GROUND
 *     0    |      UNIT_1   |     UNIT_1
 *     0    |   HEADQUARTER |     WATER
 *     1    |   HEADQUARTER |     BRIDGE
 *     1    |   HEADQUARTER |     UNIT_1
 */
int level_can_walk_over(Cell* unit, Cell* target);

/*
 * This function returns true if the given position is a valid cell 
 * position within the given level.
 * A position is valid when it is within the dimensions of the level.
 * Note: Take negative row and coloumn values into account (they are always invalid)
 */
int level_is_valid_pos(Level* level, int row, int col);

/*
 * This functions returns pointer to an empty newly allocated level. The dimensions of an empty level are given as parameters.
 * When invalid dimensions are given, an empty level will have the maximum dimensions.
 */
Level* level_alloc_empty(int width, int height);
/*
 * This functions returns pointer to an newly allocated level.
 * The level is initialised by reading and processing the given file. 
 * When the given file is in an invalid format, a NULL pointer is returned.
 */
Level* level_alloc_read_from_file(const char* filename);
/*
 * Free the memory used by a level.
 */
void level_free(Level* level);

/*
 * This functions stores the given level to file.
 */
int level_write_to_file(Level* level, const char* filename);

/*
 * Returns a Cell struct that has been initialized with the function parameters.
 */
Cell cell_init(CellType type, Owner owner, int row, int col);

#endif


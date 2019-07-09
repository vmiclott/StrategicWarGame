#include "level.h"
#include "common.h"
#include "debug_file.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>
#include <string.h>
#include <ctype.h>

/*
* Function returns a pointer to a mapping between a character, a CellType and an Owner and vice versa.
* After this function is used, the free_mapping function should be used to avoid memory leaks.
*/
static Mapping* create_mapping() {
    Mapping* map = (Mapping*)malloc(sizeof(Mapping));
    if (!map) {
        fprintf(stderr, "ERROR: allocation failed, insufficient memory for map?\n");
        exit(1);
    }
    map->symbols = allocate_strlen(NUM_OF_SYMBOLS);
    map->owners = (Owner*)malloc(sizeof(Owner)*NUM_OF_SYMBOLS);
    map->types = (CellType*)malloc(sizeof(CellType)*NUM_OF_SYMBOLS);
    strcpy(map->symbols, SYMBOLS );
    Owner temp_owners[] = { OWNERS };
    CellType temp_types[] = { CELL_TYPES };
    for (int i = 0; i < NUM_OF_SYMBOLS; i++) {
        map->owners[i] = temp_owners[i];
        map->types[i] = temp_types[i];
    }
    return map;
}

static void free_mapping(Mapping* map) {
    free(map->symbols);
    free(map->owners);
    free(map->types);
    free(map);
}

Cell cell_init(CellType type, Owner owner, int row, int col) {
    Cell cell;
    cell.owner = owner;
    cell.type = type;
    cell.row = row;
    cell.col = col;
    return cell;
}

int* create_dimensions_from_string (const char* input) {
    int result[3];
    result[0] = LEVEL_MAX_WIDTH;
    result[1] = LEVEL_MAX_HEIGHT;
    //this variable is supposed to be 1 or 0 whether the given input is correct.
    result[2] = 1;
    //buffer to store the dimension-string. +2 to store the '|' character and the EOS-character.
    char buffer[INPUT_LENGTH+2];
    //check if input has the correct length and the middle character is the separator.
    if (strlen(input)==INPUT_LENGTH && input[INPUT_LENGTH/2]== '|') {
        for (int i = 0; i < INPUT_LENGTH; i++) {
            //check if every character, except the one in the middle is a digit.
            if (!isdigit(input[i]) && i!=INPUT_LENGTH/2) {
                fprintf(stderr, "ERROR: input should have the following format 25|12.\n");
                fprintf(stderr, "The dimensions for an empty level are: %dx%d.\n", result[0], result[1]);
                result[2] = 0;
                return result;
            }
            buffer[i] = input[i];
        }
    }
    else {
        fprintf(stderr, "ERROR: input should have the following format 25|12.\n");
        fprintf(stderr, "The dimensions for an empty level are: %dx%d.\n", result[0], result[1]);
        result[2] = 0;
        return result;
    }
    char* first_part;
    char* second_part;
    first_part = strtok(buffer, "|");
    second_part = strtok(NULL, "|");
    result[0]= atoi(first_part);
    result[1] = atoi(second_part);
    if (result[0] > LEVEL_MAX_WIDTH || result[1] > LEVEL_MAX_HEIGHT || result[0] < LEVEL_MIN_DIMENSION || result[1] < LEVEL_MIN_DIMENSION) {
        fprintf(stderr, "ERROR: the given level dimensions are incorrect.\n The maximum dimensions are %dx%d, the minimum %dx%d.\n", LEVEL_MAX_WIDTH, LEVEL_MAX_HEIGHT, LEVEL_MIN_DIMENSION, LEVEL_MIN_DIMENSION);
        result[0] = LEVEL_MAX_WIDTH;
        result[1] = LEVEL_MAX_HEIGHT;
        fprintf(stderr, "The dimensions for an empty level are: %dx%d.\n", result[0], result[1]);
    }
    return result;
}

CellType level_symbol_to_cell_type(char symbol) {
    Mapping* map = create_mapping();
    int i;
    for (int i = 0; i < NUM_OF_SYMBOLS; i++) {
        if (map->symbols[i] == symbol) {
            CellType result = map->types[i];
            free_mapping(map);
            return result;
        }
    }
    free_mapping(map);
    fprintf(stderr, "ERROR: no CellType found for %c!\n", symbol);
    return -1;
}

Owner level_symbol_to_owner(char symbol) {
    Mapping* map = create_mapping();
    int i;
    for (int i = 0; i < NUM_OF_SYMBOLS; i++) {
        if (map->symbols[i] == symbol) {
            Owner result = map->owners[i];
            free_mapping(map);
            return result;
        }
    }
    free_mapping(map);
    fprintf(stderr, "ERROR: no Owner found for %c!\n", symbol);
    return -1;
}

char cell_to_symbol(Cell* cell) {
    Mapping* map = create_mapping();
    int i;
    for (i = 0; i < NUM_OF_SYMBOLS; i++) {
        if (map->types[i] == cell->type && map->owners[i]== cell->owner) {
            char result = map->symbols[i];
            free_mapping(map);
            return result;
        }
    }
    free_mapping(map);
    fprintf(stderr, "ERROR: no character found for the given cell!\n");
    //Return a capital I (short for Invalid)
    return 'I';
}

char cell_type_and_owner_to_symbol(CellType cell_type, Owner owner) {
    char result;
    Cell temp_cell = cell_init(cell_type, owner, 0, 0);
    result = cell_to_symbol(&temp_cell);
    return result;
}

int level_can_walk_over(Cell* unit, Cell* target) 
{
    if (cell_type_is_unit(target->type)) {
        return unit->type == HEADQUARTER;
    }
    else if (target->type == HEADQUARTER) {
        if (unit->type == HEADQUARTER) {
            return 1;
        } else {
            return 0;
        }
    }
    else if (target->type == GROUND || target->type == BRIDGE || target->type == ROAD) {
        return 1;
    }
    else {
        return 0;
    }
}

int level_is_valid_pos(Level* level, int row, int col) {
    if (row<0 || row>=level->height || col<0 || col>=level->width) {
        return 0;
    }
    else {
        return 1;
    }
}

Level* level_alloc_empty(int width, int height) {
    Level *lp = (Level*)malloc(sizeof(Level));
    if (!lp) {
        fprintf(stderr, "ERROR: allocation failed, insufficient memory for level?\n");
        exit(1);
    }
    lp->height = LEVEL_MAX_HEIGHT;
    lp->width = LEVEL_MAX_WIDTH;
    if (width >= LEVEL_MIN_DIMENSION && width <= LEVEL_MAX_WIDTH && height >= LEVEL_MIN_DIMENSION && height <= LEVEL_MAX_HEIGHT) {
        lp->height = height;
        lp->width = width;
    }
    for (int i = 0; i < lp->height; i++) {
        for (int j = 0; j < lp->width; j++) {
            lp->cells[i][j] = cell_init(GROUND, OWNER_NONE, i, j);
        }
    }
    return lp;
}

Level* level_alloc_read_from_file(const char* filename)
{
    FILE* fp;
    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open file!\n");
        return NULL;
    }

    //2 extra places were provided in the buffer for the \n and \0 character.
    char buffer[LEVEL_MAX_WIDTH + 2];
    fgets(buffer, INPUT_LENGTH + 2, fp);
    //Deletes the \n character at the end of the string.
    buffer[strlen(buffer) - 1] = '\0';
    int *dimensions = create_dimensions_from_string(buffer);

    //return NULL when the dimension string is invalid
    if (dimensions[2] == 0) {
        fclose(fp);
        return NULL;
    }

    int width = dimensions[0];
    int height = dimensions[1];
    Level* level = level_alloc_empty(width,height);
    int r = 0;

    //the +2 is to provide place for the \n and \0 character
    while (fgets(buffer, width+2, fp)) {
        int c;
        for (c = 0; c < width; c++) {
            CellType type = level_symbol_to_cell_type(buffer[c]);
            Owner owner = level_symbol_to_owner(buffer[c]);
            //Check whether the file contains invalid characters.
            if (type == -1 || owner == -1) {
                fclose(fp);
                if (level != NULL) level_free(level);
                return NULL;
            }
            Cell temp_cell = cell_init(type, owner, r, c);
            level->cells[r][c] = temp_cell;
        }
        r++;
    }
    fclose(fp);    
    return level;
}

int level_write_to_file(Level* level, const char* filename) {
    if (filename == NULL) {
        return 0;
    }
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: can't open file");
        return 0;
    }

    fprintf(fp, "%u|%u\n", level->width, level->height);

    for (int i = 0; i < level->height; i++) {
        for (int j = 0; j < level->width; j++) {
            char *character = cell_to_symbol(&(level->cells[i][j]));
            fprintf(fp, "%c", character);
        }
        fprintf(fp, "%c", '\n');
    }
    fclose(fp);
    return 1;
}

void level_free(Level* level)
{
    free(level);
}

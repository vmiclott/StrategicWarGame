#include "common.h"
#include "gui.h"
#include "debug_file.h"

#include <stdlib.h>
#include <stdio.h>


char* allocate_strlen(int strlen) 
{
    char* allocated = (char*)malloc((strlen + 1)*sizeof(char));
    if (!allocated) {
        fprintf(stderr, "ERROR: Allocation failed, insufficient memory?\n");
        exit(1);
    }
    allocated[strlen] = '\0';
    return allocated;
}


Pos pos_init(int row, int col)
{
    Pos pos = { row, col };
    return pos;
}


Path* path_alloc(int step_count, int distance)
{
    Path *path = (Path*)malloc(sizeof(Path));
    if (!path) {
        fprintf(stderr, "ERROR: Allocation failed, insufficient memory for Path?\n");
        return NULL;
    }
    else {
        path->steps = (Pos*)malloc(sizeof(Pos)*step_count);
        path->step_count = step_count;
        path->distance = distance;
        return path;
    }
}

void path_free(Path* path)
{
    free(path->steps);
    free(path);
}


int cell_type_is_unit(CellType cell_type)
{
    CellType unit_cells[NR_OF_UNIT_CELLS] = { UNIT_CELLS };
    int i;
    for (i=0; i < NR_OF_UNIT_CELLS; i++) {
        if (cell_type == unit_cells[i]) {
            return 1;
        }
    }
    return 0;
}

int cell_type_is_player_owned(CellType cell_type)
{
    CellType not_player_cells[NR_OF_NON_PLAYER_CELLS] = { NON_PLAYER_CELLS };
    int i;
    for (i=0; i < NR_OF_NON_PLAYER_CELLS; i++) {
        if (cell_type == not_player_cells[i]) {
            return 0;
        }
    }
    return 1;
}

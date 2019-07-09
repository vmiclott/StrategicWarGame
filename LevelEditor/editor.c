#include "level.h"
#include "pathfinder.h"
#include "gui.h"
#include "common.h"
#include "event.h"
#include "debug_file.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <stdio.h>

#define AMOUNT_OF_THEMES 2

/*
 * Makes a headquarter owned by "owner" at the 2x2 square made by the positions [row, col], [row + 1, col], [row, col + 1],
 * [row + 1, col +  1].
 */

Cell* set_hq(Level *level, int row, int col, Owner owner) {
    CellType type = HEADQUARTER;
    level->cells[row][col].type = type;
    level->cells[row][col].owner = owner;
    level->cells[row + 1][col].type = type;
    level->cells[row + 1][col].owner = owner;
    level->cells[row][col + 1].type = type;
    level->cells[row][col + 1].owner = owner;
    level->cells[row + 1][col + 1].type = type;
    level->cells[row + 1][col + 1].owner = owner;
    return &level->cells[row][col];
}


/*
 * Removes the headquarter with "cell" as upper-left headquarter cell.
 */

void remove_hq(Level *level, Cell **cell) {
    int row = (*cell)->row;
    int col = (*cell)->col;
    level->cells[row][col].type = GROUND;
    level->cells[row][col].owner = OWNER_NONE;
    level->cells[row + 1][col].type = GROUND;
    level->cells[row + 1][col].owner = OWNER_NONE;
    level->cells[row][col + 1].type = GROUND;
    level->cells[row][col + 1].owner = OWNER_NONE;
    level->cells[row + 1][col + 1].type = GROUND;
    level->cells[row + 1][col + 1].owner = OWNER_NONE;
    *cell = NULL;
}

/*
 * Returns (a pointer to) the upper-left headquarter cell from the human headquarter, if there is no such headquarter this returns a null
 * pointer.
 */

Cell* find_human_hq(Level *level) {
    Cell c;
    for (int i = 0; i < level->height; i++) {
        for (int j = 0; j < level->width; j++) {
            c = level->cells[i][j];
            if (c.owner == OWNER_HUMAN && c.type == HEADQUARTER) {
                return &level->cells[i][j];
            }
        }
    }
    return NULL;
}


/*
 * Does the same as previous method but for the AI headquarter.
 */

Cell* find_ai_hq(Level *level) {
    Cell c;
    for (int i = 0; i < level->height; i++) {
        for (int j = 0; j < level->width; j++) {
            c = level->cells[i][j];
            if (c.owner == OWNER_AI && c.type == HEADQUARTER) {
                return &level->cells[i][j];
            }
        }
    }
    return NULL;
}


/*
 * Given a headquarter (left-upper cell pointer), a level (pointer) and a position, this method calculates whether the position is part of 
 * the headquarter or not. This function returns 1 if it is, 0 if it's not and also works in case the hq-pointer is NULL.
 */

int pos_is_hq(Level* level, Cell* hq, int row, int col) {
    if (hq == NULL) return 0;

    if ((hq->col == col || hq->col + 1 == col) && (hq->row == row || hq->row + 1 == row)) {
        return 1;
    }
    return 0;
    
}

/*
 * This method shows the correct messages when a level is not valid, i.e. when not every player has a hq or when the distance between the 2 
 * hq's is <100 or when there is no path between them.
 */

int invalid_level_messages(Path *p, Cell* human_hq, Cell* ai_hq) {
    if (p != NULL) {
        if (p->distance < 100) {
            gui_add_message("Invalid level: headquarters are too close, current distance: %d.", p->distance);
            return 0;
        }
        return 1;
    }
    else if (human_hq == NULL || ai_hq == NULL) {
        gui_add_message("Invalid level: each player needs a headquarter.");
        return 0;
    }
    else {
        gui_add_message("Invalid level: there is no valid path between the headquarters.");
        return 0;
    }
}

/*
 * This method checks if the given position will create a headquarter that overlays another headquarter.
 */
int hq_pos_is_other_hq(Level* level, Cell* hq, int row, int col) {
    if (hq == NULL) return 0;

    if(pos_is_hq(level, hq, row, col) || pos_is_hq(level, hq, row + 1, col) || pos_is_hq(level, hq, row, col +1) || pos_is_hq(level, hq, row + 1, col + 1)) {
        return 1;
    }
    return 0;
    
}


/*
 * Function to reset the level, the HQ's and the path between the HQ's. An adress should be given to the current level, 
 * 2 HQ's and path. An empty string is given as parameter to load an empty level. A filename to load a saved level. The width and height refer to
 * the standard_height and standard_width in the current game. 
 */
void start_new_game(Level** level, Path** p, Cell** hq_ai, Cell** hq_human, const int width, const int height, const char* filename) 
{
    if (*level != NULL) {
        level_free(*level);
        *level = NULL;
    }
    if (*p != NULL) {
        path_free(*p);
        *p = NULL;
    }
    *hq_ai = NULL;
    *hq_human = NULL;

    if (filename[0] == '\0') {
        *level = level_alloc_empty(width, height);
        gui_add_message("Game cleared.");
    }
    else {
        *level = level_alloc_read_from_file(filename);

        if (*level != NULL) {
            gui_add_message("File loaded successfully.");

            *hq_human = find_human_hq(*level);
            *hq_ai = find_ai_hq(*level);

            if (*hq_human != NULL && *hq_ai != NULL) {
                *p = find_path(*level, *hq_human, *hq_ai);

            }
        }
        else
        {
            gui_add_message("Reading level failed. An empty level was loaded.");
            *level = level_alloc_empty(width, height);
        }
    }

    gui_set_level(*level);
    gui_show_path(*p);
}

/*
* The main function can take 1 command line argument. This should be a string containing the dimensions of an empty level in the same format
* as in the level files. E.g. "25|12". 
*/
int main(int argc, char *argv[])
{
    // Code to check Memory Leaks.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    printf("Strategic War Level Editor\n");

    // Get default level width & height from command line
    int standard_width = LEVEL_MAX_WIDTH;
    int standard_height = LEVEL_MAX_HEIGHT;
    if (argc == 2) {
        int* dimensions = create_dimensions_from_string(argv[1]);
        standard_width = dimensions[0];
        standard_height = dimensions[1];
    }
    else if (argc > 2) {
        fprintf(stderr, "ERROR: supply only 1 argument of the following format: 25|12.\n");
    }
    
    // Initialize variables
    Level* level = NULL;
    Cell* hq_human = NULL;
    Cell* hq_ai = NULL;
    Path *p = NULL;

    // Set default selected cell type & owner
    CellType type = GROUND;
    Owner owner = OWNER_NONE;

    // For different themes
    int theme_count = 0;
    char theme_char = 'a';

    // Start the GUI
    //gui_initialize("civ_b");
    gui_initialize("civ_a");

    // Get path to default level file
    char* level_name = "basic.world";
    ALLEGRO_PATH* file_path = al_create_path(FILES_ASSETS_PATH);
    al_append_path_component(file_path, FILES_LEVELS_SUBDIRNAME);
    al_set_path_filename(file_path, level_name);
    const char* filename = al_path_cstr(file_path, ALLEGRO_NATIVE_PATH_SEP);

    // Start a new game with the default level file
    start_new_game(&level, &p, &hq_ai, &hq_human, standard_width, standard_height, filename);

    // Discard used file_path
    al_destroy_path(file_path);

    gui_draw_frame();

    int stop = 0;
    while (!stop) {
        Event event;
        gui_get_next_game_event(&event);
        switch (event.type) {
            case EVENT_TIMER:
            {
                gui_show_path(p);
                gui_draw_frame();
                break;
            }
            case EVENT_BUTTON:
            {
                if (event.button_event.mouse_event_type == MOUSE_CLICK) {
                    gui_clear_messages();
                    switch (event.button_event.function) {
                        case UI_EXIT:
                        {
                            stop = 1;
                            break;
                        }
                        case UI_SAVE:
                        {
                            if (level_write_to_file(level, gui_show_save_file_dialog())) {
                                gui_add_message("File was saved.");
                            }
                            else {
                                gui_add_message("File was not saved.");
                            }
                            break;
                        }
                        case UI_CLEAR:
                        {
                            start_new_game(&level, &p, &hq_ai, &hq_human, standard_width, standard_height, "");
                            break;
                        }
                        case UI_LOAD:
                        {
                            char* load_filename = gui_show_load_file_dialog();
                            if (load_filename == NULL) {
                                gui_add_message("No file was loaded.");
                            }
                            else {
                                start_new_game(&level, &p, &hq_ai, &hq_human, standard_width, standard_height, load_filename);
                            }
                            break;
                        }
                        case UI_THEME:
                        {
                            theme_count++;
                            if (theme_count == AMOUNT_OF_THEMES)
                            {
                                theme_char = 'a';
                                theme_count = 0;
                            }
                            else 
                            {
                                theme_char++;
                            }
                            char* directory_name[6];
                            sprintf(directory_name, "civ_%c",theme_char);
                            change_theme(directory_name);
                        }
                        default: break;
                    }
                }
                break;
            }
            case EVENT_LEVEL:
            {
                switch (event.level_event.mouse_event_type) {
                    case MOUSE_HOVER:
                    {
                        gui_set_level_highlight(event.level_event.row, event.level_event.col);
                        break;
                    }
                    case MOUSE_CLICK:
                    {
                        if (level_is_valid_pos(level, event.level_event.row, event.level_event.col)) {
                            gui_set_level_highlight(event.level_event.row, event.level_event.col);
                            gui_clear_messages();

                            if (type == HEADQUARTER) {
                                if (event.level_event.row < level->height - 1 && event.level_event.col < level->width - 1) {
                                    if (owner == OWNER_HUMAN) {
                                        if (hq_human != NULL) {
                                            remove_hq(level, &hq_human);
                                        }
                                        if (hq_pos_is_other_hq(level, hq_ai, event.level_event.row, event.level_event.col)) {
                                            remove_hq(level, &hq_ai);
                                        }
                                        hq_human = set_hq(level, event.level_event.row, event.level_event.col, owner);
                                    }
                                    else if (owner == OWNER_AI) {
                                        if (hq_ai != NULL) {
                                            remove_hq(level, &hq_ai);
                                        }
                                        if (hq_pos_is_other_hq(level, hq_human, event.level_event.row, event.level_event.col)) {
                                            remove_hq(level, &hq_human);
                                        }
                                        hq_ai = set_hq(level, event.level_event.row, event.level_event.col, owner);
                                    }
                                }
                            }
                            else {
                                if (pos_is_hq(level, hq_human, event.level_event.row, event.level_event.col)) {
                                    remove_hq(level, &hq_human);
                                }
                                else if (pos_is_hq(level, hq_ai, event.level_event.row, event.level_event.col)) {
                                    remove_hq(level, &hq_ai);
                                }
                                level->cells[event.level_event.row][event.level_event.col].type = type;
                                level->cells[event.level_event.row][event.level_event.col].owner = owner;
                            }

                            if (p != NULL) path_free(p);
                            if (hq_human != NULL && hq_ai != NULL) {
                                p = find_path(level, hq_human, hq_ai);
                            }
                            else {
                                p = NULL;
                            }
                            invalid_level_messages(p, hq_human, hq_ai);
                        }

                        break;
                    }
                    default: break;
                }
                break;
            }
            case EVENT_BUILD_SELECTOR:
            {
                switch (event.build_selector_event.mouse_event_type) {
                    case MOUSE_HOVER:
                    {
                        // for the build selector, only change highlight on click
                        //gui_set_build_highlight(event.build_selector_event.cell_type, event.build_selector_event.owner);
                        break;
                    }
                    case MOUSE_CLICK:
                    {
                        gui_set_build_highlight(event.build_selector_event.cell_type, event.build_selector_event.owner);
                        owner = event.build_selector_event.owner;
                        type = event.build_selector_event.cell_type;
                        break;
                    }
                    default: break;
                }
                break;
            }
            default: break;
        }
    }
    if (p != NULL) path_free(p);
    gui_free();
    level_free(level);

    return 0;
}


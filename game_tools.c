#include "game_tools.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* *********************************************************************** */
/*                                  GAME IO                                */
/* *********************************************************************** */

game game_load(char *filename)
{
    FILE *f = fopen(filename, "r");
    assert(f);

    uint rows, cols;
    uint wrapping;
    char square_char;
    uint ret;
    ret = fscanf(f, "%d %d %d", &rows, &cols, &wrapping);
    if (ret != 3)
    {
        fprintf(stderr, "The file doesn't have proper format");
        exit(EXIT_FAILURE);
    }
    fgetc(f);

    square squares[rows * cols];
    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            ret = fscanf(f, "%c", &square_char);
            if (ret != 1)
            {
                fprintf(stderr, "The file doesn't have proper format");
                exit(EXIT_FAILURE);
            }
            squares[(i * cols) + j] = _char2square(square_char);
        }
        fgetc(f);
    }
    fclose(f);

    game g = game_new_ext(rows, cols, squares, wrapping);
    game_update_flags(g);

    return g;
}

/* ************************************************************************** */

void game_save(cgame g, char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f == NULL || g == NULL);
    uint rows = game_nb_rows(g);
    uint cols = game_nb_cols(g);
    uint wrapping = game_is_wrapping(g);
    fprintf(f, "%d %d %d\n", rows, cols, wrapping);
    char char_square;
    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            char_square = _square2strV2(game_get_state(g, i, j));
            fprintf(f, "%c", char_square);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

/* ************************************************************************** */

bool game_solve(game g) { return game_solve_aux(g, 0, 0); }

/* ************************************************************************** */

uint game_nb_solutions(cgame g)
{
    game tab[10000];     // Will stocks all solutions
    uint tab_length = 0; // Will contains the number of solutions

    game new_g = game_copy(g);
    game_nb_solutions_aux(new_g, 0, 0, tab, &tab_length, NULL);

    for (uint i = 0; i < tab_length; i++)
        game_delete(tab[i]); // Empty the array, it avoids the malloc and free
                             // of tab at the start of the function
    return tab_length;
}
/**
 * @file game_private.c
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#include "game_private.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"

/* ************************************************************************** */
/*                             STACK ROUTINES                                 */
/* ************************************************************************** */

void _stack_push_move(queue *q, move m)
{
    assert(q);
    move *pm = malloc(sizeof(move));
    assert(pm);
    *pm = m;
    queue_push_head(q, pm);
}

/* ************************************************************************** */

move _stack_pop_move(queue *q)
{
    assert(q);
    move *pm = queue_pop_head(q);
    assert(pm);
    move m = *pm;
    free(pm);
    return m;
}

/* ************************************************************************** */

bool _stack_is_empty(queue *q)
{
    assert(q);
    return queue_is_empty(q);
}

/* ************************************************************************** */

void _stack_clear(queue *q)
{
    assert(q);
    queue_clear_full(q, free);
    assert(queue_is_empty(q));
}

/* ************************************************************************** */
/*                          GAME PRIVATE ROUTINES                             */
/* ************************************************************************** */

static char image[255] = {
    [S_BLANK] = ' ',  [S_BLACK] = '0',     '1',           '2', '3', '4',
    [S_BLACKU] = 'w', [S_LIGHTBULB] = '*', [S_MARK] = '-'};

/* ************************************************************************** */

static square code[255] = {
    ['b'] = S_BLANK, ['w'] = S_BLACKU, ['0'] = S_BLACK0, S_BLACK1,
    S_BLACK2,        S_BLACK3,         S_BLACK4,         ['*'] = S_LIGHTBULB,
    ['-'] = S_MARK};

/* ************************************************************************** */

static square square2img[255] = {
    [S_BLANK] = 'b',  [S_BLACK] = '0',     '1',           '2', '3', '4',
    [S_BLACKU] = 'w', [S_LIGHTBULB] = '*', [S_MARK] = '-'};

/* ************************************************************************** */
char _square2str(square s)
{
    square state = s & S_MASK;
    square flags = s & F_MASK;
    if ((state == S_BLANK) && (flags & F_LIGHTED)) return '.';
    return image[state];
}

/* ************************************************************************** */

square _char2square(char c) { return code[(int)c]; }

/* ************************************************************************** */

char _square2strV2(square s) { return square2img[s]; }

/* ************************************************************************** */
bool _check_square(square s)
{
    square state = s & S_MASK;
    square flags = s & F_MASK;
    if (state < S_START || state > S_END) return false;
    if ((flags & ~(F_LIGHTED | F_ERROR)) != 0) return false;
    return true;
}

/* ************************************************************************** */
/*                                 NEIGHBORHOOD                               */
/* ************************************************************************** */

/* Nota Bene: https://en.cppreference.com/w/c/language/array_initialization */
static int i_offset[] = {[HERE] = 0,      [UP] = -1,       [DOWN] = +1,
                         [LEFT] = 0,      [RIGHT] = 0,     [UP_LEFT] = -1,
                         [UP_RIGHT] = -1, [DOWN_LEFT] = 1, [DOWN_RIGHT] = 1};
static int j_offset[] = {[HERE] = 0,     [UP] = 0,         [DOWN] = 0,
                         [LEFT] = -1,    [RIGHT] = 1,      [UP_LEFT] = -1,
                         [UP_RIGHT] = 1, [DOWN_LEFT] = -1, [DOWN_RIGHT] = 1};

/* ************************************************************************** */

bool _inside(cgame g, int i, int j)
{
    assert(g);
    if (game_is_wrapping(g))
    {
        i = (i + game_nb_rows(g)) % game_nb_rows(g);
        j = (j + game_nb_cols(g)) % game_nb_cols(g);
    }
    if (i < 0 || j < 0 || i >= (int)g->nb_rows || j >= (int)g->nb_cols)
        return false;
    return true;
}

/* ************************************************************************** */

bool _inside_neigh(cgame g, int i, int j, direction dir)
{
    return _inside(g, i + i_offset[dir], j + j_offset[dir]);
}

/* ************************************************************************** */

bool _next(cgame g, int *pi, int *pj, direction dir)
{
    assert(g);
    assert(pi && pj);
    int i = *pi;
    int j = *pj;
    assert(i >= 0 && j >= 0 && i < (int)g->nb_rows && j < (int)g->nb_cols);

    // move to the next square in a given direction
    i += i_offset[dir];
    j += j_offset[dir];

    if (game_is_wrapping(g))
    {
        i = (i + game_nb_rows(g)) % game_nb_rows(g);
        j = (j + game_nb_cols(g)) % game_nb_cols(g);
    }
    if (!_inside(g, i, j)) return false;

    // update square coords
    *pi = i;
    *pj = j;

    return true;
}

/* ************************************************************************** */

bool _test(cgame g, int i, int j, square s, uint m)
{
    assert(g);
    assert(s >= S_START && s < S_END);
    if (game_is_wrapping(g))
    {
        i = (i + game_nb_rows(g)) % game_nb_rows(g);
        j = (j + game_nb_cols(g)) % game_nb_cols(g);
    }
    if (!_inside(g, i, j)) return false;
    return ((SQUARE(g, i, j) & m) == s);
}

/* ************************************************************************** */

bool _test_neigh(cgame g, int i, int j, square s, uint m, direction dir)
{
    return _test(g, i + i_offset[dir], j + j_offset[dir], s, m);
}

/* ************************************************************************** */

bool _neigh(cgame g, uint i, uint j, square s, uint m, bool diag)
{
    assert(g);
    assert(s >= S_START && s < S_END);

    // orthogonally
    if (_test_neigh(g, i, j, s, m, UP) || _test_neigh(g, i, j, s, m, DOWN) ||
        _test_neigh(g, i, j, s, m, LEFT) || _test_neigh(g, i, j, s, m, RIGHT))
        return true;

    // diagonally
    return diag && (_test_neigh(g, i, j, s, m, UP_LEFT) ||
                    _test_neigh(g, i, j, s, m, UP_RIGHT) ||
                    _test_neigh(g, i, j, s, m, DOWN_LEFT) ||
                    _test_neigh(g, i, j, s, m, DOWN_RIGHT));
}

/* ************************************************************************** */

uint _neigh_size(cgame g, uint i, uint j, bool diag)
{
    assert(g);

    return _neigh_count(g, i, j, 0, 0, diag);
}

/* ************************************************************************** */

uint _neigh_count(cgame g, uint i, uint j, square s, uint m, bool diag)
{
    assert(g);

    // orthogonally
    uint count =
        _test_neigh(g, i, j, s, m, UP) + _test_neigh(g, i, j, s, m, DOWN) +
        _test_neigh(g, i, j, s, m, LEFT) + _test_neigh(g, i, j, s, m, RIGHT);

    // diagonally
    if (diag)
    {
        count += _test_neigh(g, i, j, s, m, UP_LEFT) +
                 _test_neigh(g, i, j, s, m, UP_RIGHT) +
                 _test_neigh(g, i, j, s, m, DOWN_LEFT) +
                 _test_neigh(g, i, j, s, m, DOWN_RIGHT);
    }

    return count;
}

/* ************************************************************************** */
/*                                SOLVER ROUTINE                              */
/* ************************************************************************** */

bool solve_with_next_coords(game g, uint i, uint j)
{
    if (j + 1 == game_nb_cols(g)) { return game_solve_aux(g, i + 1, 0); }
    return game_solve_aux(g, i, j + 1);
}

bool save_result(game g, game curr_g)
{
    for (uint i = 0; i < game_nb_rows(g); i++)
        for (uint j = 0; j < game_nb_cols(g); j++)
            game_set_square(g, i, j, game_get_square(curr_g, i, j));
    game_update_flags(g);
    game_delete(curr_g);
    return true;
}

bool game_solve_aux(game g, uint x, uint y)
{
    if (game_is_over(g)) { return true; } // If the game is won then true

    for (uint i = x; i < game_nb_rows(g); i++)
    {
        for (uint j = y; j < game_nb_cols(g); j++)
        {
            if (game_has_error(g, i, j))
            {
                return false;
            } // If there's an error, the game is not solved yet
            if (game_get_square(g, i, j) != S_BLANK)
            {
                continue;
            } // If there's at least 1 blank square, we can play

            game curr_g = game_copy(g);
            game_play_move(curr_g, i, j,
                           S_LIGHTBULB); // We place a lightbulb in (i,j)

            if (game_solve_aux(curr_g, i, j))
            {
                return save_result(g, curr_g);
            } // If the game is won, we stock the current game

            game_delete(curr_g);
            if (solve_with_next_coords(g, i, j))
            {
                return true;
            } // If the game is won placing a lightbulb at (i+1,j+1) it's won
              // too
        }
        y = 0; // Return to first column
    }
    return false;
}

bool has_error_wall(game g)
{
    for (uint i = 0; i < game_nb_rows(g); i++)
        for (uint j = 0; j < game_nb_cols(g); j++)
            if (game_is_black(g, i, j) && game_has_error(g, i, j))
            {
                return true;
            }

    return false;
}

bool solution_exists(cgame g, game *tab, uint tab_length)
{
    if (tab_length == 0) return false;
    for (uint i = 0; i < tab_length; i++)
    {
        if (game_equal(g, tab[i])) { return true; }
    }

    return false;
}

void game_nb_solutions_aux(game g, uint x, uint y, game *tab, uint *tab_length,
                           uint *stop_cell)
{
    uint curr_stop_cell = 0;
    for (uint i = x; i < game_nb_rows(g); i++)
    {
        for (uint j = y; j < game_nb_cols(g); j++)
        {
            // If the square isn't white -> Next coordinates
            if (game_get_square(g, i, j) != S_BLANK) { continue; }
            if (stop_cell != NULL && *stop_cell == 0)
            {
                *stop_cell = (i * game_nb_cols(g)) + j + 1;
            }
            game_play_move(g, i, j, S_LIGHTBULB);

            if (game_is_over(g))
            {
                // If the solution hasn't been found yet,
                // We put the solution in tab
                if (!solution_exists(g, tab, *tab_length))
                {
                    game solved = game_copy(g);
                    tab[*tab_length] = solved;
                    (*tab_length)++;
                }
                // We cancel the last move hoping to find a new solution with
                // the last lightbulb at a new location
                game_undo(g);
                continue;
            }
            // If there's no error with the newly placed lightbulb,
            // We solve this new game
            if (!has_error_wall(g))
            {
                game new_g = game_copy(g);
                game_nb_solutions_aux(new_g, 0, 0, tab, tab_length,
                                      &curr_stop_cell);
            }
            if (!game_is_black(g, i, j)) { game_undo(g); }
            if (curr_stop_cell != 0 &&
                curr_stop_cell == ((i * game_nb_cols(g)) + j))
            {
                return;
            }
        }
    }
    return;
}

/* ************************************************************************** */
/*                                 MATH ROUTINE                               */
/* ************************************************************************** */

int min(int x, int y) { return x < y ? x : y; }
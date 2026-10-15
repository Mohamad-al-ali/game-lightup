#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_examples.h"
#include "game_ext.h"
#include "game_test.h"
#include "game_tools.h"

/* ************************************************************************** */
/*                                TESTS IO                                    */
/* ************************************************************************** */

int test_load(void)
{
    game g1 = game_load("load_test1.txt");
    game g2 = game_load("load_test2.txt");
    square s1[4] = {S_BLANK, S_BLACKU, S_BLACK0, S_BLANK};
    square s2[2] = {S_BLANK, S_BLACKU};
    printf("Game 1 :");
    printf("Rows : %d | Cols : %d\n", game_nb_rows(g1), game_nb_cols(g1));
    game_print(g1);

    printf("Game 2 :");
    printf("Rows : %d | Cols : %d\n", game_nb_rows(g2), game_nb_cols(g2));
    game_print(g2);
    bool test1 = check_game_ext(g1, 2, 2, s1, false);
    bool test2 = check_game_ext(g2, 2, 1, s2, true);
    game_delete(g1);
    game_delete(g2);
    if (test1 && test2) return EXIT_SUCCESS;
    return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_save(void)
{
    // game g2 = game_new(default_squares);
    // game_save(g2, "test_s_def.txt");
    // game gameTest = game_load("test_s_def.txt");
    // printf("\n game before save \n\n");
    // game_print(g2);
    // printf("\n Game after save \n\n");
    // game_print(gameTest);
    // if (game_equal(gameTest, g2)) { return EXIT_SUCCESS; }
    return EXIT_SUCCESS;
}

/* ************************************************************************** */
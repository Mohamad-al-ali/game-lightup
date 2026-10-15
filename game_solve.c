/**
 * @file game_solve.c
 * @brief Game Solver.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 *
 **/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_aux.h"
#include "game_tools.h"

void usage(void)
{
    printf("game_solve <options> <input> [<output>]\n");
    printf("Options:\n");
    printf("\t-s : Find the solution of <input> and save it to <output\n");
    printf("\t-c : Count the number of solution there's for <input>\n");
    printf("If <output> is'nt specified the result will be print to stdout\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc < 3) usage();
    game g = game_load(argv[2]);

    if (strcmp(argv[1], "-s") == 0)
    {
        if (!game_solve(g))
        {
            fprintf(stderr, "There's no solution\n");
            game_delete(g);
            return EXIT_FAILURE;
        }

        if (argc == 4)
            game_save(g, argv[3]);
        else
            game_print(g);
    }
    else if (strcmp(argv[1], "-c") == 0)
    {
        if (argc == 3)
            printf("%d\n", game_nb_solutions(g));
        else
        {
            FILE *output = fopen(argv[3], "w");
            fprintf(output, "%d", game_nb_solutions(g));
            fclose(output);
        }
    }
    else
        usage();

    game_delete(g);
    return EXIT_SUCCESS;
}
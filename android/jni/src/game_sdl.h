/**
 * @file model.h
 * @brief Graphic Interface Functions.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#ifndef __GAME_SDL_H__
#define __GAME_SDL_H__

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.h"
#include "queue.h"

#include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL_image.h> // required to load transparent texture from PNG
#include <SDL_ttf.h>   // required to use TTF fonts
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* ************************************************************************** */
/*                                   MACROS                                   */
/* ************************************************************************** */

#define FONT_SIZE 0xFF
#define FONT "../../assets/arial.ttf"
#define LIGHTBULB "../../assets/lightbulb.png"
#define RESTART "../../assets/restart.png"
#define SOLVE "../../assets/solve.png"
#define QUIT "../../assets/quit.png"
#define HELP "../../assets/help.png"
#define MULTI "../../assets/multi_button.png"
#define MARK "../../assets/mark.png"
#define HTP "../../assets/list_help.png"
#define RETURN "../../assets/return.png"

#define MARGE (0.02)
#define BUT_AREA (0.25)
#define MULTI_BUT_RATIO (0.333333)
#define CENTERING (0.5)
#define DOUBLE (2)

#ifdef __ANDROID__
    #define PRINT(STR, ...)              \
        do                               \
        {                                \
            SDL_Log(STR, ##__VA_ARGS__); \
        } while (0)
    #define ERROR(STR, ...)              \
        do                               \
        {                                \
            SDL_Log(STR, ##__VA_ARGS__); \
            exit(EXIT_FAILURE);          \
        } while (0)
#else
    #define PRINT(STR, ...)             \
        do                              \
        {                               \
            printf(STR, ##__VA_ARGS__); \
        } while (0)
    #define ERROR(STR, ...)                      \
        do                                       \
        {                                        \
            fprintf(stderr, STR, ##__VA_ARGS__); \
            exit(EXIT_FAILURE);                  \
        } while (0)
#endif

/* ************************************************************************** */
/*                          PARAMETERS OF WINDOW                              */
/* ************************************************************************** */

#define APP_NAME "SDL2 LIGHTUP"
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define DELAY 100

/* ************************************************************************** */
/*                                 STRUCT                                     */
/* ************************************************************************** */

typedef enum color_e color;

typedef enum button_e button;

typedef struct Env_t Env;

typedef struct Img_t Img;

/**
 * @brief Every colors use in the game
 */
enum color_e
{
    RED,
    BLACK,
    YELLOW,
    GRAY,
    L_GRAY
};

/**
 * @brief Store every different buttons in the game
 */
enum button_e
{
    B_LIGHTBULB,
    B_MARK,
    B_UNDO,
    B_REDO,
    B_HELP,
    B_QUIT,
    B_SOLVE,
    B_RESTART,
    B_CLOSE
};

/**
 * @brief Every information needed to handle an image
 */
struct Img_t
{
    SDL_Texture *texture; //< The actual image texture
    SDL_Rect rect;        //< The coordinates and the size of the image
};

/**b
 * @brief Every variable we need in our game
 * */
struct Env_t
{
    Img lightbulb, quit, solve, restart, help, multi_button, mark, list_help,
        rtn;                          //< The buttons informations
    uint start_x, start_y, cell_size; //< The game grid informations
    game g;                           //< The actual game
    bool mark_or_light, want_help;    //<status of click
};

/* ************************************************************************** */
/*                                 FUNCTIONS                                  */
/* ************************************************************************** */

/**
 * @brief Initialise the SDL
 *
 * @param win the window
 * @param ren the renderer (the space where we will draw or write)
 * @param argc number of elements in string argv
 * @param argv[] the address of string
 * @return the windows modified with new graphisms
 */
Env *init(SDL_Renderer *ren, int argc, char *argv[]);

/**
 * @brief create the renderer
 *
 * @param win the window
 * @param ren the renderer
 * @param env the
 */
void render(SDL_Window *win, SDL_Renderer *ren, Env *env);

/**
 * @brief
 *
 * @param win the window
 * @param ren the renderer
 * @param env the
 * @param e the
 * @return if    then true, return false otherwise
 */
bool process(SDL_Window *win, Env *env, SDL_Event *e);

/**
 * @brief erase the window modified
 *
 * @param win the window
 * @param ren the renderer
 * @param env the
 */
void clean(SDL_Window *win, SDL_Renderer *ren, Env *env);

/** @brief Set the color of the renderer
 *
 * @param ren the renderer
 * @param c the color
 */
void set_color(SDL_Renderer *ren, color c);

/**
 * @brief Return the distance between the mouse and a button by giving the
 * distance in x and y axis
 *
 * @param x the x axis distance
 * @param y the y axis distance
 * @return int
 */
int hyp(int x, int y);

#endif // __GAME_SDL_H__
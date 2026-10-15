/**
 * @file game_sdl.c
 * @brief Game Graphic Interface.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#include "game_sdl.h"

/* ************************************************************************** */

Env *init(SDL_Renderer *ren, int argc, char *argv[])
{
    Env *env = malloc(sizeof(struct Env_t));

    /* Get the current game */
    if (argc == 1) { env->g = game_default(); }
    else
    {
        env->g = game_load(argv[1]);
    }

    /* Initialize textures */
    env->lightbulb.texture = IMG_LoadTexture(ren, LIGHTBULB);
    if (!env->lightbulb.texture) ERROR("IMG_LoadTexture: %s\n", LIGHTBULB);
    /***************************************/
    env->restart.texture = IMG_LoadTexture(ren, RESTART);
    if (!env->restart.texture) ERROR("IMG_LoadTexture: %s\n", RESTART);
    /***************************************/
    env->solve.texture = IMG_LoadTexture(ren, SOLVE);
    if (!env->solve.texture) ERROR("IMG_LoadTexture: %s\n", SOLVE);
    /***************************************/
    env->quit.texture = IMG_LoadTexture(ren, QUIT);
    if (!env->quit.texture) ERROR("IMG_LoadTexture: %s\n", QUIT);
    /***************************************/
    env->help.texture = IMG_LoadTexture(ren, HELP);
    if (!env->help.texture) ERROR("IMG_LoadTexture: %s\n", HELP);
    /***************************************/
    env->multi_button.texture = IMG_LoadTexture(ren, MULTI);
    if (!env->multi_button.texture) ERROR("IMG_LoadTexture: %s\n", MULTI);
    /***************************************/
    env->mark.texture = IMG_LoadTexture(ren, MARK);
    if (!env->mark.texture) ERROR("IMG_LoadTexture: %s\n", MARK);
    /***************************************/
    env->list_help.texture = IMG_LoadTexture(ren, HTP);
    if (!env->list_help.texture) ERROR("IMG_LoadTexture: %s\n", HTP);
    /***************************************/
    env->rtn.texture = IMG_LoadTexture(ren, RETURN);
    if (!env->rtn.texture) ERROR("IMG_LoadTexture: %s\n", RETURN);
    // to play a lightbulb
    env->mark_or_light = true;

    // to display  the list of help
    env->want_help = false;
    return env;
}

/* ************************************************************************** */

void render_wall_number(SDL_Renderer *ren, SDL_Rect rect, Env *env, uint i,
                        uint j)
{
    if (game_get_black_number(env->g, i, j) >= 0)
    {
        // Setting the color depending on if there's an error or not
        SDL_Color color = game_has_error(env->g, i, j)
                              ? (SDL_Color){0xFF, 0, 0, 0xFF}
                              : (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF};

        // init text texture using Arial font
        TTF_Font *font = TTF_OpenFont(FONT, FONT_SIZE);
        if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
        TTF_SetFontStyle(font, TTF_STYLE_BOLD);
        SDL_Surface *surf = TTF_RenderText_Blended(
            font, (char[2]){_square2str(game_get_square(env->g, i, j)), '\0'},
            color);
        SDL_Texture *text = SDL_CreateTextureFromSurface(ren, surf);
        SDL_FreeSurface(surf);
        TTF_CloseFont(font);

        // Resizing and placing the text in the middle of the cell
        rect.w = 0.75 * env->cell_size;
        rect.h = 0.75 * env->cell_size;
        rect.x += (env->cell_size - rect.w) * CENTERING;
        rect.y += (env->cell_size - rect.h) * CENTERING;
        SDL_RenderCopy(ren, text, NULL, &rect);
    }
}

/* ************************************************************************** */

void set_color(SDL_Renderer *ren, color c)
{
    switch (c)
    {
    case (RED): SDL_SetRenderDrawColor(ren, 0xF0, 0, 0, 0x50); return;
    case (BLACK): SDL_SetRenderDrawColor(ren, 0, 0, 0, 0xFF); return;
    case (YELLOW): SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0, 0x50); return;
    case (GRAY): SDL_SetRenderDrawColor(ren, 0x40, 0x40, 0x40, 0xFF); return;
    case (L_GRAY): SDL_SetRenderDrawColor(ren, 0xC0, 0xC0, 0xC0, 0xFF); return;
    }
}

/* ************************************************************************** */

int hyp(int x, int y) { return sqrt(pow(x, 2) + pow(y, 2)); }

/* ************************************************************************** */

bool is_clicked(Env *env, button b, SDL_Point mouse)
{
    if (b == B_RESTART || b == B_SOLVE)
    {
        int radius = env->restart.rect.w * CENTERING;
        int center_x, center_y;

        center_x = b == B_RESTART ? mouse.x - (env->restart.rect.x + radius)
                                  : mouse.x - (env->solve.rect.x + radius);
        center_y = b == B_RESTART ? mouse.y - (env->restart.rect.y + radius)
                                  : mouse.y - (env->solve.rect.y + radius);

        int mouse_dist_from_but = hyp(center_x, center_y);
        return mouse_dist_from_but <= radius;
    }
    else
    {
        int start_x, start_y, end_x, end_y;

        switch (b)
        {
        case B_QUIT:
            start_x = env->quit.rect.x;
            start_y = env->quit.rect.y;
            end_x = start_x + env->quit.rect.w;
            end_y = start_y + env->quit.rect.h;
            break;

        case B_HELP:
            start_x = env->help.rect.x;
            start_y = env->help.rect.y;
            end_x = start_x + env->help.rect.w;
            end_y = start_y + env->help.rect.h;
            break;

        case B_UNDO:
            start_x = env->multi_button.rect.x;
            start_y = env->multi_button.rect.y +
                      env->multi_button.rect.h * MULTI_BUT_RATIO;
            end_x = start_x + env->multi_button.rect.w * MULTI_BUT_RATIO;
            end_y = start_y + env->multi_button.rect.h * MULTI_BUT_RATIO;
            break;

        case B_REDO:
            start_x = env->multi_button.rect.x +
                      env->multi_button.rect.w * MULTI_BUT_RATIO * DOUBLE;
            start_y = env->multi_button.rect.y +
                      env->multi_button.rect.h * MULTI_BUT_RATIO;
            end_x = start_x + env->multi_button.rect.w * MULTI_BUT_RATIO;
            end_y = start_y + env->multi_button.rect.h * MULTI_BUT_RATIO;
            break;

        case B_MARK:
            start_x = env->multi_button.rect.x +
                      env->multi_button.rect.w * MULTI_BUT_RATIO;
            start_y = env->multi_button.rect.y +
                      env->multi_button.rect.h * MULTI_BUT_RATIO * DOUBLE;
            end_x = start_x + env->multi_button.rect.w * MULTI_BUT_RATIO;
            end_y = start_y + env->multi_button.rect.h * MULTI_BUT_RATIO;
            break;

        case B_LIGHTBULB:
            start_x = env->multi_button.rect.x +
                      env->multi_button.rect.w * MULTI_BUT_RATIO;
            start_y = env->multi_button.rect.y;
            end_x = start_x + env->multi_button.rect.w * MULTI_BUT_RATIO;
            end_y = start_y + env->multi_button.rect.h * MULTI_BUT_RATIO;
            break;

        case B_CLOSE:
            start_x = env->rtn.rect.x;
            start_y = env->rtn.rect.y;
            end_x = start_x + env->rtn.rect.w;
            end_y = start_y + env->rtn.rect.h;
            break;

        default: return false;
        }

        return mouse.x > start_x && mouse.x < end_x && mouse.y > start_y &&
               mouse.y < end_y;
    }
}

/* ************************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env)
{
    SDL_Rect rect;
    uint nb_cols = game_nb_cols(env->g);
    uint nb_rows = game_nb_rows(env->g);

    /* Get current window size */
    uint w, h;
    SDL_GetWindowSize(win, (int *)&w, (int *)&h);

    /* background in gray */
    set_color(ren, GRAY);
    SDL_RenderClear(ren);

    /* Playground in Opaque Light-Gray with the margin + the button area
     * positioned depending on the shape of the screen */
    set_color(ren, L_GRAY);
    rect.x = w * MARGE;
    rect.y = h * MARGE;
    rect.w =
        h > w ? w - (rect.x * DOUBLE) : w - (rect.x * DOUBLE) - (w * BUT_AREA);
    rect.h =
        h > w ? h - (h * BUT_AREA) - (rect.y * DOUBLE) : h - (rect.y * DOUBLE);
    SDL_RenderFillRect(ren, &rect);

    /* Rendering each cells one by one with their contents */
    env->cell_size = min(rect.w / nb_cols, rect.h / nb_rows);
    env->start_x = rect.x + ((rect.w - (env->cell_size * nb_cols)) * CENTERING);
    env->start_y = rect.y + ((rect.h - (env->cell_size * nb_rows)) * CENTERING);
    rect =
        (SDL_Rect){env->start_x, env->start_y, env->cell_size, env->cell_size};

    // Enable Transparency
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    for (uint i = 0; i < nb_rows; i++)
    {
        for (uint j = 0; j < nb_cols; j++)
        {
            // Set Grid Border Color to Opaque Black
            set_color(ren, BLACK);
            SDL_RenderDrawRect(ren, &rect);
            if (game_is_black(env->g, i, j))
            {
                // Here color is already set before
                SDL_RenderFillRect(ren, &rect);
                render_wall_number(ren, rect, env, i, j);
            }
            else if (game_has_error(env->g, i, j))
            {
                // set drawing color to Transparent Red
                set_color(ren, RED);
                SDL_RenderFillRect(ren, &rect);
            }
            else if (game_is_lighted(env->g, i, j))
            {
                // set drawing color to Transparent Yellow
                set_color(ren, YELLOW);
                SDL_RenderFillRect(ren, &rect);
            }
            if (game_is_lightbulb(env->g, i, j))
            {
                SDL_RenderCopy(ren, env->lightbulb.texture, NULL, &rect);
            }
            if (game_is_marked(env->g, i, j))
            {
                SDL_RenderCopy(ren, env->mark.texture, NULL, &rect);
            }
            // Going to the next cell's coordinates
            rect.x += env->cell_size;
        }
        // Going to the next rows of cells
        rect.x = env->start_x;
        rect.y += env->cell_size;
    }
    /* render button Restart texture */
    int _w = h <= w ? w * BUT_AREA * MULTI_BUT_RATIO : w * MULTI_BUT_RATIO;
    int _h = h <= w ? h * MULTI_BUT_RATIO : h * BUT_AREA * MULTI_BUT_RATIO;

    env->restart.rect.w = min(_w, _h);
    env->restart.rect.h = env->restart.rect.w;
    env->restart.rect.x =
        h <= w ? w - (w * BUT_AREA * CENTERING) : w * MULTI_BUT_RATIO / DOUBLE;
    env->restart.rect.y = h <= w ? h - (h * MULTI_BUT_RATIO / DOUBLE) -
                                       env->restart.rect.h * CENTERING
                                 : h - (h * BUT_AREA * CENTERING);
    SDL_RenderCopy(ren, env->restart.texture, NULL, &env->restart.rect);

    /* render button solve texture */
    env->solve.rect.w = env->restart.rect.w;
    env->solve.rect.h = env->restart.rect.h;
    env->solve.rect.x = env->restart.rect.x - env->solve.rect.w;
    env->solve.rect.y = env->restart.rect.y - env->solve.rect.h;
    SDL_RenderCopy(ren, env->solve.texture, NULL, &env->solve.rect);

    /* render button quit texture */
    env->quit.rect.w = (w * BUT_AREA) * MULTI_BUT_RATIO / DOUBLE;
    env->quit.rect.h = (h * BUT_AREA) * CENTERING;

    env->quit.rect.x = h <= w ? w - (w * MARGE) - env->quit.rect.w
                              : w * CENTERING + env->quit.rect.w * DOUBLE;
    env->quit.rect.y = h <= w ? h * CENTERING
                              : h - (h * MARGE) - (h * BUT_AREA * CENTERING) -
                                    env->quit.rect.h * CENTERING;
    SDL_RenderCopy(ren, env->quit.texture, NULL, &env->quit.rect);

    /* render button help texture */
    env->help.rect.w = env->quit.rect.w;
    env->help.rect.h = env->quit.rect.h;
    env->help.rect.x = h <= w ? w - (w * MARGE) - env->help.rect.w
                              : w * CENTERING - env->help.rect.w * DOUBLE;
    env->help.rect.y = h <= w ? h * MULTI_BUT_RATIO
                              : h - (h * MARGE) - (h * BUT_AREA * CENTERING) -
                                    env->help.rect.h * CENTERING;
    SDL_RenderCopy(ren, env->help.texture, NULL, &env->help.rect);

    /* render button multi texture */
    _w = w * BUT_AREA - (w * MARGE);
    _h = h * BUT_AREA - (h * MARGE);
    env->multi_button.rect.w = min(_w, _h);
    env->multi_button.rect.h = env->multi_button.rect.w;
    env->multi_button.rect.x = h <= w ? w - w * (MARGE + BUT_AREA * CENTERING) -
                                            env->multi_button.rect.w * CENTERING
                                      : w - (w * MULTI_BUT_RATIO) + (w * MARGE);
    env->multi_button.rect.y =
        h <= w ? h * MULTI_BUT_RATIO - env->multi_button.rect.h
               : h * (1 - MARGE - BUT_AREA * CENTERING) -
                     env->multi_button.rect.h * CENTERING;
    SDL_RenderCopy(ren, env->multi_button.texture, NULL,
                   &env->multi_button.rect);
    if (env->want_help)
    {
        /* render display help texture */
        env->list_help.rect.w = w;
        env->list_help.rect.h = h;
        env->list_help.rect.x = 0;
        env->list_help.rect.y = 0;
        SDL_RenderCopy(ren, env->list_help.texture, NULL, &env->list_help.rect);

        /* render button close to list display help texture */
        env->rtn.rect.w = w / 20;
        env->rtn.rect.h = h / 20;
        env->rtn.rect.x = w * MARGE;
        env->rtn.rect.y = h * MARGE;
        SDL_RenderCopy(ren, env->rtn.texture, NULL, &env->rtn.rect);
    }

    if (game_is_over(env->g))
    {
        SDL_Color color = (SDL_Color){0xFF, 0xFF, 0xFF, 0xA0};

        // init text texture using Arial font
        TTF_Font *font = TTF_OpenFont(FONT, FONT_SIZE);
        if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
        TTF_SetFontStyle(font, TTF_STYLE_BOLD);
        SDL_Surface *surf =
            TTF_RenderText_Blended(font, "Congratulation !", color);
        SDL_Texture *text = SDL_CreateTextureFromSurface(ren, surf);
        SDL_FreeSurface(surf);
        TTF_CloseFont(font);

        SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
        double ratio = rect.w / rect.h;
        rect.w = w - w * MARGE * DOUBLE;
        rect.h = rect.w / ratio;
        rect.x = w * CENTERING - rect.w * CENTERING;
        rect.y = h * CENTERING - rect.h * CENTERING;
        SDL_RenderCopy(ren, text, NULL, &rect);
    }
}

/* ************************************************************************** */

bool process(SDL_Window *win, Env *env, SDL_Event *e)
{
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    SDL_Point mouse;
    if (e->type == SDL_QUIT) { return true; }
#ifdef __ANDROID__
    else if (e->type == SDL_FINGERDOWN)
    {
        mouse = (SDL_Point){e->tfinger.x * w, e->tfinger.y * h};
    }
#else
    else if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&mouse.x, &mouse.y);
    }
#endif
    else return false;

    if (is_clicked(env, B_RESTART, mouse))
    {
        game_restart(env->g);
        return false;
    }
    if (is_clicked(env, B_QUIT, mouse)) { return true; }
    if (is_clicked(env, B_UNDO, mouse))
    {
        game_undo(env->g);
        return false;
    }
    if (is_clicked(env, B_REDO, mouse))
    {
        game_redo(env->g);
        return false;
    }

    // If the game is over, we just need to handle the Restart, Quit
    // Undo and Redo button So we quit the function after those four
    if (game_is_over(env->g)) { return false; }
    if (is_clicked(env, B_SOLVE, mouse))
    {
        game_restart(env->g);
        game_solve(env->g);
        return false;
    }
    if (is_clicked(env, B_HELP, mouse))
    {
        env->want_help = true;
        return false;
    }
    if (is_clicked(env, B_CLOSE, mouse))
    {
        env->want_help = false;
        return false;
    }
    if (is_clicked(env, B_MARK, mouse)) { env->mark_or_light = false; }
    if (is_clicked(env, B_LIGHTBULB, mouse)) { env->mark_or_light = true; }

    int i = (mouse.y - env->start_y) / env->cell_size;
    int j = (mouse.x - env->start_x) / env->cell_size;

    // If the click happened outside of the grid, then ignore the event
    if (i < 0 || j < 0 || i >= (int)game_nb_rows(env->g) ||
        j >= (int)game_nb_cols(env->g))
        return false;
    // If the click happened on a wall, then ignore the event
    if (game_is_black(env->g, i, j)) { return false; }

    square s;
    if (env->mark_or_light)
    {
        s = game_is_lightbulb(env->g, i, j) ? S_BLANK : S_LIGHTBULB;
        game_play_move(env->g, i, j, s);
    }
    else
    {
        s = game_is_marked(env->g, i, j) ? S_BLANK : S_MARK;
        game_play_move(env->g, i, j, s);
    }
    return false;
}

/* ************************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env)
{
    SDL_DestroyTexture(env->lightbulb.texture);
    SDL_DestroyTexture(env->restart.texture);
    SDL_DestroyTexture(env->solve.texture);
    SDL_DestroyTexture(env->quit.texture);
    SDL_DestroyTexture(env->help.texture);
    SDL_DestroyTexture(env->mark.texture);
    SDL_DestroyTexture(env->multi_button.texture);
    SDL_DestroyTexture(env->list_help.texture);
    SDL_DestroyTexture(env->rtn.texture);
    game_delete(env->g);
    free(env);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}

/* ************************************************************************** */

int main(int argc, char *argv[])
{
    /* initialize SDL2 and some extensions */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        ERROR("Error: SDL_Init VIDEO (%s)", SDL_GetError());
    if (IMG_Init(IMG_INIT_PNG & IMG_INIT_PNG) != IMG_INIT_PNG)
        ERROR("Error: IMG_Init PNG (%s)", SDL_GetError());
    if (TTF_Init() != 0) ERROR("Error: TTF_Init (%s)", SDL_GetError());

    /* create window and renderer */
    SDL_Window *win = SDL_CreateWindow(
        APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!win) ERROR("Error: SDL_CreateWindow (%s)", SDL_GetError());
    SDL_Renderer *ren = SDL_CreateRenderer(
        win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    if (!ren) ERROR("Error: SDL_CreateRenderer (%s)", SDL_GetError());

    /* initialize your environment */
    Env *env = init(ren, argc, argv);

    /* main render loop */
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        /* manage events */
        while (SDL_PollEvent(&e))
        {
            /* process your events */
            quit = process(win, env, &e);
            if (quit) break;
        }
        /* render all what you want */
        render(win, ren, env);
        SDL_RenderPresent(ren);
        SDL_Delay(DELAY);
    }

    /* clean your environment */
    clean(win, ren, env);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
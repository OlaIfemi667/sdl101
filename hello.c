
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_log.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define CELL_SIZE 100
#define ROWS (HEIGHT / CELL_SIZE)
#define COLUMNS (WIDTH / CELL_SIZE)
#define SIZE (ROWS * COLUMNS)

typedef struct Vector {
    int x;
    int y;
} Vector;

typedef struct Cells {
    SDL_FRect shape;
    bool is_occupied;
} Cells;

typedef struct Board {
    Cells cellules[SIZE];
} Board;

typedef struct Snake {
    Cells body[SIZE];
} Snake;

void init_board(Board *b)
{
    for (int i = 0; i < SIZE; i++) {
        b->cellules[i].is_occupied = (i % 4 == 0);

        b->cellules[i].shape.x = (float)(CELL_SIZE * (i % COLUMNS));
        b->cellules[i].shape.y = (float)(CELL_SIZE * (i / COLUMNS));
        b->cellules[i].shape.w = (float)CELL_SIZE;
        b->cellules[i].shape.h = (float)CELL_SIZE;
    }

    SDL_Log("SIZE = %d", SIZE);
}

void draw_board(Board *b, SDL_Renderer *renderer)
{
    for (int i = 0; i < SIZE; i++) {
        if (b->cellules[i].is_occupied)
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        if (!SDL_RenderFillRect(renderer, &b->cellules[i].shape))
            SDL_Log("Erreur de dessin : %s", SDL_GetError());
        /* Bordure sombre pour distinguer les cases */
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_RenderRect(renderer, &b->cellules[i].shape);
    }
}

int main(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("My first window", WIDTH, HEIGHT, 0);

    if (!window) {
        SDL_Log("Erreur fenêtre : %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) {
        SDL_Log("Erreur renderer : %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Board board;
    init_board(&board);
    board.cellules[22].is_occupied = true;

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    if (e.key.key == SDLK_ESCAPE)
                        quit = true;
                    break;

                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        if (!SDL_RenderClear(renderer)) {
            SDL_Log("Erreur d'effacement : %s", SDL_GetError());
            quit = true;
            break;
        }

        draw_board(&board, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


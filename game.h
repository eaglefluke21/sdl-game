#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "entities.h"

typedef struct {
    SDL_Window*   window;
    SDL_Renderer* renderer;
    int screenW, screenH;

    Player player1;
    Player player2;
    Ball   ball;

    int running;
} Game;

typedef struct{
    int p1x,p2x,p1y,p2y;
} Player_positions;



void game_init(Game* g ,Player_positions* p);
void game_run(Game* g);
void game_cleanup(Game* g);

#endif

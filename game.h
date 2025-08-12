#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <math.h> 

typedef enum {
    OWNER_NONE,
    OWNER_PLAYER1,
    OWNER_PLAYER2
} BallOwner;

typedef enum {
    SIDE_LEFT,
    SIDE_RIGHT
} AttachSide;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_FRect player1, player2, ball;
} Game;


void render(Game* game);
void cleanup(Game* game);

#endif

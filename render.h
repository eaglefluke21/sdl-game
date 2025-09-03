#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include <math.h>
#include "entities.h"


void render_begin(SDL_Renderer* r);
void render_end(SDL_Renderer* r);

void render_player(SDL_Renderer* r, const Player* p, SDL_Color color);
void render_ball(SDL_Renderer* r, const Ball* b, SDL_Color color);

// shape helpers (your old ones, names kept similar)
void drawFilledCircle(SDL_Renderer* r, int cx, int cy, int radius);
void drawRoundedRect(SDL_Renderer* r, const SDL_FRect* rect, int radius);

#endif

#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <SDL3/SDL.h>

// Draw a filled circle
void drawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius);

// Draw a rounded rectangle
void drawRoundedRect(SDL_Renderer *renderer, SDL_FRect *rect, int radius);

#endif

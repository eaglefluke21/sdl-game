#include "render_utils.h"
#include <math.h> 

void drawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = sqrtf(radius * radius - dy * dy);
        SDL_RenderLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

void drawRoundedRect(SDL_Renderer *renderer, SDL_FRect *rect, int radius) {
    SDL_FRect middle = { rect->x + radius, rect->y, rect->w - 2 * radius, rect->h };
    SDL_RenderFillRect(renderer, &middle);

    SDL_FRect left = { rect->x, rect->y + radius, radius, rect->h - 2 * radius };
    SDL_RenderFillRect(renderer, &left);

    SDL_FRect right = { rect->x + rect->w - radius, rect->y + radius, radius, rect->h - 2 * radius };
    SDL_RenderFillRect(renderer, &right);

    drawFilledCircle(renderer, rect->x + radius, rect->y + radius, radius);
    drawFilledCircle(renderer, rect->x + rect->w - radius, rect->y + radius, radius);
    drawFilledCircle(renderer, rect->x + radius, rect->y + rect->h - radius, radius);
    drawFilledCircle(renderer, rect->x + rect->w - radius, rect->y + rect->h - radius, radius);
}

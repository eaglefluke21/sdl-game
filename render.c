#include "render.h"
#include <math.h>

void render_begin(SDL_Renderer* r) {
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderClear(r);
}

void render_end(SDL_Renderer* r) {
    SDL_RenderPresent(r);
}

void render_player(SDL_Renderer* r, const Player* p, SDL_Color color) {
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
    drawRoundedRect(r, &p->rect, 10);
}

void render_ball(SDL_Renderer* r, const Ball* b, SDL_Color color) {
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

    // center
    float cx = b->rect.x + b->rect.w * 0.5f;
    float cy = b->rect.y + b->rect.h * 0.5f;

    int radius = (int)(b->rect.w * 0.5f);

    drawFilledCircle(r, (int)cx, (int)cy, radius);
}


// ---- shape helpers (from your previous render_utils) ----
void drawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = (int)sqrtf((float)(radius * radius - dy * dy));
        SDL_RenderLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

void drawRoundedRect(SDL_Renderer *renderer, const SDL_FRect *rect, int radius) {
    SDL_FRect middle = { rect->x + radius, rect->y, rect->w - 2 * radius, rect->h };
    SDL_RenderFillRect(renderer, &middle);

    SDL_FRect left = { rect->x, rect->y + radius, (float)radius, rect->h - 2 * radius };
    SDL_RenderFillRect(renderer, &left);

    SDL_FRect right = { rect->x + rect->w - radius, rect->y + radius, (float)radius, rect->h - 2 * radius };
    SDL_RenderFillRect(renderer, &right);

    drawFilledCircle(renderer, (int)(rect->x + radius),                 (int)(rect->y + radius),                 radius);
    drawFilledCircle(renderer, (int)(rect->x + rect->w - radius),       (int)(rect->y + radius),                 radius);
    drawFilledCircle(renderer, (int)(rect->x + radius),                 (int)(rect->y + rect->h - radius),       radius);
    drawFilledCircle(renderer, (int)(rect->x + rect->w - radius),       (int)(rect->y + rect->h - radius),       radius);
}



#include "game.h"

void render(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    SDL_SetRenderDrawColor(game->renderer, 212, 115, 78, 255); // orange
    SDL_RenderFillRect(game->renderer, &game->player1);

    SDL_SetRenderDrawColor(game->renderer, 50, 205, 218, 235); // turquoise
    SDL_RenderFillRect(game->renderer, &game->player2);

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(game->renderer, &game->ball);

    SDL_RenderPresent(game->renderer);
    SDL_Delay(17);
}

void cleanup(Game* game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

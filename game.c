#include "game.h"
#include "render_utils.h"


void update(Game* game, float deltaTime,float *ballVelx, float *ballVely) {
    game->ball.x += (*ballVelx) * deltaTime;
    game->ball.y += (*ballVely) * deltaTime;

    // Collision with left/right walls
    if (game->ball.x <= 0 || game->ball.x + game->ball.w >= game->screenW) {
        *ballVelx = -*ballVelx;
    }

    // Collision with top/bottom walls
    if (game->ball.y <= 0 || game->ball.y + game->ball.h >= game->screenH) {
        *ballVely = -*ballVely;
    }
}



void render(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    SDL_SetRenderDrawColor(game->renderer, 212, 115, 78, 255); // orange
    drawRoundedRect(game->renderer,&game->player1,10.0f);

    SDL_SetRenderDrawColor(game->renderer, 50, 205, 218, 235); // turquoise
    drawRoundedRect(game->renderer,&game->player2,10.0f);

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    drawFilledCircle(
        game->renderer,
        game->ball.x + game->ball.w/2,
        game->ball.y + game->ball.h/2,
        game->ball.w/2
    );

    SDL_RenderPresent(game->renderer);
    SDL_Delay(17);
}

void cleanup(Game* game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

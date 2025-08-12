#include "game.h"
#include <stdio.h>

int main() {
    Game game;
    SDL_Init(SDL_INIT_VIDEO);

    game.window = SDL_CreateWindow("ball", 1800, 990, 0);
    game.renderer = SDL_CreateRenderer(game.window, NULL);

    game.player1 = (SDL_FRect){100.0f, 50.0f, 50.0f, 100.0f};
    game.player2 = (SDL_FRect){490.0f, 50.0f, 50.0f, 100.0f};
    game.ball = (SDL_FRect){295.0f, 240.0f, 10.0f, 10.0f};

    SDL_Event event;
    int running = 1;

    float ballVelx = 0, ballVely = 0;
    float ballSpeed = 5.0f;
    int pickupCooldown = 0;

    AttachSide attachSide;
    BallOwner ballOwner = OWNER_NONE;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }

        const bool* keys = SDL_GetKeyboardState(NULL);

        // Player 1 movement
        if (keys[SDL_SCANCODE_LEFT])  game.player1.x -= 5;
        if (keys[SDL_SCANCODE_RIGHT]) game.player1.x += 5;
        if (keys[SDL_SCANCODE_UP])    game.player1.y -= 5;
        if (keys[SDL_SCANCODE_DOWN])  game.player1.y += 5;

        // Player 2 movement
        if (keys[SDL_SCANCODE_A]) game.player2.x -= 5;
        if (keys[SDL_SCANCODE_D]) game.player2.x += 5;
        if (keys[SDL_SCANCODE_W]) game.player2.y -= 5;
        if (keys[SDL_SCANCODE_S]) game.player2.y += 5;

        // Throw mechanics
        if (ballOwner == OWNER_PLAYER1 && keys[SDL_SCANCODE_SPACE]) {
            ballVelx = (attachSide == SIDE_RIGHT) ? ballSpeed : -ballSpeed;
            ballVely = 0;
            pickupCooldown = 0;
            ballOwner = OWNER_NONE;
        }
        if (ballOwner == OWNER_PLAYER2 && keys[SDL_SCANCODE_F]) {
            ballVelx = (attachSide == SIDE_LEFT) ? -ballSpeed : ballSpeed;
            ballVely = 0;
            pickupCooldown = 0;
            ballOwner = OWNER_NONE;
        }

        // Move ball if free
        if (ballOwner == OWNER_NONE) {
            game.ball.x += ballVelx;
            game.ball.y += ballVely;

            ballVelx *= 0.98f;
            ballVely *= 0.98f;
            if (fabsf(ballVelx) < 0.1f && fabsf(ballVely) < 0.1f) {
                ballVelx = ballVely = 0;
            }
        } else {
            if (ballOwner == OWNER_PLAYER1) {
                game.ball.x = (attachSide == SIDE_RIGHT)
                    ? game.player1.x + game.player1.w
                    : game.player1.x - game.ball.w;
                game.ball.y = game.player1.y + game.player1.h / 2 - game.ball.h / 2;
            } else if (ballOwner == OWNER_PLAYER2) {
                game.ball.x = (attachSide == SIDE_LEFT)
                    ? game.player2.x - game.ball.w
                    : game.player2.x + game.player2.w;
                game.ball.y = game.player2.y + game.player2.h / 2 - game.ball.h / 2;
            }
        }

        // Collision detection
        if (ballOwner == OWNER_NONE && pickupCooldown <= 0) {
            if (SDL_HasRectIntersectionFloat(&game.player1, &game.ball)) {
                ballOwner = OWNER_PLAYER1;
                attachSide = (game.ball.x + game.ball.w / 2 < game.player1.x + game.player1.w / 2)
                    ? SIDE_LEFT : SIDE_RIGHT;
            } else if (SDL_HasRectIntersectionFloat(&game.player2, &game.ball)) {
                ballOwner = OWNER_PLAYER2;
                attachSide = (game.ball.x + game.ball.w / 2 < game.player2.x + game.player2.w / 2)
                    ? SIDE_LEFT : SIDE_RIGHT;
            }
        }

        if (pickupCooldown > 0) pickupCooldown--;

        render(&game);
    }

    cleanup(&game);
    return 0;
}

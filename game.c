#include "game.h"
#include "render.h"
#include "utils.h"
#include "helper.h"

void game_init(Game* g, Player_positions* p) {
    SDL_Init(SDL_INIT_VIDEO);

    g->window   = SDL_CreateWindow("Push-Pull (2D baseline)", 800, 600, SDL_WINDOW_RESIZABLE);
    g->renderer = SDL_CreateRenderer(g->window, NULL);
    SDL_GetWindowSize(g->window, &g->screenW, &g->screenH);

    load_player_positions(p);

    // players: keep your old sizes/feel (P1 bottom, P2 top)
    player_init(&g->player1, (SDL_FRect){p->p1x,p->p1y, 50, 100}, 420.0f); //orange
    player_init(&g->player2, (SDL_FRect){p->p2x,p->p2y, 50, 100}, 420.0f); // turqoise

    // ball: keep tiny size; velocities handled in Ball
    ball_init(&g->ball, (SDL_FRect){g->screenW * 0.5f - 5, g->screenH * 0.5f - 5, 10, 10});

    g->running = 1;
}

static void handle_events(Game* g, const bool** keysOut) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) g->running = 0;
        else if (e.type == SDL_EVENT_WINDOW_RESIZED) {
            SDL_GetWindowSize(g->window, &g->screenW, &g->screenH);
        }
    }
    *keysOut = SDL_GetKeyboardState(NULL); // SDL3 -> const bool*
}

void game_run(Game* g) {
    Uint64 lastTick = SDL_GetTicks();
    while (g->running) {
        const bool* keys = NULL;
        handle_events(g, &keys);

        Uint64 now = SDL_GetTicks();
        float dt = (now - lastTick) / 1000.0f; // seconds
        lastTick = now;

        // controls (restore your original mapping)
        // P1: Arrow keys
        player_update(&g->player1, keys,
                      SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
                      SDL_SCANCODE_UP,   SDL_SCANCODE_DOWN,
                      dt, g->screenW, g->screenH);

        // P2: WASD
        player_update(&g->player2, keys,
                      SDL_SCANCODE_A, SDL_SCANCODE_D,
                      SDL_SCANCODE_W, SDL_SCANCODE_S,
                      dt, g->screenW, g->screenH);

        // ball carry/throw + pickup (identical behavior, just modular)
        ball_handle_carry_and_throw(&g->ball, &g->player1, &g->player2, keys);
        ball_try_pickup(&g->ball, &g->player1, &g->player2);
        ball_update(&g->ball, dt, g->screenW, g->screenH, &g->player1, &g->player2);

     
        // draw
        render_begin(g->renderer);
        render_player(g->renderer, &g->player1, (SDL_Color){212,115,78,255});     // orange
        render_player(g->renderer, &g->player2, (SDL_Color){50,205,218,235});     // turquoise
        render_ball(g->renderer, &g->ball, (SDL_Color){255,255,255,255});         // white
        render_end(g->renderer);

        SDL_Delay(1); // be nice to CPU; your frame pacing lives in dt
    }
}

void game_cleanup(Game* g) {
    SDL_DestroyRenderer(g->renderer);
    SDL_DestroyWindow(g->window);
    SDL_Quit();
}

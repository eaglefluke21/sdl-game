#ifndef ENTITIES_H
#define ENTITIES_H

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_FRect rect;
    float speed; // px/s
} Player;

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
    SDL_FRect rect;
    float vx, vy;        
    float frictionPerSec; 
    int   pickupCooldown; 
    BallOwner owner;
    AttachSide attach;
} Ball;

// Players
void player_init(Player* p, SDL_FRect rect, float speed);
void player_update(Player* p, const bool* keys,
                   SDL_Scancode left, SDL_Scancode right,
                   SDL_Scancode up,   SDL_Scancode down,
                   float dt, int screenW, int screenH);

// Ball
void ball_init(Ball* b, SDL_FRect rect);

// carries, throws, pickups (restored behavior)
void ball_handle_carry_and_throw(Ball* b, const Player* p1, const Player* p2, const bool* keys);
void ball_try_pickup(Ball* b, const Player* p1, const Player* p2);

// physics & wall bounce; also keeps attached position when carried
void ball_update(Ball* b, float dt, int screenW, int screenH, const Player* p1, const Player* p2);

#endif

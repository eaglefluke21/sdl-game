#include "entities.h"
#include <math.h> // fabsf, powf

// ---------------- Players ----------------
void player_init(Player* p, SDL_FRect rect, float speed) {
    p->rect = rect;
    p->speed = speed;
}

void player_update(Player* p, const bool* keys,
                   SDL_Scancode left, SDL_Scancode right,
                   SDL_Scancode up,   SDL_Scancode down,
                   float dt, int screenW, int screenH)
{
    float dx = 0.f, dy = 0.f;
    if (keys[left])  dx -= 1.f;
    if (keys[right]) dx += 1.f;
    if (keys[up])    dy -= 1.f;
    if (keys[down])  dy += 1.f;

    p->rect.x += dx * p->speed * dt;
    p->rect.y += dy * p->speed * dt;

    // clamp to window
    if (p->rect.x < 0) p->rect.x = 0;
    if (p->rect.y < 0) p->rect.y = 0;
    if (p->rect.x + p->rect.w > screenW) p->rect.x = screenW - p->rect.w;
    if (p->rect.y + p->rect.h > screenH) p->rect.y = screenH - p->rect.h;
}

// ---------------- Ball ----------------
static AttachSide pick_attach_side(const SDL_FRect* player, const SDL_FRect* ball) {
    float ballCenter  = ball->x + ball->w * 0.5f;
    float playerCenter= player->x + player->w * 0.5f;
    return (ballCenter < playerCenter) ? SIDE_LEFT : SIDE_RIGHT;
}

void ball_init(Ball* b, SDL_FRect rect) {
    b->rect = rect;
    b->vx = 0.f;
    b->vy = 0.f;
    b->frictionPerSec = 0.60f; // ~match your 0.98 per frame feel at ~60fps
    b->pickupCooldown = 0;
    b->owner = OWNER_NONE;
    b->attach = SIDE_RIGHT;
}

static void attach_to_player(Ball* b, const Player* p, AttachSide side) {
    if (side == SIDE_RIGHT) {
        b->rect.x = p->rect.x + p->rect.w;
    } else {
        b->rect.x = p->rect.x - b->rect.w;
    }
    b->rect.y = p->rect.y + p->rect.h * 0.5f - b->rect.h * 0.5f;
}

void ball_handle_carry_and_throw(Ball* b, const Player* p1, const Player* p2, const bool* keys) {
    const float throwSpeed = 300.0f; // px/s, akin to your ballSpeed=5 with dt scaling

    if (b->owner == OWNER_PLAYER1) {
        // keep attached
        attach_to_player(b, p1, b->attach);
        if (keys[SDL_SCANCODE_SPACE]) {
            b->vx = (b->attach == SIDE_RIGHT) ? +throwSpeed : -throwSpeed;
            b->vy = 0.f;
            b->owner = OWNER_NONE;
            b->pickupCooldown = 10;
        }
    } else if (b->owner == OWNER_PLAYER2) {
        attach_to_player(b, p2, b->attach);
        if (keys[SDL_SCANCODE_F]) {
            b->vx = (b->attach == SIDE_LEFT) ? -throwSpeed : +throwSpeed;
            b->vy = 0.f;
            b->owner = OWNER_NONE;
            b->pickupCooldown = 10;
        }
    }
}

void ball_try_pickup(Ball* b, const Player* p1, const Player* p2) {
    if (b->owner != OWNER_NONE || b->pickupCooldown > 0) return;

    if (SDL_HasRectIntersectionFloat(&p1->rect, &b->rect)) {
        b->owner = OWNER_PLAYER1;
        b->attach = pick_attach_side(&p1->rect, &b->rect);
        b->vx = 0.f; b->vy = 0.f;
    } else if (SDL_HasRectIntersectionFloat(&p2->rect, &b->rect)) {
        b->owner = OWNER_PLAYER2;
        b->attach = pick_attach_side(&p2->rect, &b->rect);
        b->vx = 0.f; b->vy = 0.f;
    }
}

void ball_update(Ball* b, float dt, int screenW, int screenH, const Player* p1, const Player* p2) {
    if (b->pickupCooldown > 0) b->pickupCooldown--;

    if (b->owner == OWNER_NONE) {
        // free flight
        b->rect.x += b->vx * dt;
        b->rect.y += b->vy * dt;

        // friction as per second factor (dt-aware)
        float damp = powf(b->frictionPerSec, dt); // e.g., 0.60^dt
        b->vx *= damp;
        b->vy *= damp;
        if (fabsf(b->vx) < 5e-2f) b->vx = 0.f;
        if (fabsf(b->vy) < 5e-2f) b->vy = 0.f;

        // wall bounce
        if (b->rect.x <= 0)                { b->rect.x = 0;                       b->vx = -b->vx; }
        if (b->rect.x + b->rect.w >= screenW){ b->rect.x = screenW - b->rect.w;   b->vx = -b->vx; }
        if (b->rect.y <= 0)                { b->rect.y = 0;                       b->vy = -b->vy; }
        if (b->rect.y + b->rect.h >= screenH){ b->rect.y = screenH - b->rect.h;   b->vy = -b->vy; }
    } else {
        // snap to carrier
        if (b->owner == OWNER_PLAYER1) attach_to_player(b, p1, b->attach);
        else                           attach_to_player(b, p2, b->attach);
    }
}

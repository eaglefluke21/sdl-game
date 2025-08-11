#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>  

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("ball",1800,990,0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,NULL);

    SDL_FRect player1 = {100.0f,50.0f,50.0f,100.0f};
    SDL_FRect player2 = {490.0f,50.0f,50.0f,100.0f};
    SDL_FRect ball = {295.0f,240.0f,10.0f,10.0f};

    SDL_Event event;
    int running = 1;
    int ballHeldby = 1;

    float ballVelx = 0, ballVely = 0;
    float ballSpeed = 5.0f;

    typedef enum {
        OWNER_NONE,
        OWNER_PLAYER1,
        OWNER_PLAYER2
    }  BallOwner;

    typedef enum{
        SIDE_LEFT,
        SIDE_RIGHT
    } AttachSide;

    AttachSide attachSide;

    BallOwner ballOwner = OWNER_NONE;

    int pickupCooldown = 0;
    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = 0;
            }else if(event.type == SDL_EVENT_KEY_DOWN){
                        SDL_Keycode key = event.key.key;

               if(key == SDLK_SPACE){
                        ballOwner = (ballOwner == OWNER_PLAYER1)? OWNER_PLAYER2:OWNER_PLAYER1;
                    }
               if(key == SDLK_F){
                        ballOwner = (ballOwner == OWNER_PLAYER2)? OWNER_PLAYER1:OWNER_PLAYER2;
                    }
                }
                 
            }
                const bool* keys = SDL_GetKeyboardState(NULL);


        if (keys[SDL_SCANCODE_LEFT])  player1.x -= 5;
        if (keys[SDL_SCANCODE_RIGHT]) player1.x += 5;
        if (keys[SDL_SCANCODE_UP])    player1.y -= 5;
        if (keys[SDL_SCANCODE_DOWN])  player1.y += 5;

        if (keys[SDL_SCANCODE_A]) player2.x -= 5;
        if (keys[SDL_SCANCODE_D]) player2.x += 5;
        if (keys[SDL_SCANCODE_W]) player2.y -= 5;
        if (keys[SDL_SCANCODE_S]) player2.y += 5;


         if (ballOwner == OWNER_PLAYER1 && keys[SDL_SCANCODE_SPACE]) {
         ballVelx = (attachSide == SIDE_RIGHT) ? ballSpeed : -ballSpeed;
        ballVely = 0;
        pickupCooldown = 120; 
        ballOwner = OWNER_NONE;
        
    }
    if (ballOwner == OWNER_PLAYER2 && keys[SDL_SCANCODE_F]) {
        ballVelx = (attachSide == SIDE_LEFT) ? -ballSpeed : ballSpeed;
         ballVely = 0;
         pickupCooldown = 120;
        ballOwner = OWNER_NONE;
       
    }

    // Move ball if free
    if (ballOwner == OWNER_NONE) {
        ball.x += ballVelx;
        ball.y += ballVely;

        ballVelx *= 0.98f;
        ballVely *= 0.98f;
        if (fabs(ballVelx) < 0.1f && fabs(ballVely) < 0.1f) {
            ballVelx = ballVely = 0;
        }
    } else {
        // Stick ball to player
        if (ballOwner == OWNER_PLAYER1) {
              if (attachSide == SIDE_RIGHT) {
        ball.x = player1.x + player1.w;
    } else {
        ball.x = player1.x - ball.w;
    }
            ball.y = player1.y + player1.h/2 - ball.h/2;
        } else if (ballOwner == OWNER_PLAYER2) {
            if (attachSide == SIDE_LEFT) {
        ball.x = player2.x - ball.w;
    } else {
        ball.x = player2.x + player2.w;
    }
            ball.y = player2.y + player2.h/2 - ball.h/2;
        }
    }

    // Collision (only if ball free and cooldown over)
    if (ballOwner == OWNER_NONE && pickupCooldown <= 0) {
        if (SDL_HasRectIntersectionFloat(&player1, &ball)) {
              if (pickupCooldown <= 0) {
        ballOwner = OWNER_PLAYER1;
        attachSide = (ball.x + ball.w/2 < player1.x + player1.w/2) ? SIDE_LEFT : SIDE_RIGHT;
    }
        } else if (SDL_HasRectIntersectionFloat(&player2, &ball)) {
               if (pickupCooldown <= 0) {
        ballOwner = OWNER_PLAYER2;
        attachSide = (ball.x + ball.w/2 < player2.x + player2.w/2) ? SIDE_LEFT : SIDE_RIGHT;
    }
        }
    }

    if (pickupCooldown > 0) pickupCooldown--;

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,212,115,78,255); //orange
    SDL_RenderFillRect(renderer,&player1);

    SDL_SetRenderDrawColor(renderer,50,205,218,235); //turquoise
    SDL_RenderFillRect(renderer,&player2);
    
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&ball);

    SDL_RenderPresent(renderer);
    SDL_Delay(17);

    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
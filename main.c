#include <SDL3/SDL.h>
#include <stdio.h>
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

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = 0;
            }else if(event.type == SDL_EVENT_KEY_DOWN){
                SDL_Keycode key = event.key.key;
                switch(key){
                    case SDLK_LEFT:
                    player1.x -= 10;
                    break;
                    case SDLK_RIGHT:
                    player1.x += 10;
                    break;
                    case SDLK_UP:
                    player1.y -= 10;
                    break;
                    case SDLK_DOWN:
                    player1.y += 10;
                    break;
                     case SDLK_A:
                    player2.x -= 10;
                    break;
                    case SDLK_D:
                    player2.x += 10;
                    break;
                    case SDLK_W:
                    player2.y -= 10;
                    break;
                    case SDLK_S:
                    player2.y += 10;
                    break;

                }
                 if(key == SDLK_SPACE){
                        if(ballHeldby == 1){
                            ballHeldby = 2;
                        }else{
                            ballHeldby = 1;
                        }
                    }
            }
        }

        if(ballHeldby == 1){
            ball.x = player1.x + player1.w;
            ball.y = player1.y + player1.h/2;
        } else if (ballHeldby == 2) {
            ball.x = player2.x - ball.w;
            ball.y = player2.y + player2.h/2;

        }
        
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,212,115,78,255);
    SDL_RenderFillRect(renderer,&player1);

    SDL_SetRenderDrawColor(renderer,40,65,118,255);
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

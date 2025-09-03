#include "helper.h"
#include <stdio.h>
int load_player_positions(Player_positions* p){
    FILE* f  = fopen("config.txt","r");

    if(f){
        fscanf(f,"player1_x=%d\n",&p->p1x);
        fscanf(f,"player2_x=%d\n",&p->p2x);
        fscanf(f,"player1_y=%d\n",&p->p1y);
        fscanf(f,"player2_y=%d\n",&p->p2y);
        fclose(f);
    }else{
         p->p1x = 100; p->p1y = 500;
        p->p2x = 600; p->p2y = 50;
    }
    return 0;
}

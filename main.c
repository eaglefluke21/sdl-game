#include "game.h"

int main() {
    Game g;
    Player_positions p;
    game_init(&g,&p);
    game_run(&g);
    game_cleanup(&g);
    return 0;
}

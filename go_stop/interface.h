#include <stdio.h>
#include "card.h"
#include "score.h"

#define MAX_PLAYER	3

void print_scr(Game *game,int turn);

int save(Game *game);
int load(Game *game);

int reset_game(Game *game);

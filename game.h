#ifndef GAME_H
#define GAME_H

#include "map.h"

void handleInput(Map *map, char input);
void rotateEnemies(Map *map);
void movePlayer(Map *map, char dir);

#endif
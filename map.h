#ifndef MAP_H
#define MAP_H

typedef struct {
    int row, col, dir;
} Enemy;

typedef struct {
    char **grid;
    int rows;
    int cols;
    int playerRow;
    int playerCol;
    int gameOver;
    int enemyCount;
    Enemy *enemies;
} Map;

Map *loadMap(const char *filename);
Map *copyMap(Map *original);       /* <- Add this line */
void printMap(Map *map);
void freeMap(Map *map);
char getEnemyChar(int dir);

#endif

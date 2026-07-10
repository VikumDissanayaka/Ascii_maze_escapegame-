#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* return character representation of an enemy facing direction*/
char getEnemyChar(int dir) {
    if (dir == 0) return '^';
    if (dir == 1) return '>';
    if (dir == 2) return 'v';
    return '<';
}

/* Load man data from a file and initialize the map struture*/
Map *loadMap(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return NULL;

    srand(time(NULL));/*seed for random directions*/

    int r, c, e = 0;
    int i, j;
    fscanf(fp, "%d %d", &r, &c);/*read rows and colums*/
    Map *map = (Map *)malloc(sizeof(Map));
    map->rows = r + 2;
    map->cols = c + 2;
    map->gameOver = 0;

    /*allocate 2d grid and fill with spaces/walls*/
    map->grid = (char **)malloc(map->rows * sizeof(char *));
    for (i = 0; i < map->rows; i++) {
        map->grid[i] = (char *)malloc(map->cols);
        for (j = 0; j < map->cols; j++) {
            if (i == 0 || i == map->rows - 1 || j == 0 || j == map->cols - 1)
                map->grid[i][j] = '*';
            else
                map->grid[i][j] = ' ';
        }
    }

    map->enemies = (Enemy *)malloc(sizeof(Enemy) * r * c);

    /* fill grid from file and record player */
    for (i = 1; i <= r; i++) {
        for (j = 1; j <= c; j++) {
            int val;
            fscanf(fp, "%d", &val);
            switch (val) {
                case 0: map->grid[i][j] = ' '; break;
                case 1: map->grid[i][j] = 'P'; map->playerRow = i; map->playerCol = j; break;
                case 2: map->grid[i][j] = 'G'; break;
                case 3: map->grid[i][j] = 'O'; break;
                case 4:
                    map->enemies[e].row = i;
                    map->enemies[e].col = j;
                    map->enemies[e].dir = rand() % 4;
                    map->grid[i][j] = getEnemyChar(map->enemies[e].dir);
                    e++;
                    break;
            }
        }
    }

    map->enemyCount = e;
    fclose(fp);
    return map;
}

/* print the current map to terminal with player/enemy symbols*/
void printMap(Map *map)
{
    int i, j;
    for (i = 0; i < map->rows; i++) {
        for (j = 0; j < map->cols; j++) {
            printf("%c", map->grid[i][j]);
        }
        printf("\n");
    }
    printf("Press 'w' to move up\nPress 's' to move down\n");
    printf("Press 'a' to move left\nPress 'd' to move right\n");
    printf("Press 'u' to undo\n");
}

void freeMap(Map *map)
{
    int i;
    for (i = 0; i < map->rows; i++) {
        free(map->grid[i]);
    }
    free(map->grid);
    free(map->enemies);
    free(map);
}


Map *copyMap(Map *original)
{
    if (original == NULL) return NULL;

    Map *copy = (Map *)malloc(sizeof(Map));
    int i;

    /* Copy basic attributes */
    copy->rows = original->rows;
    copy->cols = original->cols;
    copy->playerRow = original->playerRow;
    copy->playerCol = original->playerCol;
    copy->gameOver = original->gameOver;
    copy->enemyCount = original->enemyCount;

    /* Allocate memory for grid */
    copy->grid = (char **)malloc(sizeof(char *) * copy->rows);
    for (i = 0; i < copy->rows; i++) {
        copy->grid[i] = (char *)malloc(sizeof(char) * copy->cols);
        memcpy(copy->grid[i], original->grid[i], sizeof(char) * copy->cols);
    }

    /* Copy enemies */
    copy->enemies = (Enemy *)malloc(sizeof(Enemy) * copy->enemyCount);
    for (i = 0; i < copy->enemyCount; i++) {
        copy->enemies[i] = original->enemies[i];
    }

    return copy;
}

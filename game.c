#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "map.h"
#include "game.h"
#include "terminal.h"
#include "undo.h"

extern UndoStack *undoStack;
/* create a deep copy of the map */

void *copyMapWrapper(void *original) {
    return (void *)copyMap((Map *)original);
}


/* Free a map (used if needed in freeUndoStack) */
void freeMapWrapper(void *map) {
    freeMap((Map *)map);
}

/* Check if the player was  seen by a  enemyin the game */
int isPlayerSeen(Map *map, Enemy *enemy)
{
    int r = enemy->row;
    int c = enemy->col;
    int pr = map->playerRow;
    int pc = map->playerCol;
    int i;

    /*facing up*/
    if (enemy->dir == 0 && c == pc && pr < r) {
        for (i = r - 1; i > pr; i--) {
            if (map->grid[i][c] == 'O') return 0;
        }
        return 1;
    }

    /*facing right*/
    if (enemy->dir == 1 && r == pr && pc > c) {
        for (i = c + 1; i < pc; i++) {
            if (map->grid[r][i] == 'O') return 0;
        }
        return 1;
    }

    /*facing down*/
    if (enemy->dir == 2 && c == pc && pr > r) {
        for (i = r + 1; i < pr; i++) {
            if (map->grid[i][c] == 'O') return 0;
        }
        return 1;
    }

    /*facing left*/
    if (enemy->dir == 3 && r == pr && pc < c) {
        for (i = c - 1; i > pc; i--) {
            if (map->grid[r][i] == 'O') return 0;
        }
        return 1;
    }

    return 0;
}

/* Enemy chases player step by step */
void chasePlayer(Map *map, Enemy *enemy)
{
    int r = enemy->row;
    int c = enemy->col;
    int pr = map->playerRow;
    int pc = map->playerCol;

    while (r != pr || c != pc) {
        map->grid[r][c] = ' ';

        if (r < pr) r++;
        else if (r > pr) r--;
        else if (c < pc) c++;
        else if (c > pc) c--;

        map->grid[r][c] = getEnemyChar(enemy->dir);
        clearScreen();
        printMap(map);
        sleep(1);

        if (r == pr && c == pc) {
            enemy->row = r;
            enemy->col = c;
            map->gameOver = 1;
            printf("You lose!\n");
            break;
        }
    }
}

/* Rotate all enemies 90 degrees clockwise */
void rotateEnemies(Map *map)
{
    int i, r, c;
    for (i = 0; i < map->enemyCount; i++) {
        r = map->enemies[i].row;
        c = map->enemies[i].col;
        map->grid[r][c] = ' ';
        map->enemies[i].dir = (map->enemies[i].dir + 1) % 4;
        map->grid[r][c] = getEnemyChar(map->enemies[i].dir);
    }
}

/* Move the player in the specific direction */
void movePlayer(Map *map, char dir)
{
    int dr = 0, dc = 0;
    if (dir == 'w') dr = -1;
    else if (dir == 's') dr = 1;
    else if (dir == 'a') dc = -1;
    else if (dir == 'd') dc = 1;

    int nr = map->playerRow + dr;
    int nc = map->playerCol + dc;

    if (map->grid[nr][nc] == ' ' || map->grid[nr][nc] == 'G') {
        if (map->grid[nr][nc] == 'G') {
            printf("You win!\n");
            map->gameOver = 1;
        }
        map->grid[map->playerRow][map->playerCol] = ' ';
        map->grid[nr][nc] = 'P';
        map->playerRow = nr;
        map->playerCol = nc;
    }
}

/* Handle player input and update the game state */
void handleInput(Map *map, char input)
{
    int i;
    if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
        pushState(undoStack, map, copyMapWrapper);  /* Use generic undo*/
        movePlayer(map, input);
        rotateEnemies(map);

        for (i = 0; i < map->enemyCount; i++) {
            if (isPlayerSeen(map, &map->enemies[i])) {
                chasePlayer(map, &map->enemies[i]);
                break;
            }
        }
    } else if (input == 'u') {
        Map *previous = (Map *)popState(undoStack);
        if (previous != NULL) {
            for (i = 0; i < map->rows; i++) {
                free(map->grid[i]);
            }
            free(map->grid);
            free(map->enemies);

            *map = *previous;
            free(previous);
        }
    }
}

#include <stdio.h>
#include "game.h"
#include "map.h"
#include "terminal.h"
#include "undo.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* global undo stack */
UndoStack *undoStack = NULL;
/* entry point of the programme*/
int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <map_file>\n", argv[0]);
        return 1;
    }

    initTerminal();/*set up termina for raw input*/

    /* allocate memory for welcome message at the begining of the programme*/
    char *welcomeMsg = (char *)malloc(50 * sizeof(char));
    if (welcomeMsg != NULL) {
        strcpy(welcomeMsg, "Welcome to the ASCII adventure Game!");
        printf("%s\n", welcomeMsg);
        sleep(2);
        free(welcomeMsg);
    }

    undoStack = createUndoStack();/*initialize udo stack*/

    /* load map from the file */
    Map *map = loadMap(argv[1]);
    if (map == NULL) {
        printf("Failed to load map.\n");
        return 1;
    }
/*main game loop*/
    while (!map->gameOver) {
        clearScreen();
        printMap(map);
        char input = getInput();/* take single inputs fro the use*/
        handleInput(map, input);/*process player input and trigger gae events*/
    }

    /* reset terminal before exiting */
    resetTerminal();

    /* free undo stack and map memory */
    freeUndoStack(undoStack, (void (*)(void *))freeMap);
    freeMap(map);

    return 0;
}

#include "terminal.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

/* used to store original and modified terminal settings*/
static struct termios oldt, newt;

/*set terminal to raw mode*/ 
void initTerminal()
{
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

/* restore original terminal settings*/ 
void resetTerminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
/*clear terminal screen using ansi escape codes*/
void clearScreen()
{
    printf("\033[2J\033[1;1H");
}
/* read one character from the output*/
char getInput()
{
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}

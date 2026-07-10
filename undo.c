#include <stdlib.h>
#include "undo.h"
/*creates and initialize an  new empty undo stack*/

UndoStack *createUndoStack()
{
    UndoStack *stack = (UndoStack *)malloc(sizeof(UndoStack));
    stack->top = NULL;
    return stack;
}


/*pushes a deep copy f game state onto the undo stack*/
void pushState(UndoStack *stack, void *data, void *(*copyFunc)(void *))
{
    UndoNode *node = (UndoNode *)malloc(sizeof(UndoNode));
    node->snapshot = copyFunc(data);  /* generic deep copy*/
    node->next = stack->top;
    stack->top = node;
}


/*pop the most recent state from the stack*/
void *popState(UndoStack *stack)
{
    if (stack->top == NULL)
        return NULL;

    UndoNode *node = stack->top;
    void *data = node->snapshot;
    stack->top = node->next;
    free(node);
    return data;
}


/*free all nodes in the stack along with their data*/
void freeUndoStack(UndoStack *stack, void (*freeFunc)(void *))
{
    while (stack->top != NULL) {
        void *data = popState(stack);
        freeFunc(data);
    }
    free(stack);
}

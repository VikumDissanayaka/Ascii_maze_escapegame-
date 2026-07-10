#ifndef UNDO_H
#define UNDO_H

typedef struct UndoNode {
    void *snapshot;
    struct UndoNode *next;
} UndoNode;

typedef struct UndoStack {
    UndoNode *top;
} UndoStack;

UndoStack *createUndoStack();
void pushState(UndoStack *stack, void *data, void *(*copyFunc)(void *));
void *popState(UndoStack *stack);
void freeUndoStack(UndoStack *stack, void (*freeFunc)(void *));

#endif

#include "list_handling.h"
#include "engine.h"

void append(struct snake* s)
{
    struct node* newTail = (struct node*)malloc(sizeof(struct node));
    validateMalloc(newTail, TYPE_NODE);
    newTail->next = NULL;
    newTail->newX = s->head->newX;
    newTail->newY = s->head->newY;

    if (s->head == s->tail) {
        newTail->prev = s->head;
        s->head->next = newTail;
        s->tail = newTail;
    } else {
        newTail->newX = s->tail->x;
        newTail->newY = s->tail->y;

        s->tail->next = newTail;
        newTail->prev = s->tail;

        s->tail = newTail;
    }
}

void freeSnakeNodes(struct snake* s)
{
    struct node* element;
    while ((element = s->head) != NULL) {
        s->head = s->head->next;
        free(element);
    }
}

void freeGameSessionMemory(struct snake* snake_ptr, struct point* point_ptr)
{
    freeSnakeNodes(snake_ptr);
    free(snake_ptr);
    free(point_ptr);
}

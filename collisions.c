#include "collisions.h"

extern void gameOverSound();

bool collisionWithBordersOccurred(struct snake* s)
{
    if (s->head->newX >= map_size_x - 1 || s->head->newX <= 0 || s->head->newY >= (map_size_y - 1) || s->head->newY <= 0)
        return true;
    else
        return false;
}

bool selfCollision(struct snake* s, struct node* element)
{
    if ((s->head->x == element->newX) && (s->head->y == element->newY))
        return true;
    else
        return false;
}

void checkForSelfCollision(struct snake* s, struct node* element)
{
    if (s->length > 3) {
        if (selfCollision(s, element)) {
            gameOverSound();
            s->isAlive = false;
        }
    }
}

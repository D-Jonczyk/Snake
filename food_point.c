#include "food_point.h"

void randomFoodPoint(struct snake* s, struct point* p, char (*ptr)[map_size_y])
{
    p->x = (rand() % (map_size_x - 2)) + 1;
    p->y = (rand() % (map_size_y - 2)) + 1;
    if (s->fieldsOccupiedBySnake[p->x][p->y]) //checks if point has spawned at snake's actual position
        randomFoodPoint(s, p, ptr);
}

void printFoodPoint(struct point* p, char (*ptr)[map_size_y])
{
    ptr[p->x][p->y] = FOOD_TEXTURE;
}

bool scoredPoint(struct snake* s, struct point* p)
{
    if (s->head->newX == p->x && s->head->newY == p->y)
        return true;

    return false;
}

void pointScoredSound()
{
    Beep(300, 30);
}

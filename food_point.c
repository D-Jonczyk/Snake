#include "food_point.h"

void randomFoodPoint(struct snake *s, struct point *p, char (*ptr)[map_size_y])
{
	p->x = ( rand() % (map_size_x-2) ) + 1;
	p->y = ( rand() % (map_size_y-2) ) + 1;
	if(s->field[p->x][p->y] ) //checks if point spawned at snake's actual position
		randomFoodPoint(s,p,ptr);
}

void printFoodPoint(struct point *p, char (*ptr)[map_size_y])
{
	ptr[p->x][p->y] = FOOD_TEXTURE;
}

bool scoredPoint(struct snake *s, struct point *p)
{
	if(s->head->_x == p->x && s->head->_y == p->y)
		return true;
	return false;
}

void pointScoredSound(){
    Beep(300,30);
}

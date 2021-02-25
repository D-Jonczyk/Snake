#include "food_point.h"

void random_point(struct snake *s, struct point *p, char (*ptr)[map_size_y]) //randomuje punkt i sprawdza czy pojawil sie na wezu
{
	p->x = ( rand() % (map_size_x-2) ) + 1;
	p->y = ( rand() % (map_size_y-2) ) + 1;
	if(s->field[p->x][p->y] )
		random_point(s,p,ptr);
}

void set_point(struct point *p, char (*ptr)[map_size_y])
{
	ptr[p->x][p->y] = 158;
}

bool compare_point(struct snake *s, struct point *p)
{
	if(s->head->_x == p->x && s->head->_y == p->y)
		return true;
	return false;
}

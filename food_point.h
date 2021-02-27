#pragma once

#include "global_configs.h"

void randomFoodPoint(struct snake* s, struct point* p, char (*ptr)[map_size_y]);
bool scoredPoint(struct snake* s, struct point* p);
void printFoodPoint(struct point* p, char (*ptr)[map_size_y]);
void pointScoredSound();

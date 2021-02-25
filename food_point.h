#pragma once

#include "global_configs.h"

void random_point(struct snake *s, struct point *p, char (*ptr)[map_size_y]);
bool compare_point(struct snake *s, struct point *p);
void set_point(struct point *p, char (*ptr)[map_size_y]);
void pointScoredSound();

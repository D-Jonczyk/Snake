#pragma once

#include "global_configs.h"

void checkForSelfCollision(struct snake*, struct node*);
bool collisionWithBordersOccurred(struct snake*);
bool selfCollision(struct snake*, struct node*);

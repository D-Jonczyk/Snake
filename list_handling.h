#pragma once

#include "global_configs.h"

void append(struct snake*);
void freeSnakeNodes(struct snake*);
void freeGameSessionMemory(struct snake*,struct point*);

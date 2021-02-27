#pragma once

#include "dialogs.h"

void initConsoleParameters();
void fillTheBoardWithAscii(char (*)[map_size_y]);
void initSnake(struct snake* s);
void printSnakeAtStartPos(char (*)[map_size_y], struct snake* s);

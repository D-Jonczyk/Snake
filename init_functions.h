#ifndef INIT_FUNCTIONS_H_INCLUDED
#define INIT_FUNCTIONS_H_INCLUDED

#include "global_configs.h"
#include "dialogs.h"

void initConsoleParameters();
void fillTheBoardWithAscii(char (*)[map_size_y]);
void initSnake(struct snake* s);
void printSnakeAtStartPos(char (*)[map_size_y], struct snake* s);

#endif // INIT_FUNCTIONS_H_INCLUDED

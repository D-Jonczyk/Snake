#pragma once

#include "global_configs.h"

char processKeyboardInput(char);

void programLoop(char (*board_ptr)[map_size_y]);
void gameLoop(struct snake *snake_ptr, struct point *point_ptr, char (*board_ptr)[map_size_y]);

void updateSnakeMoveDirection(int, struct snake*, char (*)[map_size_y]);
void movement(char (*)[map_size_y], struct snake* s, int move_x, int move_y);
void append(struct snake* s);

void printBoard(char (*)[map_size_y]);
void addPoints(struct snake* s);
void wait(short int);

bool pressedWrongKey(char);
bool wrongTurnAngle(char, char);

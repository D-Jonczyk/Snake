#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#define PAUSE_KEY 112
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

//ascii characters for board drawing
#define TOP_LEFT_CORNER 201
#define BOT_LEFT_CORNER 200
#define TOP_RIGHT_CORNER 187
#define BOT_RIGHT_CORNER 188
#define HORIZONTAL_BORDER 219
#define VERTICAL_BORDER 186
#define BOARD_FILL 255
#define SNAKE_TEXTURE 178
#define FOOD_TEXTURE 158

#define map_size_x 15
#define map_size_y 25

//difficulty levels
#define easy  18
#define medium  14
#define hard  10

COORD gameOverPosition;
COORD startPosition;

struct point{
	int x,y;
};

struct node{
	struct node *prev;
	struct node *next;
	int x,y, newX, newY;
};

struct snake{
	struct node *head;
	struct node *tail;

	long int score;
	short int length,last_x,last_y;

	bool fieldsOccupiedBySnake[map_size_x][map_size_y];
	bool isAlive;
};

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

#define map_size_x 15
#define map_size_y 25

//difficulty levels
#define easy  18
#define medium  14
#define hard  10

COORD game_over;

struct point{
	int x,y;
};

struct node{
	struct node *prev;
	struct node *next;
	int x,y, _x, _y;
};

struct snake{
	struct node *head;
	struct node *tail;
	long int score;
	short int count,last_x,last_y;
	bool field[map_size_x][map_size_y]; //miejsca w ktorych aktualnie znajduje sie waz
	bool status; //status gry(false=game over)
};

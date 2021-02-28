//    Snake based on doubly linked list

#include "engine.h"
#include "global_configs.h"
#include "init_functions.h"

void gameOverSound();

extern int difficulty;

int main(void)
{
    srand(time(NULL));
    difficulty = easy;

    char board[map_size_x][map_size_y];
    char(*board_ptr)[map_size_y] = board;

    initConsoleParameters();
    programLoop(board_ptr);
}

void gameOverSound() { Beep(100, 400); }

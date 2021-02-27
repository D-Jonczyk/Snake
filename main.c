//    Snake based on doubly linked list

#include <string.h>

#include "dialogs.h"
#include "engine.h"
#include "food_point.h"
#include "global_configs.h"
#include "init_functions.h"

void freeSnakeNodes(struct snake* s);
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

void freeSnakeNodes(struct snake* s)
{
    struct node* element;
    while ((element = s->head) != NULL) {
        s->head = s->head->next;
        free(element);
    }
}

void gameOverSound() { Beep(100, 400); }

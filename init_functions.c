#include "global_configs.h"
#include "init_functions.h"
#include "engine.h"

extern void append(struct snake* s);

void initSnake(struct snake* s)
{
    int i, j;
    struct node* head = (struct node*)malloc(sizeof(struct node));
    validateMalloc(head, TYPE_NODE);
    head->x = 5;
    head->y = 5;
    head->newX = 5;
    head->newY = 5;
    s->last_x = 5;
    s->last_y = 5;
    head->next = NULL;
    head->prev = NULL;
    s->head = head;
    s->tail = head;
    s->length = 1;
    s->score = 1;
    s->isAlive = true;
    append(s);
    for (i = 0; i < map_size_x; i++)
        for (j = 0; j < map_size_y; j++)
            s->fieldsOccupiedBySnake[i][j] = false;
    s->fieldsOccupiedBySnake[5][5] = true;
}

void initConsoleParameters()
{
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    char title[30];
    strcpy(title, "Snake_");
    strcat(title, GAME_VERSION);
    SetConsoleTitle(title);

    SMALL_RECT windowSize = { 0, 0, 25, 17 };
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    COORD bufferSize = { 26, 18 };

    SetConsoleScreenBufferSize(wHnd, bufferSize);

    startPosition.X = 0;
    startPosition.Y = 0;

    gameOverPosition.X = 7;
    gameOverPosition.Y = 3;

    printMainMenu(); // without it there are black spots in unused area of the
        // console
    system("cls");
}

void fillTheBoardWithAscii(char (*ptr)[map_size_y])
{
    unsigned int i, j;
    for (i = 0; i < map_size_x; i++) {
        for (j = 0; j < map_size_y; j++) {
            if (i == 0 || i == (map_size_x - 1))
                ptr[i][j] = HORIZONTAL_BORDER;
            else if (j == 0 || j == (map_size_y - 1))
                ptr[i][j] = VERTICAL_BORDER;
            else
                ptr[i][j] = BOARD_FILL;

            if (i == 0 && j == 0)
                ptr[i][j] = TOP_LEFT_CORNER;
            if (i == 0 && j == (map_size_y - 1))
                ptr[i][j] = TOP_RIGHT_CORNER;
            if (i == (map_size_x - 1) && j == 0)
                ptr[i][j] = BOT_LEFT_CORNER;
            if (i == (map_size_x - 1) && j == (map_size_y - 1))
                ptr[i][j] = BOT_RIGHT_CORNER;
        }
    }
}

void printSnakeAtStartPos(
    char (*ptr)[map_size_y],
    struct snake* s)
{
    ptr[5][5] = SNAKE_TEXTURE;
}

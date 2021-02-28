#include "collisions.h"
#include "dialogs.h"
#include "engine.h"
#include "food_point.h"
#include "init_functions.h"
#include "list_handling.h"

extern void gameOverSound();
extern void freeSnakeNodes(struct snake*);

int difficulty;

void programLoop(char (*board_ptr)[map_size_y])
{
    char choice = 0;
    while (choice != '3') {
        printMainMenu();
        processUserChoice();

        struct snake* snake_ptr = (struct snake*)malloc(
            sizeof(struct snake) * (sizeof(struct node) * 100));
        struct point* point_ptr = (struct point*)malloc(sizeof(struct point));

        fillTheBoardWithAscii(board_ptr);
        initSnake(snake_ptr);
        printSnakeAtStartPos(board_ptr, snake_ptr);
        randomFoodPoint(snake_ptr, point_ptr, board_ptr);
        printFoodPoint(point_ptr, board_ptr);
        printBoard(board_ptr);

        gameLoop(snake_ptr, point_ptr, board_ptr);

    } // end of the program
}

void gameLoop(struct snake* snake_ptr, struct point* point_ptr,
    char (*board_ptr)[map_size_y])
{
    int pressedKey = 0;

    while (snake_ptr->isAlive) {
        if (kbhit()) {
            pressedKey = processKeyboardInput(pressedKey);
        }
        updateSnakeMoveDirection(pressedKey, snake_ptr, board_ptr);
        printBoard(board_ptr);
        printFoodPoint(point_ptr, board_ptr);

        if (scoredPoint(snake_ptr, point_ptr)) {
            pointScoredSound();
            append(snake_ptr);
            addPoints(snake_ptr);
            randomFoodPoint(snake_ptr, point_ptr, board_ptr);
            printFoodPoint(point_ptr, board_ptr);
        }

        wait(difficulty);

        if (kbhit()) {
            pressedKey = processKeyboardInput(pressedKey);
        }

        printf(" Score: %ld\t\n", snake_ptr->score);
    }

    SetConsoleCursorPosition(wHnd, gameOverPosition);
    printf("You lost!\n");

    freeGameSessionMemory(snake_ptr, point_ptr);
}

void printBoard(char (*ptr)[map_size_y])
{
    int i, j;
    SetConsoleCursorPosition(wHnd, startPosition);
    for (i = 0; i < map_size_x; i++) {
        for (j = 0; j < map_size_y; j++)
            printf("%c", ptr[i][j]);
        puts("");
    }
}

void movement(char (*board)[map_size_y], struct snake* s, int deltaX,
    int deltaY)
{
    if (collisionWithBordersOccurred(s)) {
        s->isAlive = false;
        gameOverSound();
    } else {
        struct node* currentElement = s->head;
        currentElement->newX += deltaX;
        currentElement->newY += deltaY;
        do {
            board[currentElement->newX][currentElement->newY] = SNAKE_TEXTURE;

            if (s->head == s->tail) {
                board[currentElement->x][currentElement->y] = 0;
                s->last_x = currentElement->x;
                s->last_y = currentElement->y;
                currentElement->x = currentElement->newX;
                currentElement->y = currentElement->newY;
            }

            if (currentElement->next) {
                currentElement = currentElement->next;
                if (currentElement->next == NULL) {
                    s->last_x = currentElement->newX;
                    s->last_y = currentElement->newY;
                    s->tail->newX = currentElement->prev->x;
                    s->tail->newY = currentElement->prev->y;
                    s->tail->x = s->last_x;
                    s->tail->y = s->last_y;
                    currentElement->prev->x = currentElement->prev->newX;
                    currentElement->prev->y = currentElement->prev->newY;
                    board[s->tail->newX][s->tail->newY] = 0;
                } else {
                    currentElement->newX = currentElement->prev->x;
                    currentElement->newY = currentElement->prev->y;
                    currentElement->prev->x = currentElement->prev->newX;
                    currentElement->prev->y = currentElement->prev->newY;
                }
            }
            checkForSelfCollision(s, currentElement);

            s->fieldsOccupiedBySnake[currentElement->newX][currentElement->newY] = true;
            s->fieldsOccupiedBySnake[s->last_x][s->last_y] = false;
        } while (currentElement->next != NULL);
    }
}

char processKeyboardInput(char pressedKey)
{
    char last = pressedKey;
    pressedKey = getch();

    if (pressedKey == PAUSE_KEY) {
        do {
            pressedKey = last;
            if (kbhit())
                pressedKey = getch();

        } while (pressedKey != PAUSE_KEY);
    }

    if (pressedWrongKey(pressedKey) || wrongTurnAngle(pressedKey, last)) //(wrong button || 180° turn)
        pressedKey = last;

    return pressedKey;
}

bool wrongTurnAngle(char pressedKey, char last)
{
    if (abs(pressedKey - last) == 8 || abs(pressedKey - last) == 2)
        return true;
    else
        return false;
}

bool pressedWrongKey(char pressedKey)
{
    if (pressedKey != KEY_DOWN && pressedKey != KEY_UP && pressedKey != KEY_LEFT && pressedKey != KEY_RIGHT)
        return true;
    else
        return false;
}

void updateSnakeMoveDirection(int pressedKey, struct snake* snake_ptr,
    char (*board_ptr)[map_size_y])
{
    switch (pressedKey) {
    case KEY_DOWN:
        movement(board_ptr, snake_ptr, 1, 0);
        break;
    case KEY_UP:
        movement(board_ptr, snake_ptr, -1, 0);
        break;
    case KEY_LEFT:
        movement(board_ptr, snake_ptr, 0, -1);
        break;
    case KEY_RIGHT:
        movement(board_ptr, snake_ptr, 0, 1);
        break;
    default:;
    }
}

void wait(short int difficulty)
{
    clock_t endTime = clock() + difficulty * CLOCKS_PER_SEC / 100;
    while (clock() < endTime)
        continue;
}

void addPoints(struct snake* s)
{
    s->length++;

    if (difficulty == easy)
        s->score += (s->length * 0.4) * (difficulty * 0.1);
    else if (difficulty == medium)
        s->score += (s->length * 0.4) * (difficulty * 0.2);
    else
        s->score += (s->length * 0.4) * (difficulty * 0.4);
}

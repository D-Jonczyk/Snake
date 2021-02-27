#include <time.h>

#include "dialogs.h"
#include "food_point.h"

//    SNAKE NA PODSTAWIE LISTY DWUKIERUNKOWEJ

/*TODO:
[krytyczne]:
---------
[opcjonalne]:
	-poziomy
	-jakies podpowiedzi co do klawiszy
*/

static int difficulty;

bool gameOverConditionsFulfilled(struct snake *s);
void checkForSelfCollision(struct snake *, struct node *);
void processUserChoice();
char processKeyboardInput(char);
bool pressedWrongKey(char);
bool wrongTurnAngle(char, char);
bool selfCollisionOccurred();

void initConsoleParameters();
void fillTheBoardWithAscii(char (*)[map_size_y]);
void printBoard(char (*)[map_size_y], COORD);
void wait(short int);
void updateSnakeMoveDirection(int, struct snake *, char (*)[map_size_y]);

void initSnake(struct snake *s);
void printSnakeAtStartPos(char (*)[map_size_y], struct snake *s);

void movement(char (*)[map_size_y], struct snake *s, int move_x, int move_y);
void append(struct snake *s, struct point *p);
void addPoints(struct snake *s);
void freeSnakeNodes(struct snake *s);

void gameOverSound();

int main() {
	srand(time(NULL));
	difficulty = easy;
	int pressedKey;
	char choice = 0;
	char board[map_size_x][map_size_y];
	char (*board_ptr)[map_size_y] = board;

    initConsoleParameters();

	while(choice != '3') //program loop
	{
		printMainMenu();
		processUserChoice();

		struct snake *snake_ptr = (struct snake*)malloc(sizeof(struct snake) * (sizeof(struct node) * 100));
		struct point *point_ptr = (struct point*)malloc(sizeof(struct point));

		fillTheBoardWithAscii(board_ptr);
		initSnake(snake_ptr);
		printSnakeAtStartPos(board_ptr,snake_ptr);
		randomFoodPoint(snake_ptr, point_ptr, board_ptr);
		printFoodPoint(point_ptr, board_ptr);
		printBoard(board_ptr, startPosition);
		pressedKey = 0;

		while(snake_ptr->isAlive) //game session loop
		{
			if( kbhit() )
			{
                pressedKey = processKeyboardInput(pressedKey);
			}
			updateSnakeMoveDirection(pressedKey, snake_ptr, board_ptr);

			printBoard(board_ptr, startPosition);
			printFoodPoint(point_ptr, board_ptr); //fix rzadkiego buga gdy punkt znikal z mapy

			if( scoredPoint(snake_ptr, point_ptr) )
			{
				pointScoredSound();
				append(snake_ptr, point_ptr);
				addPoints(snake_ptr);
				randomFoodPoint(snake_ptr, point_ptr, board_ptr);
				printFoodPoint(point_ptr, board_ptr);
			}

			wait(difficulty);

			if( kbhit() )
			{
                pressedKey = processKeyboardInput(pressedKey);
			}

		printf(" Score: %ld\t\n", snake_ptr->score);
		} //end of game session

		SetConsoleCursorPosition(wHnd, gameOverPosition);
		printf("You lost!\n");
		freeSnakeNodes(snake_ptr);
		free(snake_ptr);
		free(point_ptr);
	}//end of the program
}

void freeSnakeNodes(struct snake *s)
{
    struct node *element;
    while((element = s->head) != NULL)
    {
        s->head = s->head->next;
        free(element);
    }
}

void append(struct snake *s, struct point *p) //dodaje element na koniec weza
{
	struct node *newTail = (struct node*)malloc(sizeof(struct node));
	newTail->next = NULL;
    newTail->newX = p->x;
    newTail->newY = p->y;

    if(s->head == s->tail){
        newTail->prev = s->head;
        s->head->next = newTail;
        s->tail = newTail;
    }
    else
    {
        newTail->newX = s->tail->x;
        newTail->newY = s->tail->y;

        s->tail->next = newTail;
        newTail->prev = s->tail;

        s->tail = newTail;
    }
}

void movement(char (*board)[map_size_y], struct snake *s, int deltaX, int deltaY) //efekt kilku dni debugowania i pisania wszystkiego od nowa
{
	if(gameOverConditionsFulfilled(s))
	{
			s->isAlive=false;
			gameOverSound();
	}
	else
	{
		struct node *snakeHead = s->head;
		snakeHead->newX += deltaX;
		snakeHead->newY += deltaY;
		do
		{
			board[snakeHead->newX][snakeHead->newY] = SNAKE_TEXTURE;

			//gdy waz ma jeden element
			if(s->head == s->tail)
			{
				board[snakeHead->x][snakeHead->y] = 0;
				s->last_x = snakeHead->x;
				s->last_y = snakeHead->y;
				snakeHead->x = snakeHead->newX;
				snakeHead->y = snakeHead->newY;
			}

			if(snakeHead->next)
			{
				snakeHead = snakeHead->next;
				if(snakeHead->next==NULL)
				{
					s->last_x = snakeHead->newX;
					s->last_y = snakeHead->newY;
					s->tail->newX = snakeHead->prev->x;
					s->tail->newY = snakeHead->prev->y;
					s->tail->x = s->last_x;
					s->tail->y = s->last_y;
					snakeHead->prev->x = snakeHead->prev->newX;
					snakeHead->prev->y = snakeHead->prev->newY;
					board[s->tail->newX][s->tail->newY] = 0;
				}
				else
                {
					snakeHead->newX = snakeHead->prev->x;
					snakeHead->newY = snakeHead->prev->y;
					snakeHead->prev->x = snakeHead->prev->newX;
					snakeHead->prev->y = snakeHead->prev->newY;
				}
			}

            //checkForSelfCollision(s, snakeHead);

			s->fieldsOccupiedBySnake[snakeHead->newX][snakeHead->newY] = true;
			s->fieldsOccupiedBySnake[s->last_x][s->last_y] = false;
		}while(snakeHead->next != NULL);
	}
}

void initSnake(struct snake *s) //chyba mozna to bylo krocej napisac ale boje sie bo dziala
{
	int i,j;
	struct node *head = (struct node*)malloc(sizeof(struct node));
	head->x = 5;
	head->y = 5;
	head->newX = 5;
	head->newY = 5;
	head->next = NULL;
	head->prev = NULL;
	s->head = head;
	s->tail = head;
	s->last_x = 5;
	s->last_y = 5;
	s->length = 1;
	s->score = 1;
	s->isAlive = true;

	for(i=0;i<map_size_x;i++)
		for(j=0;j<map_size_y;j++)
			s->fieldsOccupiedBySnake[i][j] = false;
	s->fieldsOccupiedBySnake[5][5] = true;
}

void addPoints(struct snake *s)
{
    s->length++;

	if(difficulty==easy)
		s->score += (s->length*0.4) * (difficulty*0.1);
	else if(difficulty==medium)
		s->score += (s->length*0.4) * (difficulty*0.2);
	else
		s->score += (s->length*0.4) * (difficulty*0.4);
}

void updateSnakeMoveDirection(int pressedKey, struct snake *snake_ptr, char (*board_ptr)[map_size_y])
{
    switch(pressedKey)
    {
        case KEY_DOWN: movement(board_ptr, snake_ptr, 1, 0);	break;
        case KEY_UP: movement(board_ptr, snake_ptr, -1,0);	break;
        case KEY_LEFT: movement(board_ptr, snake_ptr, 0,-1);	break;
        case KEY_RIGHT: movement(board_ptr, snake_ptr, 0,1);	break;
        default: ;
    }
}

void processUserChoice(){
    char choice = getUserMenuChoice();
    switch(choice)
    {
        case '1':
            break;
        case '2':
            system("cls");
            printDifficultyMenu();
            char choice_d = getUserMenuChoice();
            switch(choice_d)
            {
                case '1':
                    difficulty = easy;
                    break;
                case '2':
                    difficulty = medium;
                    break;
                case '3':
                    difficulty = hard;
                    break;
            }
            break;
        case '3':
            exit(0);
    }
}

char processKeyboardInput(char pressedKey)
{
    char last = pressedKey;
    pressedKey = getch();

    if(pressedKey == PAUSE_KEY){
        do{
            pressedKey = last;
            if(kbhit())
                pressedKey = getch();

        }while(pressedKey != PAUSE_KEY);
    }

    if( pressedWrongKey(pressedKey) || wrongTurnAngle(pressedKey, last) ) //(wrong button || 180° turn)
        pressedKey = last;

    return pressedKey;
}

bool wrongTurnAngle(char pressedKey, char last)
{
    if( abs(pressedKey-last) == 8 || abs(pressedKey-last) == 2 )
        return true;
    else
        return false;
}

bool pressedWrongKey(char pressedKey)
{
    if(pressedKey != KEY_DOWN && pressedKey != KEY_UP && pressedKey != KEY_LEFT && pressedKey != KEY_RIGHT )
        return true;
    else
        return false;
}

void initConsoleParameters()
{
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTitle(TEXT("Snake v1.0"));
	SMALL_RECT windowSize = {0, 0, 25, 17};
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

 	// zmienna COORD trzymajaca rozmiar bufora:
	COORD bufferSize = {26, 18};
  	// zmiana rozmiarow bufora:
	SetConsoleScreenBufferSize(wHnd, bufferSize);

    startPosition.X=0;
	startPosition.Y=0;

	gameOverPosition.X = 7;
    gameOverPosition.Y = 3;

    printMainMenu(); //without it there are black spots in unused area of the console
	system("cls");
}

void fillTheBoardWithAscii(char (*ptr)[map_size_y])
{
	unsigned int i,j;
	for(i=0;i<map_size_x;i++)
	{
		for(j=0;j<map_size_y;j++)
		{
			if(i==0 || i==(map_size_x-1))
				ptr[i][j] = HORIZONTAL_BORDER;
			else if(j==0 || j==(map_size_y-1))
				ptr[i][j] = VERTICAL_BORDER;
			else
				ptr[i][j] = BOARD_FILL;

			if( i==0 && j==0 )
				ptr[i][j] = TOP_LEFT_CORNER;
			if( i==0 && j==(map_size_y-1) )
				ptr[i][j] = TOP_RIGHT_CORNER;
			if( i==(map_size_x-1) && j==0 )
				ptr[i][j] = BOT_LEFT_CORNER;
			if( i==(map_size_x-1) && j==(map_size_y-1) )
				ptr[i][j] = BOT_RIGHT_CORNER;
		}
	}
}

void printBoard(char (*ptr)[map_size_y], COORD begin)
{
	int i,j;
	SetConsoleCursorPosition(wHnd,begin);
	for(i=0;i<map_size_x;i++){
		for(j=0;j<map_size_y;j++)
			printf("%c", ptr[i][j]);
		puts("");
	}
}

void wait(short int difficulty)
{
    clock_t endTime = clock() + difficulty * CLOCKS_PER_SEC/100;
    while( clock() < endTime ) continue;
}

void printSnakeAtStartPos(char (*ptr)[map_size_y], struct snake *s) //ustawia glowke na poczatkowe koordynaty
{
	ptr[s->head->newX][s->head->newY] = SNAKE_TEXTURE;
}

bool gameOverConditionsFulfilled(struct snake *s)
{
    if( s->head->newX >= map_size_x-1 || s->head->newX <= 0 || s->head->newY >= (map_size_y-1) || s->head->newY <= 0)
        return true;
    else
        return false;
}

bool selfCollision(struct snake *s, struct node *element){
    if( (s->head->newX == element->newX) && (s->head->newY == element->newY) )
        return true;
    else
        return false;
}

void checkForSelfCollision(struct snake *s, struct node *element)
{
    if(s->tail)
    {
        if(selfCollision(s, element))
        {
            gameOverSound();
            s->isAlive=false;
        }
    }
}

void gameOverSound()
{
    Beep(100,400);
}


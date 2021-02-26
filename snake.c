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

void processUserChoice();
char processKeyboardInput(char ch);
bool pressedWrongKey(char ch);
bool wrongTurnAngle(char, char);

void initConsoleParameters();
void fillTheBoardWithAscii(char (*ptr)[map_size_y]);
void board_print(char (*ptr)[map_size_y], COORD begin);
void wait(short int);

void snake_init(struct snake *s);
void printSnakeAtStartPos(char (*ptr)[map_size_y], struct snake *s);

void movement(char (*ptr)[map_size_y], struct snake *s, int move_x, int move_y);
void append(struct snake *s, struct point *p, short int difficulty);

void gameOverSound();

int main() {
	srand(time(NULL));
	difficulty = easy;
	int pressedKey;
	char choice = 0;
	char tab[map_size_x][map_size_y];
	char (*t_ptr)[map_size_y]= tab;

    initConsoleParameters();

	while(choice != '3') //program loop
	{
		printMainMenu();
		processUserChoice();

		struct snake *snake_ptr = (struct snake*)malloc(sizeof(struct snake));
		struct point *point_ptr = (struct point*)malloc(sizeof(struct point));
		fillTheBoardWithAscii(t_ptr);
		snake_init(snake_ptr);
		printSnakeAtStartPos(t_ptr,snake_ptr);
		randomFoodPoint(snake_ptr,point_ptr, t_ptr);
		printFoodPoint(point_ptr, t_ptr);
		board_print(t_ptr, startPosition);
		pressedKey = 0;

		while(!snake_ptr->status) //game session loop
		{
			if( kbhit() )
			{
                pressedKey = processKeyboardInput(pressedKey);
                switch(pressedKey)
                {
                    case KEY_DOWN: movement(t_ptr, snake_ptr, 1, 0);	break;
                    case KEY_UP: movement(t_ptr, snake_ptr, -1,0);	break;
                    case KEY_LEFT: movement(t_ptr, snake_ptr, 0,-1);	break;
                    case KEY_RIGHT: movement(t_ptr, snake_ptr, 0,1);	break;
                    default: ;
                }
			}

			board_print(t_ptr, startPosition);
			printFoodPoint(point_ptr, t_ptr); //fix rzadkiego buga gdy punkt znikal z mapy

			if( scoredPoint(snake_ptr, point_ptr) )
			{
				pointScoredSound();
				append(snake_ptr,point_ptr,difficulty);
				randomFoodPoint(snake_ptr,point_ptr, t_ptr);
				printFoodPoint(point_ptr, t_ptr);
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
		free(snake_ptr);
		free(point_ptr);
	}//end of the program
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

void snake_init(struct snake *s) //chyba mozna to bylo krocej napisac ale boje sie bo dziala
{
	int i,j;
	struct node *_head = (struct node*)malloc(sizeof(struct node));
	//struct node *_tail = (struct node*)malloc(sizeof(struct node));
	_head->x = 5;
	_head->y = 5;
	_head->_x = 5;
	_head->_y = 5;
	_head->next = NULL;
	_head->prev = NULL;
	s->head = _head;
	s->tail = NULL;
	s->last_x = 5;
	s->last_y = 5;
	s->count = 1;
	s->score = 1;
	s->status = false;

	for(i=0;i<map_size_x;i++)
		for(j=0;j<map_size_y;j++)
			s->field[i][j] = false;
	s->field[5][5] = true;
}

void board_print(char (*ptr)[map_size_y], COORD begin)
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
	ptr[s->head->_x][s->head->_y] = SNAKE_TEXTURE;
}

bool gameOverConditionsFulfilled(struct snake *s)
{
    if( s->head->_x >= map_size_x-1 ||s->head->_x <= 0 || s->head->_y >= (map_size_y-1) || s->head->_y <= 0)
        return true;
    else
        return false;
}

bool selfCollision(struct snake *s, struct node *element){
    if( (s->head->_x == element->_x) && (s->head->_y == element->_y) )
        return true;
    else
        return false;
}

void gameOverSound()
{
    Beep(100,400);
}

void movement(char (*ptr)[map_size_y], struct snake *s, int move_x, int move_y) //efekt kilku dni debugowania i pisania wszystkiego od nowa
{
	if(gameOverConditionsFulfilled(s))
	{
			s->status=true;
			gameOverSound();
	}
	else
	{
		struct node *element = (struct node*)malloc(sizeof(struct node));
		element = s->head;
		element->_x += move_x;
		element->_y += move_y;
		do
		{
			ptr[element->_x][element->_y] = SNAKE_TEXTURE;
			//gdy waz ma jeden element
			if(s->tail==NULL)
			{
				ptr[element->x][element->y] = 0;
				s->last_x = element->x;
				s->last_y = element->y;
				element->x = element->_x;
				element->y = element->_y;
			}
			//
			if(element->prev)
			{
				element = element->prev;
				if(element->prev==NULL)
				{
					s->last_x = element->_x;
					s->last_y = element->_y;
					s->tail->_x = element->next->x;
					s->tail->_y = element->next->y;
					s->tail->x = s->last_x;
					s->tail->y = s->last_y;
					element->next->x = element->next->_x;
					element->next->y = element->next->_y;
					ptr[s->last_x][s->last_y] = 0;
				}
				else{

					element->_x = element->next->x;
					element->_y = element->next->y;
					element->next->x = element->next->_x;
					element->next->y = element->next->_y;
				}
			}

			if(s->tail)
            {
				if(selfCollision(s, element))
				{
				    gameOverSound();
					s->status=true;
				}
            }
			s->field[element->_x][element->_y] = true;
			s->field[s->last_x][s->last_y] = false;
		}while(element->prev);
	}
}

void append(struct snake *s, struct point *p, short int difficulty) //dodaje element na koniec weza
{
	struct node *element = (struct node*)malloc(sizeof(struct node));
	element->prev = NULL;
	//to nie powinno tak chyba dzia³aæ - gdy jest jeden element to jest on  zarowno glowa jak i ogonem, sprawdzic to i poprawic
	if(s->tail==NULL){ //gdy tail nie istnieje(sama glowa)
		element->_x = s->head->_x;
		element->_y = s->head->_y;
		element->next = s->head;
		s->head->prev = element;
		s->tail = element;
	}
	else
	{
		element->_x = s->tail->_x;
		element->_y = s->tail->_y;
		element->next = s->tail;
		s->tail->prev = element;
		s->tail = element;
	}

	s->count++;

	if(difficulty==easy)
		s->score += (s->count*0.4) * (difficulty*0.1);
	else if(difficulty==medium)
		s->score += (s->count*0.4) * (difficulty*0.2);
	else
		s->score += (s->count*0.4) * (difficulty*0.4);
}



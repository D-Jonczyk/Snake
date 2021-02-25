#include "dialogs.h"

char menu_option(void)
{
	char choice;
	do{
		choice = getch();
		switch( choice )
		{
			case '1':
			case '2':
			case '3': return(choice);
			default:
				choice='0';
		}
	}while( choice == '0');
	return choice;
}

void menu(void)
{
	SetConsoleTextAttribute(wHnd, BACKGROUND_GREEN | BACKGROUND_RED  | FOREGROUND_RED);
	printf("\n\n\n");
	printf("-------------------------\n");
	printf("[1] Nowa gra\t\t\n");
	printf("[2] Poziom trudnosci\t\n");
	printf("[3] Wyjscie\t\t\n");
	printf("-------------------------\n");
}

void menu_difficulty(void)
{
	printf("\n");
	printf("-------------------------\n");
	printf("[1] Latwy\t\t\n");
	printf("[2] Sredni\t\n");
	printf("[3] Trudny\t\t\n");
	printf("-------------------------\n");
}

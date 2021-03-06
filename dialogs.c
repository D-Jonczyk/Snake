#include "dialogs.h"

extern int difficulty;

char getUserMenuChoice(void)
{
    char choice;
    do {
        choice = getch();
        switch (choice) {
        case '1':
        case '2':
        case '3':
            return (choice);
        default:
            choice = '0';
        }
    } while (choice == '0');

    return choice;
}

void printMainMenu(void)
{
    SetConsoleTextAttribute(wHnd, BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_RED);
    printf("\n\n");
    printf("-------------------------\n");
    printf("[1] New Game\t\t\n");
    printf("[2] Difficulty\t\n");
    printf("[3] Exit\t\t\n");
    printf("-------------------------\n");
}

void printDifficultyMenu(void)
{
    printf("\n");
    printf("-------------------------\n");
    printf("[1] Easy\t\t\n");
    printf("[2] Medium\t\n");
    printf("[3] Hard\t\t\n");
    printf("-------------------------\n");
}

void processUserChoice(void)
{
    char choice = getUserMenuChoice();
    switch (choice) {
    case '1':
        break;
    case '2':
        system("cls");
        printDifficultyMenu();
        char choice_d = getUserMenuChoice();
        switch (choice_d) {
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

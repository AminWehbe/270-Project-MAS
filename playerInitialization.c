#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define SIZE 10
#define PLAYER_COUNT 2
#define SHIP_COUNT sizeof(shipSizes) / sizeof(shipSizes[0])
#define MAX_RADAR_SWEEPS 3

enum ShipType
{
    CARRIER,
    BATTLESHIP,
    DESTROYER,
    SUBMARINE
};
int shipSizes[] = {5, 4, 3, 2};
bool isHard;
typedef struct
{
    int radarSweeps;
    int smokeScreens;
    int shipsSunk;
    char board[SIZE][SIZE];
} PlayerStatus;

void initializeBoard(char board[SIZE][SIZE]);
void printBoard(char board[SIZE][SIZE], bool isHardMode);
int randomFirstPlayer();
void getDifficulity(char difficulity[2]);
void getPlayerNames(char names[PLAYER_COUNT][20]);
void clearScreen();
void placeShips(char board[SIZE][SIZE]);
bool attack(char player2Board[SIZE][SIZE], int x, int y);
bool radarSweep(char player2Board[SIZE][SIZE], int x, int y);
void smokeScreen(char player2Board[SIZE][SIZE], int x, int y);
bool artillery(char player2Board[SIZE][SIZE], int x, int y);
bool torpedo(char player2Board[SIZE][SIZE], int index);
bool checkWin(char player2Board[SIZE][SIZE]);

int main()
{
    PlayerStatus playerStatus[PLAYER_COUNT];
    char playerNames[PLAYER_COUNT][20];
    int firstPlayer;
    char hardMode[2];
    getPlayerNames(playerNames);
    getDifficulity(hardMode);
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        playerStatus[i].radarSweeps = MAX_RADAR_SWEEPS;
        playerStatus[i].smokeScreens = 0;
        playerStatus[i].shipsSunk = 0;
    }
    puts(playerNames[1]);
}
void getPlayerNames(char names[PLAYER_COUNT][20])
{
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        printf("Enter name for player %d: ", i + 1);
        scanf("%s", names[i]);
    }
}
void getDifficulity(char difficulity[2])
{
    int c;
    printf("Please enter a tracking difficulty level: E --D Easy, H --D Hard.\n");
    scanf("%s", difficulity);
    while (strlen(difficulity) > 1 || difficulity[0] != 'E' && difficulity[0] != 'H' && difficulity[0] != 'e' && difficulity[0] != 'h')
    {
        printf("The input you entered is invalid. Please enter: E --D Easy, H --D Hard.\n");
        scanf("%s", difficulity);
    }
    switch (difficulity[0])
    {
    case 'E' | 'e':
        puts("You chose Easy Mode. Let's Play!");
        isHard = false;
        break;
    case 'H' | 'h':
        puts("You chose Hard Mode. Let's Play!");
        isHard = true;
        break;
    default:
        break;
    }
}
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

typedef struct
{
    int radarSweeps;
    int smokeScreens;
    int shipsSunk;
    char board[SIZE][SIZE];
    // bool isHardMode;
} PlayerStatus;

void initializeBoard(char board[SIZE][SIZE]);
void printBoard(char board[SIZE][SIZE], bool isHardMode);
int randomFirstPlayer();
void getPlayerNames(char names[PLAYER_COUNT][20]);
void clearScreen();
void placeShips(char board[SIZE][SIZE]);
bool attack(char player2Board[SIZE][SIZE], int x, int y);
bool radarSweep(char player2Board[SIZE][SIZE], int x, int y);
void smokeScreen(char player2Board[SIZE][SIZE], int x, int y);
bool artillery(char player2Board[SIZE][SIZE], int x, int y);
bool torpedo(char player2Board[SIZE][SIZE], int index);
bool checkWin(char player2Board[SIZE][SIZE]);

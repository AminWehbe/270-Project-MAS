#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define SIZE 10
#define PLAYER_COUNT 2
#define SHIP_COUNT 4
#define MAX_RADAR_SWEEPS 3

typedef struct
{
    char board[SIZE][SIZE];
    char displayBoard[SIZE][SIZE];
    bool shipsAvailable[SHIP_COUNT];
    int radarSweeps;
    int smokeScreens;
    int shipsSunk;
    bool canUseSmokeScreen;
    bool canUseArtillery;
    bool canUseTorpedo;
} PlayerStatus;

int shipSizes[4] = {5, 4, 3, 2};
bool isHard;
void clearScreen();
void getPlayerNames(char names[PLAYER_COUNT][20]);
void getDifficulity(char difficulity[]);
void initializeBoard(char board[SIZE][SIZE]);
int randomFirstPlayer();
void waitFor(unsigned int secs);
void printBoard(char board[SIZE][SIZE], bool isHardMode);
void placeShip(char board[SIZE][SIZE], int size);
void displayAvailableShips(PlayerStatus playerStatus[], int playerIndex);
void displayAvailableMoves(PlayerStatus playerStatus[], int playerIndex);

bool attack(char player1Board[SIZE][SIZE], char displayBoard[SIZE][SIZE], int x, int y);
bool radarSweep(char player2Board[SIZE][SIZE], int x, int y);
void smokeScreen(char player2Board[SIZE][SIZE], int x, int y);
bool artillery(char player2Board[SIZE][SIZE], int x, int y);
bool torpedo(char player2Board[SIZE][SIZE], int index);
bool checkWin(char player2Board[SIZE][SIZE]);
void switchTurn(int p1, int p2);
bool validateCoordinates(int x, int y);
bool checkPlacement(char board[SIZE][SIZE], int x, int y, int size, bool horizontal);

int main()
{
    // Player initialization phase
    clearScreen();
    srand(time(NULL));
    PlayerStatus playerStatus[PLAYER_COUNT];
    char playerNames[PLAYER_COUNT][20];
    int randomNumber;
    char hardMode[2];
    getPlayerNames(playerNames);
    getDifficulity(hardMode);
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        playerStatus[i].radarSweeps = MAX_RADAR_SWEEPS;
        playerStatus[i].smokeScreens = 0;
        playerStatus[i].shipsSunk = 0;
        playerStatus[i].canUseSmokeScreen = false;
        playerStatus[i].canUseArtillery = false;
        playerStatus[i].canUseTorpedo = false;
        for (int j = 0; j < SHIP_COUNT; j++)
        {
            playerStatus[i].shipsAvailable[j] = true;
        }
        initializeBoard(playerStatus[i].board);
        initializeBoard(playerStatus[i].displayBoard);
    }

    puts("Let's see who goes first.");
    randomNumber = randomFirstPlayer();
    waitFor(2);
    printf("%s goes first!\n", playerNames[randomNumber]);

    bool gameOver = false;
    int player1 = randomNumber;
    int player2 = player1 + 1;
    waitFor(1);
    puts("LAUNCHING GAME...");
    waitFor(3);
    // Ship placing phase
    for (int i = 0; i < PLAYER_COUNT; i++) {
        clearScreen();
        printf("%s, PLACE YOUR SHIPS:\n\n", playerNames[i]);
        printBoard(playerStatus[i].board, isHard);
        while (playerStatus[i].shipsAvailable[0] || playerStatus[i].shipsAvailable[1] || playerStatus[i].shipsAvailable[2] || playerStatus[i].shipsAvailable[3]) {
            displayAvailableShips(playerStatus, i);
        }
        waitFor(1);
        puts("NICE PLACEMENTS.\n");
        if(i == 0)
        {
            waitFor(1);
            puts("SWITCHING TURNS.");
        }
        waitFor(3);
    }
    clearScreen();
    puts("LET'S PLAY OCEAN WARFARE!\nMAY THE BEST SHIP CAPTAIN WIN!\n");
    waitFor(1);
    puts("GAME STARTING...");
    waitFor(3);

    // Game loop
    while (!gameOver)
    {
        clearScreen();
        printf("%s's turn!\n\n", playerNames[player1]);
        printf("This is your board %s.\n\n", playerNames[player1]);
        printBoard(playerStatus[player1].board, isHard);
        
        printf("This is the opponent's board (your view):\n");
        printBoard(playerStatus[player1].displayBoard, false); // Show the opponent's display board

        
        displayAvailableMoves(playerStatus, player1);
        /*char coord[3];
        int x, y;
        printf("Enter your attack coordinates (e.g., A1): ");
        scanf(" %s", coord);
        
        y = toupper(coord[0]) - 'A';
        x = atoi(&coord[1]) - 1;

        if (attack(playerStatus[player2].board, playerStatus[player1].displayBoard, x, y)) 
            printf("Hit!\n");
         else
            printf("Miss.\n");*/

        // Switch players
        player1 = player2;
        player2 = (player1 + 1) % PLAYER_COUNT;
        printf("SWITCHING TURNS\n");
        waitFor(3);
    }
    return 0;
}
// Functions implementation
void getPlayerNames(char names[PLAYER_COUNT][20])
{
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        printf("Enter name for player %d: ", i + 1);
        scanf("%s", names[i]);
    }
}
void getDifficulity(char difficulity[])
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
// Initializes board with water (~)
void initializeBoard(char board[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
            board[i][j] = '~';
    }
}
// Prints the board
void printBoard(char board[SIZE][SIZE], bool isHardMode)
{
    printf("   A B C D E F G H I J\n"); // Column headers
    for (int i = 0; i < SIZE; i++)
    {
        printf("%2d", i + 1); // Row numbers without space for alignment
        for (int j = 0; j < SIZE; j++)
        {
            char cell = board[i][j];
            if (cell == 'S')
                printf(" S"); // Show ship placement
            else if (cell == 'X')
                printf(" *"); // Shows Hit
            else if (cell == 'o' && !isHardMode)
                printf(" o"); // Shows Miss
            else
                printf(" ~"); // Water or hidden in Hard mode
        }
        printf("\n");
    }
    printf("\n");
}
// Method that displays to the player his only available moves.
void displayAvailableMoves(PlayerStatus playerStatus[], int playerIndex)
{
    int i = 1;
    printf("Available moves:\n");
    printf("%d. Fire\n", i++);

    if (playerStatus[playerIndex].radarSweeps > 0)
        printf("%d. Radar Sweep (Remaining: %d)\n", i++, playerStatus[playerIndex].radarSweeps);

    if (playerStatus[playerIndex].smokeScreens > 0)
        printf("%d. Smoke Screen (Available: %d)\n", i++, playerStatus[playerIndex].smokeScreens);

    if (playerStatus[playerIndex].canUseArtillery)
        printf("%d. Artillery\n", i++);

    if (playerStatus[playerIndex].canUseTorpedo)
        printf("%d. Torpedo\n", i++);
        for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (playerStatus[playerIndex].shipsAvailable[i])
            printf("%d. Ship of size %d\n", i + 1, shipSizes[i]);
    }

    int choice;
    while (true)
    { 
        printf("Select a move to make (enter the number): ");

        // Read input and check if it's valid integer
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // to clear input buffer (gpt)
            continue;
        }
        if (choice < 1 || choice > i)
        {
            printf("Invalid choice. Please select a valid move number.\n");
            continue;
        }
        //here we need to put in each case a call to the chosen method + updating both the actual and display boards.
        /*switch (choice)
        {
        case 1:
            if (playerStatus[playerIndex].radarSweeps > 0)
                printf("%d. Radar Sweep (Remaining: %d)\n", i++, playerStatus[playerIndex].radarSweeps);
            break;
        case 2:
            if (playerStatus[playerIndex].smokeScreens > 0)
                printf("%d. Smoke Screen (Available: %d)\n", i++, playerStatus[playerIndex].smokeScreens);
            break;
        case 3:
            if (playerStatus[playerIndex].canUseArtillery)
                printf("%d. Artillery\n", i++);
            break;
        case 4:
            if (playerStatus[playerIndex].canUseTorpedo)
                printf("%d. Torpedo\n", i++);
            break;
        default:
            printf("Invalid choice.");
            break;
        }*/
        break;
    }
}
// Method that checks for out-of-bounds in the coordinates given
bool validateCoordinates(int x, int y)
{
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
    {
        return false;
    }
    return true;
}
// Method that displays to user whish ships can be placed with their sizes.
void displayAvailableShips(PlayerStatus playerStatus[], int playerIndex)
{
    printf("Available ships to place:\n");
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (playerStatus[playerIndex].shipsAvailable[i])
            printf("%d. Ship of size %d\n", i + 1, shipSizes[i]);
    }

    int choice;
    while (true)
    { // Infinite loop until a valid input is received
        printf("Select a ship to place (enter the number): ");

        // Read input and check if it's valid integer
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // to clear input buffer (gpt)
            continue;
        }
        if (choice < 1 || choice > SHIP_COUNT)
        {
            printf("Invalid choice. Please select a valid ship number.\n");
            continue;
        }
        switch (choice)
        {
        case 1:
            if (playerStatus[playerIndex].shipsAvailable[0])
            {
                placeShip(playerStatus[playerIndex].board, shipSizes[0]);
                playerStatus[playerIndex].shipsAvailable[0] = false;
            }
            else
            {
                printf("Ship of size %d is not available.\n", shipSizes[0]);
            }
            break;
        case 2:
            if (playerStatus[playerIndex].shipsAvailable[1])
            {
                placeShip(playerStatus[playerIndex].board, shipSizes[1]);
                playerStatus[playerIndex].shipsAvailable[1] = false;
            }
            else
            {
                printf("Ship of size %d is not available.\n", shipSizes[1]);
            }
            break;
        case 3:
            if (playerStatus[playerIndex].shipsAvailable[2])
            {
                placeShip(playerStatus[playerIndex].board, shipSizes[2]);
                playerStatus[playerIndex].shipsAvailable[2] = false;
            }
            else
            {
                printf("Ship of size %d is not available.\n", shipSizes[2]);
            }
            break;
        case 4:
            if (playerStatus[playerIndex].shipsAvailable[3])
            {
                placeShip(playerStatus[playerIndex].board, shipSizes[3]);
                playerStatus[playerIndex].shipsAvailable[3] = false;
            }
            else
            {
                printf("Ship of size %d is not available.\n", shipSizes[3]);
            }
            break;
        }
        break;
    }
}
// Method to place the ships after checking the coordinates and the size of the ship
void placeShip(char board[SIZE][SIZE], int size)
{
    bool placed = false;
    while (!placed)
    {
        int x, y;
        char direction;
        printf("Enter the coordinates for your ship of size %d (e.g., A1 H): ", size);
        char coord[3];
        scanf(" %s %c", coord, &direction);

        if (strlen(coord) < 2 && strlen(coord) > 3 && /*coord[1] < '1' || coord[1] > '9' ||*/ coord[1] == '1' && coord[2] != '0')
        {
            printf("Invalid coordinates. Please use the format A1 or B10.\n");
            continue;
        }
        y = toupper(coord[0]) - 'A';
        x = atoi(&coord[1]) - 1;
        validateCoordinates(x, y);
        bool horizontal = (direction == 'H' || direction == 'h');
        if (horizontal)
        {
            if (y + size <= SIZE && checkPlacement(board, x, y, size, true))
            {
                for (int j = 0; j < size; j++)
                {
                    board[x][y + j] = 'S'; // Place ship
                }
                placed = true;
                puts("\nSHIP PLACED!");
                printBoard(board, false);
            }
            else
            {
                printf("Invalid placement. Try again.\n");
                continue;
            }
        }
        else
        {
            if (x + size <= SIZE && checkPlacement(board, x, y, size, false))
            {
                for (int j = 0; j < size; j++)
                {
                    board[x + j][y] = 'S'; // Place ship
                }
                puts("SHIP PLACED!");
                printBoard(board, false);
                placed = true;
            }
            else
            {
                printf("Invalid placement. Try again.\n");

                continue;
            }
        }
    }
}
// Method to check the placement in case of overlap. Returns true if overlap found, hence not allowing for ship placement
bool checkPlacement(char board[SIZE][SIZE], int x, int y, int size, bool horizontal)
{
    for (int j = 0; j < size; j++)
    {
        if (horizontal)
        {
            if (board[x][y + j] != '~')
                return false; // Overlap
        }
        else
        {
            if (board[x + j][y] != '~')
                return false; // Overlap
        }
    }
    return true; // Can place ship
}
// Method to randomly get and integer with the rand function and then get the remainder from the % operation(here %2 hence getting either 0 or 1 from any possible int)
int randomFirstPlayer()
{
    return rand() % PLAYER_COUNT;
}
// Method to add suspense (ex: when gettin first player randomly). No practical use as much as something i wanted to do and got from stackoverflow
void waitFor(unsigned int secs)
{
    unsigned int retTime = time(0) + secs; // Get finishing time.
    while (time(0) < retTime)
        ; // Loop until it arrives.
}
// To clear console screen depending on OS
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
bool attack(char opponentBoard[SIZE][SIZE], char displayBoard[SIZE][SIZE], int x, int y)
{
    if (validateCoordinates(x, y)) {
        if (opponentBoard[x][y] == 'S') {
            opponentBoard[x][y] = 'X'; // Mark hit on the actual board
            displayBoard[x][y] = 'X'; // Mark hit on the display board
            return true; // Hit
        } else {
            opponentBoard[x][y] = 'o'; // Mark miss on the actual board
            displayBoard[x][y] = 'o'; // Mark miss on the display board
            return false; // Miss
        }
    }
    return false; // Invalid attack
}
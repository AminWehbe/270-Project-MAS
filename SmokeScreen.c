#include <stdio.h>
#include <stdbool.h>

#define SIZE 10

void smokeScreen(char player2Board[SIZE][SIZE], int row, int col) {
    if (row < 0 || row >= SIZE - 1 || col < 0 || col >= SIZE - 1) {
        printf("Invalid coordinates for smoke screen. Out of bounds.\n");
    }
    for (int i = row; i < row + 2; i++) {
        for (int j = col; j < col + 2; j++) {
            player2Board[i][j] = '~'; 
        }
    }
    printf("Smoke screen applied at (%d, %c).\n", row + 1, 'A' + col);
}

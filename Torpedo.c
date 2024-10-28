#include <stdio.h>
#include <stdbool.h>

#define SIZE 10

bool torpedo(char player2Board[SIZE][SIZE], char index) {
    bool hit = false;

    if (isdigit(index)) {
        int row = index - '1';
        if (row >= 0 && row < SIZE) {
            for (int j = 0; j < SIZE; j++) {
                if (player2Board[row][j] == 'S') {
                    player2Board[row][j] = 'X'; 
                    hit = true;
                }
            }
        }
    } else if (isalpha(index)) {
        int col = toupper(index) - 'A';
        if (col >= 0 && col < SIZE) {
            for (int i = 0; i < SIZE; i++) {
                if (player2Board[i][col] == 'S') {
                    player2Board[i][col] = 'X'; 
                    hit = true;
                }
            }
        }
    }

    if (hit) {
        printf("Torpedo hit at least one target!\n");
    } else {
        printf("Torpedo missed all targets.\n");
    }

    return hit;
}

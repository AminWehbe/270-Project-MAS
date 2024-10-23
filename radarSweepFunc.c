#include <stdio.h>
#include <stdbool.h>
#define SIZE 10

bool radarSweep(char player2Board[SIZE][SIZE], int x, int y){
    if(x < 0 || x >= SIZE-1 || y < 0 || y >= SIZE-1){ // check bounds
        return false;
    }
    for(int i = x; i <= x+1; i++){
        for(int j = y; j <= y+1; j++){
            if(player2Board[i][j] == 'S'){ // check if a ship is found 
                return true;
            }
        }
    }
   return false;
}
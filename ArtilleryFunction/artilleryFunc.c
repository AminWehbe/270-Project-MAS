#include <stdio.h>
#include <stdbool.h>
#define SIZE 10


bool artillery(char player2Board[SIZE][SIZE], int x, int y){
        bool check = false;
        for(int i = x; i <= x+1; i++){
            for(int j = y; j <= y+1; j++){
                if(player2Board[i][j] == 'S'){
                    player2Board[i][j] = '*'; //Hit
                    check = true;
                }else if(player2Board[i][j] == '~'){
                    player2Board[i][j] = 'o'; // Miss
                }
            }
        }
        return check;
    
}
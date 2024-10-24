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
} //maybe change the method from bool to void, as i dont see a need for it to be bool.
// we can discuss your logic if you think otherwise when you are available
// please do include some description for the logic 

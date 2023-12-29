#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 3645    //this can be changed if the server initiation fails
#define BUFFSIZE 1024   //this may be changed based on how long of a string 
                        //you would like to pass between server and client
//TOBUFFEROPT will take an integet value of 1, 2, 3, or 4 and fill replace with the corresponding string
#define TOBUFFEROPT(option) ((option) == 1? "Rock" : ((option) == 2? "Paper" : ((option) == 3? "Scissors": "exit")))

/*
Description: fills the buffer with a player choice: 
             "Rock", "Paper", "Scissors", or "exit"
             Contains user input error detection

Pre-condition: Buffer is empty and at least size 9.

*/
void userInput(char buffer[]){ 

    int option; 
    
    do{ 
        printf("1. Rock\n"); 
        printf("2. Paper\n");
        printf("3. Scissors\n");  
        printf("4. close game\n"); 
        printf("enter a numerical value --> ");

        scanf("%d", &option); 

        if(!(option >= 1 && option <= 4)){ 
            printf("Invalid input, please enter the given options 1, 2, 3, or 4\n"); 
        } 

    }while(!(option >= 1 && option <= 4)); 

    strcpy(buffer, TOBUFFEROPT(option));
    return;
}
/*
Description: fills the buffer with a computer choice: 
             "Rock", "Paper", or "Scissors".

Pre-condition: Buffer is empty and at least size 9.

*/
void serverInput(char buffer[]){ 

    time_t t; 
    srand((unsigned) time(&t)); 
    int option = (rand() % 3) + 1; 

    strcpy(buffer, TOBUFFEROPT(option));
    return;
}
/*
Description: Determines the winner of the round and changes the game number 
             if either player or computer has won the round

pre-condition: choice_a and choice_b must be the same size array, buffer 
               must be size BUFFSIZE and game_number, score_a, and score_b
               must already be initiated to some integervalue (non-NULL).
*/
void findWinner(char choice_a[], char choice_b[], char buffer[], int* game_number, int* score_a, int* score_b){
        if(strcmp(choice_a, "exit") == 0){
            sprintf(buffer, "Server shutting down... \n");
            return;
        }
        
        if (strcmp(choice_a, choice_b) == 0){ 
            sprintf(buffer, "it was a draw \n"); 
        }

        else if(((strcmp(choice_a, "Paper") == 0) && (strcmp(choice_b, "Rock") == 0)) || ((strcmp(choice_a, "Scissors") == 0) && (strcmp(choice_b, "Paper") == 0)) || ((strcmp(choice_a, "Rock") == 0) && (strcmp(choice_b, "Scissors") == 0))){
            *game_number += 1; 
            *score_a += 1;
            sprintf(buffer, "player won this round \nScore Player %d:%d Computer\n", *score_a, *score_b);             
        } 

        else if (((strcmp(choice_a, "Rock") == 0) && (strcmp(choice_b, "Paper") == 0)) || ((strcmp(choice_a, "Paper") == 0) && (strcmp(choice_b, "Scissors") == 0))|| ((strcmp(choice_a, "Scissors") == 0) && (strcmp(choice_b, "Rock") == 0))){ 
            *score_b += 1; 
            *game_number += 1; 
            sprintf(buffer, "computer won this round \nScore Player %d:%d Computer\n", *score_a, *score_b); 
        }         
    
    return;
}
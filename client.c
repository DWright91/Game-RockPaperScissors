#include "server.h"
////////////////////////////////////////////////////////////////////////////////
// Client Driver
////////////////////////////////////////////////////////////////////////////////
int main(){


    int clientsoc;
    struct sockaddr_in serveradd;
    char buff[BUFFSIZE], winner = ' ';

    clientsoc = socket(AF_INET, SOCK_STREAM, 0); //create socket

    if(clientsoc < 0){
        printf("connection error\n");
        exit(1);
    }

    printf("client socket created\n");

    memset(&serveradd, '\0', sizeof(serveradd)); //connection data
    serveradd.sin_family = AF_INET;
    serveradd.sin_port = htons(PORT);
    serveradd.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(clientsoc, (struct sockaddr*)&serveradd, sizeof(serveradd)) < 0){ //connect to server
        printf("connection error\n");
        exit(1);
    }

    printf("connected to main server\n");
    memset(buff, 0, BUFFSIZE);
    recv(clientsoc, buff, BUFFSIZE, 0); //get number of rounds
    printf("%s", buff);
    while(winner != 'a' && winner != 'b'){ //play with the client
        userInput(buff); //get user move
        send(clientsoc, buff, BUFFSIZE, 0); //send user move
        memset(buff, 0, BUFFSIZE);
        recv(clientsoc, buff, BUFFSIZE, 0); //get computer move
        if(strcmp(buff, "exit") == 0){
            printf("Shutting down the server\n");
            break;
        }
        printf("%s", buff);
        memset(buff, 0, BUFFSIZE);
        send(clientsoc, buff, BUFFSIZE, 0);  //wait for server to send score and winner of the round
        memset(buff, 0, BUFFSIZE);
        recv(clientsoc, buff, BUFFSIZE, 0); //get score and winner of the round and winner of game is there is one
        winner = buff[BUFFSIZE - 1]; //detects if somebody won the entire game (not just a round)
                            //will be 'a' if player won, will be 'b' if computer won, will be ' ' if no winner yet
        buff[BUFFSIZE - 1] = (char)NULL;
        printf("%s", buff);
        memset(buff, 0, BUFFSIZE);
    }
    if(close(clientsoc) < 0){
        printf("error closing clientsoc\n");
        exit(4);
    }
    if(winner == 'a'){
        printf("The player has won the game.\n");
    }
    else if(winner == 'b'){
        printf("The computer has won the game.\n");
    }
    else{
        printf("Server has been closed\n");
    }

    printf("closed clientsoc successfully\n");
    return 0;
}
//recv(clientsoc, buff, BUFFSIZE, 0);
//printf("%s", buff);
//sprintf(buff, "from client\n");
//send(clientsoc, buff, BUFFSIZE, 0);

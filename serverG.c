#include "server.h"
////////////////////////////////////////////////////////////////////////////////
// ServerG Driver
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    time_t t;

    srand((unsigned) time(&t));

    int options[4] = {3 , 5 , 7 , 9} , //helps to determine number of rounds
    score_a  = 0 , score_b = 0 ,
    game_number = 0,
    random_number = (rand() % 4),
    total_win = options[random_number]/2 + 1,
    total_game = options[random_number];
    char choice_a[BUFFSIZE], choice_b[BUFFSIZE];

    int socfd, newsoc, err, id = (rand() % 99999);  //id will help differentiate between clients
    struct sockaddr_in serveradd;
    char buff[BUFFSIZE], bufft[BUFFSIZE];
    struct sockaddr_in newaddr;

    socklen_t addr_size;

    pid_t childpid;

    sscanf(argv[1], "%d", &socfd); //take the server socket fd from execl in serverC

    memset(&serveradd, '\0', sizeof(serveradd)); //sub-server for client communication
    serveradd.sin_family = AF_INET;
    serveradd.sin_port = htons(PORT);
    serveradd.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("%d this will be a game of %d  out of  %d \n" ,id, total_win , total_game );
    sprintf(buff, "this will be a game of %d  out of  %d \n" , total_win , total_game );
    send(socfd, buff, BUFFSIZE, 0); //notify server of number of rounds

    while((game_number < total_game) && (score_a < total_win) && (score_b < total_win)){
        recv(socfd, buff, BUFFSIZE, 0); //get user option
        strcpy(choice_a, buff);
        serverInput(choice_b); //server chooses move
        printf("%d player %s:%s computer\n", id, choice_a, choice_b);
        //determine tie, player wins, computer wins, or game exit
        findWinner(choice_a, choice_b, bufft, &game_number, &score_a, &score_b);
        if(strcmp(choice_a, "exit") == 0){
            send(socfd, choice_a, BUFFSIZE, 0);
            break;
        }
        memset(buff, BUFFSIZE, 0);
        sprintf(buff, "The computer player chose ");
        strcat(buff, choice_b);
        strcat(buff, "\n");
        send(socfd, buff, BUFFSIZE, 0); //notify user of computer choice
        memset(buff, 0, BUFFSIZE);
        recv(socfd, buff, BUFFSIZE, 0); //regain server control
        bufft[BUFFSIZE - 1] = ((score_a == total_win)? 'a' : ((score_b == total_win) ? 'b' : ' '));
        send(socfd, bufft, BUFFSIZE, 0); //send score and who won the round
        memset(buff, 0, BUFFSIZE); //reset buffers
        memset(bufft, 0, BUFFSIZE);
        memset(choice_a, 0, BUFFSIZE);
        memset(choice_b, 0, BUFFSIZE);
        printf("%d score player %d:%d computer\n", id, score_a, score_b); //display in server what the score is
    }
    if(close(socfd) < 0){
        printf("%d failed to close socfd in server...\n", id);
        exit(5);
    }
    printf("%d game over\n", id); //notify in server that the game has ended
    exit(0);
    //return 0;
}

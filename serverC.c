#include "server.h"
////////////////////////////////////////////////////////////////////////////////
// ServerC Driver
////////////////////////////////////////////////////////////////////////////////
int main(){

    int socfd, ret, newsoc, err, clientcount = 0, status;
    struct sockaddr_in serveradd;
    char buff[BUFFSIZE], rbuff[BUFFSIZE];
    struct sockaddr_in newaddr;

    socklen_t addr_size;

    pid_t childpid;

    socfd = socket(AF_INET, SOCK_STREAM, 0); //start socket

    if(socfd < 0){
        printf("connection error\n");
        exit(1);
    }

    printf("server socket created\n");

    memset(&serveradd, '\0', sizeof(serveradd)); //prepare server socket
    serveradd.sin_family = AF_INET;
    serveradd.sin_port = htons(PORT);
    serveradd.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(socfd, (struct sockaddr*)&serveradd, sizeof(serveradd)); //bind socket

    if(ret < 0){
        printf("error in binding...\n");
        exit(1);
    }

    printf("bound to port %d\n", PORT);

    if(listen(socfd,10) == 0){
        printf("listening...\n");
    }

    else{
        printf("error in listening...\n");
        exit(7);
    }

    while(1){ //server loop
        newsoc = accept(socfd, (struct sockaddr*)&newaddr, &addr_size);
        if(newsoc < 0){
            exit(1);
        }
        printf("connection has been accepted by %s: %d\n", inet_ntoa(newaddr.sin_addr), ntohs(newaddr.sin_port));
        childpid = fork();
        if(childpid == 0){ //starts serverG instance for a client

            sprintf(buff, "%d", newsoc);
            close(socfd);
            execl("./serverG", "serverG", buff, (char *)NULL);
            printf("executed?\n");
        }
        close(newsoc);
        clientcount++;
        printf("client count: %d\n", clientcount);
        while (clientcount){ /* Clean up all zombies */
            ret = waitpid((pid_t) -1, &status, WNOHANG);  /* Non-blocking wait */
            if (ret < 0){  /* waitpid() error? */
                printf("fork did not close properly pid = %d\n", childpid);
                exit(6);
            }
            else if (ret > 0){
                clientcount--;  /* Cleaned up after a child */
                printf("client closed %d\n", WEXITSTATUS(status));
                break;
            }
            break;
        }
        printf("client count: %d\n", clientcount);
    }

    return 0;
}

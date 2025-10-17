#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>


int main( int argc, char * argv[]){
    if (argc != 2){
        perror("Usage : ./server port");
        return 1;
    }

    int port = atoi(argv[1]);

    int socket1 = socket(AF_INET, SOCK_STREAM,0);
    if (socket1 == -1){
        perror("Socket creation error");
        return 1;
    }

    struct sockaddr_in addrServ;
    addrServ.sin_family = AF_INET;
    int err_inet_aton = inet_aton("127.0.0.1",&addrServ.sin_addr);
    if (err_inet_aton == 0){
        perror("Inet_aton error");
        return 1;
    }
    addrServ.sin_port = htons(port);
    bzero(addrServ.sin_zero,8);

    int bind1 = bind(socket1,(struct sockaddr*)&addrServ, sizeof(struct sockaddr));
    if (bind1 == -1){
        perror("Bind error");
        return 1;
    }

    int listen1 = listen(socket1,1);
    if (listen1 == -1){
        perror("Listen error");
        return 1;
    }

    struct sockaddr_in addrClient;
    socklen_t clientSize = sizeof(addrClient);
    int socketClient = accept(socket1, (struct sockaddr*)&addrClient, &clientSize);
    if (socketClient == -1) {
        perror("Accept error");
        close(socket1);
        return 1;
    }

    const char *msg = "Je suis le serveur \n";
    send(socketClient, msg, strlen(msg), 0);

    close(socket1);

    printf("Server closed.\n");
    return 0;
}
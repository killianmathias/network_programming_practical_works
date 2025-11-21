#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>

#define BUFSIZE 20
int main( int argc, char * argv[]){
    if (argc != 4){
        perror("Usage : ./server ipD portD portL");
        return 1;
    }

    char message[BUFSIZE] = "Ceci est un message"; 
    message[BUFSIZE-1] = '\0';


    char* ipD = argv[1];
    int portD = atoi(argv[2]);
    int portL = atoi(argv[3]);

    int socket1 = socket(AF_INET, SOCK_DGRAM,0);
    if (socket1 == -1){
        perror("Socket creation error");
        return 1;
    }

    struct sockaddr_in addrL;
    addrL.sin_family = AF_INET;
    addrL.sin_addr.s_addr = INADDR_ANY;
    addrL.sin_port = htons(portL);
    bzero(addrL.sin_zero,8);

    int bind1 = bind(socket1,(struct sockaddr*)&addrL, sizeof(struct sockaddr));
    if (bind1 == -1){
        perror("Bind error");
        return 1;
    }

    struct sockaddr_in addrD;
    addrD.sin_family = AF_INET;
    int err_inet_aton = inet_aton(ipD,&addrD.sin_addr);
    if (err_inet_aton == 0){
        perror("Inet_aton error");
        return 1;
    }
    addrD.sin_port = htons(portD);
    bzero(addrD.sin_zero,8);

    ssize_t err = sendto(socket1, message, BUFSIZE,0, &addrD, sizeof(struct sockaddr));

    int close_err = close(socket1);
    printf("Server closed.\n");
    return 0;
}
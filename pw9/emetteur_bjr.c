#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>

#define BUFSIZE 256
int main( int argc, char * argv[]){
    if (argc != 5){
        printf("%d",argc);
        perror("Usage : ./server ipD portD portL nom");
        return 1;
    }

    char* ipD = argv[1];
    int portD = atoi(argv[2]);
    int portL = atoi(argv[3]);
    char * nom = argv[4];

    char result[BUFSIZE];
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

    sendto(socket1, nom, strlen(nom), 0,(struct sockaddr*)&addrD, sizeof(struct sockaddr_in));

    ssize_t recv_err = recvfrom(socket1, result, BUFSIZE, 0, NULL, NULL);

    printf("%s", result);
    int close_err = close(socket1);
    printf("\nServer closed.\n");
    return 0;
}
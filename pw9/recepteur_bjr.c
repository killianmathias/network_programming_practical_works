#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#define BUFSIZE 512

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage : ./client_tp port\n");
        return 1;
    }

    char buffer[BUFSIZE];
    int client_port = atoi(argv[1]);

    int socket1 = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket1 == -1) {
        perror("Socket error");
        return 1;
    }

    // Client address
    struct sockaddr_in addrL;
    addrL.sin_family = AF_INET;
    if (inet_aton("127.0.0.1", &addrL.sin_addr) == 0) {
        fprintf(stderr, "inet_aton error\n");
        return 1;
    }
    addrL.sin_port = htons(client_port);
    bzero(addrL.sin_zero, 8);

    if (bind(socket1, (struct sockaddr*)&addrL, sizeof(struct sockaddr_in)) == -1) {
        perror("Bind error");
        return 1;
    }

    // Server address (will be filled by recvfrom)
    struct sockaddr_in addrD;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    char name[BUFSIZE];
    ssize_t recv_err = recvfrom(socket1, name, BUFSIZE - 1, 0,
                                (struct sockaddr*)&addrD, &addr_len);
    if (recv_err < 0) {
        perror("recvfrom");
        return 1;
    }
    name[recv_err] = '\0'; // secure string

    // Build response
    strcpy(buffer, "Bonjour ");
    strcat(buffer, name);

    ssize_t err = sendto(socket1, buffer, strlen(buffer), 0,
                         (struct sockaddr*)&addrD, sizeof(struct sockaddr_in));
    if (err < 0) {
        perror("sendto");
        return 1;
    }

    close(socket1);
    return 0;
}
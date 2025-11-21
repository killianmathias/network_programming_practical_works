#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>

#define BUFSIZE 512
int main(int argc, char* argv[]){
    if (argc !=2){
        fprintf(stderr, "Usage : ./client_tp port\n");
        return 1;
    }
    char buffer[BUFSIZE];

    int client_port = atoi(argv[1]);
    
    int socket1 = socket(AF_INET, SOCK_DGRAM,0);
    if (socket1 == -1){
        fprintf(stderr, "Erreur lors de la création de la socket.\n");
        return 1;
    }



    //Server
    // struct sockaddr_in addrD;
    // addrD.sin_family = AF_INET;
    // int err_inet_aton = INADDR_ANY;

    // addrD.sin_port = htons(server_port);
    // bzero(addrD.sin_zero,8);

    //Client
    struct sockaddr_in addrL;
    addrL.sin_family = AF_INET;
    int err_inet_aton = inet_aton("127.0.0.1", &addrL.sin_addr);

    addrL.sin_port = htons(client_port);
    bzero(addrL.sin_zero,8);

    int bind1 = bind(socket1,(struct sockaddr*)&addrL, sizeof(struct sockaddr));
    if (bind1 == -1){
        perror("Bind error");
        return 1;
    }
    ssize_t recv_err = recvfrom(socket1, buffer, BUFSIZE, 0, NULL, NULL);
    
    printf("%s", buffer);

    
   
    //Méthode 1
    // for (int i = 0; i<10;i++){
    //     ssize_t response = recv(socket1,buffer+i*10,10,0);
    //     fprintf(stdout, "Données reçues : %d octets.\n",response);
    //     if (response==0){
    //         fprintf(stderr,"Déconnexion côté serveur\n");
    //         return 1;
    //     }
    //     if (response == -1){
    //         fprintf(stderr,"Erreur\n");
    //     }
    // }
    

    int close_err = close(socket1);
    if (close_err == -1){
        perror("Erreur close.");
        return 0;
    }
}
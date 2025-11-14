#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>

#define BUFSIZE 101
#define PI 100004
int main(int argc, char ** argv){
    if (argc !=3){
        fprintf(stderr, "Usage : ./client_tp hostname port\n");
        return 1;
    }

    char * hostname = argv[1];
    int port = atoi(argv[2]);
    
    int socket1 = socket(AF_INET, SOCK_STREAM,0);
    if (socket1 == -1){
        fprintf(stderr, "Erreur lors de la création de la socket.\n");
        return 1;
    }
    struct sockaddr_in addrServ;
    addrServ.sin_family = AF_INET;
    int err_inet_aton = inet_aton(hostname,&addrServ.sin_addr);

    addrServ.sin_port = htons(port);

    bzero(addrServ.sin_zero,8);
    int err = connect(socket1,(struct sockaddr*)&addrServ,sizeof(struct sockaddr));

    if (err){
        perror("Could not connect");
        return 1;
    }

    char buffer[PI];
    ssize_t response = recv(socket1,&buffer,PI,0);
    fprintf(stdout, "Données reçues : %d octets.\n",response);
    if (response==0){
        fprintf(stderr,"Déconnexion côté serveur\n");
        return 1;
    }
    if (response == -1){
        fprintf(stderr,"Erreur\n");
    }
   
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
    buffer[PI-1] = '\0';
    // fprintf(stdout, buffer);

    int err_shutdown = shutdown(socket1, SHUT_RDWR);
    if (err_shutdown==-1){
        fprintf(stderr, "Erreur de shutdown\n");
        return 1;
    }

    int close_err = close(socket1);
    if (close_err == -1){
        perror("Erreur close.");
        return 0;
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){
    
    int sockfd, newsockfd, portno, n;
    struct sockaddr_in serv_addr;

    char buffer[1024];

    if(argc < 3) {
        printf("Not enough arguments. Example: ./a.out <IP> <port>\n");
        exit(1);
    }

    portno = atoi(argv[2]);
    //portno = 9898;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("ERROR in opening socket.\n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection faild...!!!\n");
        exit(1);
    }

    for(;;) {
        
        bzero(buffer, 1024);
        fgets(buffer, 1024, stdin);

        if ((n = write(sockfd, buffer, strlen(buffer)) < 0)) {
            printf("ERROR in writing...\n");
            exit(1);
        }

        bzero(buffer, 1024);
        if ((n = read(sockfd, buffer, 1024) < 0)) {
            printf("ERROR in reading...\n");
            exit(1);
        }

        printf("\nServer --> %s", buffer);

        if (!strncmp("bye", buffer, 3))
            break;        
    }

    close(sockfd);
    return 0;
}
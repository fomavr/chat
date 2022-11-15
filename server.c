#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    int sockfd, newsockfd, n;
    struct sockaddr_in serv_addr, client_addr;

    char buffer[1024];

    socklen_t len;

    if(argc < 2) {
        printf("Not enough arguments. Example: ./a.out <port>\n");
        exit(1);
    }

    //portno = 9898;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("ERROR in opening socket.\n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        printf("ERROR in binding socket\n");
    
    listen(sockfd, 5);

    len = sizeof(client_addr);

    if ((newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &len)) < 0)
        printf("ERROR in accepting request...");
    
    for(;;) {
        bzero(buffer, 1024);
        if ((n = read(newsockfd, buffer, 1024)) < 0) {
            printf("ERROR in reading...");
            exit(1);
        }
        printf("\nClient --> %s", buffer);

        fgets(buffer, 1024, stdin);
        if ((n = write(newsockfd, buffer, 1024)) < 0) {
            printf("ERROR in writing...");
            exit(1);
        }

        if (!strncmp("bye", buffer, 3))
            break;
    }
    close(newsockfd);
    close(sockfd);

    return 0;
}
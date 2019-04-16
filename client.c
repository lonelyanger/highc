#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "aes_options.h"           //add

int main()
{
    int len;
    int client_sockfd;  
    struct sockaddr_in server_addr;
    char buffer[BUFSIZ];
    char *encrypt_string = NULL;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(9000);

    if((client_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket create failed");
        return 1;

    }


    if(connect(client_sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("connect failed");
        return 1;
    }

    printf("connect to server\n");
    len = recv(client_sockfd, buffer, BUFSIZ, 0);

    buffer[len] = '\0';
    printf("%s", buffer);

    while(1)
    {
        printf("enter a data:");
        scanf("%s", buffer);
        if(!strcmp(buffer,"quit"))
            break;
        int encrypt_length = encrypt(buffer, &encrypt_string);     //add
        len = send(client_sockfd, encrypt_string, encrypt_length, 0); //add
        len = recv(client_sockfd, buffer, BUFSIZ, 0);
        buffer[len] = '\0';
        printf("recived:%s \n", buffer);
    }

    close(client_sockfd);
    printf("bye");


    return 0;
}

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "aes_options.h"   //add

int main()
{
    int server_fd;
    int client_fd;
    int len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sin_size;
    char buffer[BUFSIZ];
    // printf("%d",BUFSIZ);
    memset(&server_addr, 0, sizeof(server_addr));  //initialize struct
    memset(&server_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9000);

    if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)   //create server socket
    {
        perror("socket create failed");
        return 1;
    }

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) //bind info on server socket
    {
        perror("bind failed");
        return 1;
    }

    listen(server_fd, 5);   //listen port 9000 

    sin_size = sizeof(struct sockaddr_in);

    if((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &sin_size)) < 0)
    {
        perror("accept failed");
        return 1;   
    }

    printf("accept client %s\n", inet_ntoa(client_addr.sin_addr));
    len = send(client_fd, "Welcome to my server\n", 21, 0);

    while((len=recv(client_fd, buffer, BUFSIZ, 0)) > 0)
    {

        char *decryto_string = NULL;                          //add
        decrypt(buffer, &decryto_string, len);                //add
        printf("%s \n", decryto_string);
        if(send(client_fd, decryto_string, len, 0) < 0)       //modified
        {
            perror("send failed");
            return 1;
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}

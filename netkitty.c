#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFSIZE 128

//I worked with Hunter Locher on this assignment

int nkServer(int portNumber);
int nkHelper(void);
int nkClient(char *ipAddr, int portNumber,int argc,char *argv[]);
int main(int argc, char *argv[])
{
    int options;
    int portNumber = 0;
    while(( options = getopt(argc, argv, "l:h")) != -1)
    {
        switch(options)
        {
            case 'l':
            {   
                portNumber = atoi(optarg);
                nkServer(portNumber);
                return 0;
            }
            case 'h':
            {
                nkHelper();
                break;
            }
        }
    }

    if(-1 == options)
    {
        int portNumber = atoi(argv[2]);
        char *ipAddr = argv[1];
        nkClient(ipAddr, portNumber,argc,argv);
    }


    return 0;
}
int nkClient(char *ipAddr, int portNumber,int argc,char *argv[])
{   
    int sock = 0, valread,Y;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNumber);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if (connect(sock, (struct sockaddr*)&serv_addr,
                sizeof(serv_addr))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
  
    
    while(1){
        
        char *line = NULL;
        size_t len = 0;
        ssize_t linesize = 0;
        linesize = getline(&line,&len,stdin);
        Y = write(sock, line,len);
        free(line);
        
    }


    return 0;

}

//error is with the setup of server, not with reading and writing
int nkServer(int portNumber)
{   

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
        == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portNumber);

 
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    send(new_socket, hello, buffer,0);
    while(1){
    
        valread = read(new_socket, buffer, 1024);
        printf("%s", buffer);

    }
    return 0;

}


int nkHelper(void)
{
    printf("Arguments:\n\t-l: start server\n\t-h: help\nClient instructions:\nEnter 127.0.0.1 loopback IP address and port 8080\n");
}

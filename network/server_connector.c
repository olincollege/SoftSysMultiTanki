// For server to connect two peers
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define TRUE   1 
#define FALSE  0 
#define PORT 7777
#define MAX_PLAYERS 2

void sending();
void receiving(int server_fd);
void *receive_thread(void *server_fd);

int main(int argc, char const *argv[]){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen, i, k = 0;
    int max_clients = MAX_PLAYERS, client_socket[MAX_PLAYERS];
    int opt = TRUE;

    // create a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    //set socket to allow multiple connections
    if( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  

    // attaching socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // assign port
    //address.sin_port = 0; // randomly assign open port

    // associate server socket address with socket descriptor
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // ready to accept connection requests
    if (listen(server_fd, MAX_PLAYERS) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);  
    puts("Waiting for connections ...");
}
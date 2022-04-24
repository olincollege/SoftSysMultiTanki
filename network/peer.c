// Peer to use for network communication
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MATCHMAKING_HOST "127.0.0.1"//"157.245.235.105"
#define PORT 7777
#define ERROR 1

struct args_struct {
  int *network_socket;
  struct sockaddr_in *peer_address;
};

// "declarations", code is easier to read
void sending();
void receiving(int server_fd);
void *receive_thread(void *server_fd);

char peer_ip_addr[INET_ADDRSTRLEN];

// first for connecting to other peer
int matchmaking() {
    struct sockaddr_in server;
    struct hostent * hp;
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    hp = getaddrinfo(MATCHMAKING_HOST);
    if ( hp == NULL )
    {
        perror("Unknown host");
        return(ERROR);
    }
    int sock;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket creating error");
        return(ERROR);
    }
    server.sin_addr.s_addr = inet_addr(MATCHMAKING_HOST);

    // connect to host
    connect(sock, (struct sockaddr *) &server, sizeof(server));

    // receive data
    int ip_received = recv(sock, peer_ip_addr, INET_ADDRSTRLEN, 0);
    if (ip_received == -1){
        perror("Can't read from host");
        return(ERROR);
    }

    // close connection
    close(sock);
    return(0);
}


int main(int argc, char const *argv[]){
    // find other peer
    if (!matchmaking()) {
        perror("Matchmaking failed");
        return(ERROR);
    }

    // connect to peer
    int network_socket = 0, valread;
    struct sockaddr_in serv_addr;
    char hello[1024] = {0};
    char buffer[2000] = {0};

    // create socket with UDP protocol
    if ((network_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP)) < 0)
    {
        printf("\n Socket creation error \n");
        return(ERROR);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_aton(peer_ip_addr, &serv_addr);
    serv_addr.sin_port = htons(PORT);

    // associate server socket address with socket descriptor
    if (bind(network_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Bind failed");
        return(ERROR);
    }
    // ready to accept connection requests
    if (listen(network_socket, 5) < 0)
    {
        perror("listen");
        exit(ERROR);
    }
}
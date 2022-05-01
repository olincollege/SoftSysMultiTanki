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
#define NET_BUF_SIZE 32
#define sendrecvflag 0
#define FILENAME "test.json"

char peer_ip_addr[INET_ADDRSTRLEN];

// clear buffer
void clear_buf(char* buf) {
    int i;
    for (i=0; i < NET_BUF_SIZE; i++)
        buf[i] = '/0';
}

// first for connecting to other peer
int matchmaking() {
    struct sockaddr_in server;
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    
    int sock;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket creating error");
        exit(EXIT_FAILURE);
    }
    server.sin_addr.s_addr = inet_addr(MATCHMAKING_HOST);

    // connect to host
    connect(sock, (struct sockaddr *) &server, sizeof(server));

    // receive data
    int ip_received;
    if ((ip_received=recv(sock, peer_ip_addr, INET_ADDRSTRLEN, 0)) == -1){
        perror("Can't read from host");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", peer_ip_addr);

    // close connection
    close(sock);
    return(0);
}

// function to send file
char nofile[15] = "No file found!";
int send_file(FILE* fp, char* buf, int buf_size){
    int i, len;
    // if file NULL
    if (fp == NULL) {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        return(1);
    }
    // read file
    char ch;
    for (i = 0; i<buf_size;i++) {
        // read from next pointer position
        ch = fgetc(fp);
        buf[i] = ch;
        if (ch==EOF){
            return(1);
        }
    }
    return(0);
}

// function to receive file
int recv_file(char* buf, int buf_size) {
    int i;
    char ch;
    for (i=0; i<buf_size; i++) {
        ch = buf[i];
        if (ch == EOF)
            return(1);
        else
            printf("%c", ch);
    }
    return(0);
}


int main(int argc, char const *argv[]){
    // set up
    FILE* fp;
    char net_buf[NET_BUF_SIZE];

    // find other peer
    if (!matchmaking()) {
        perror("Matchmaking failed");
        exit(EXIT_FAILURE);
    }

    // connect to peer
    int network_socket, nBytes;
    struct sockaddr_in serv_addr;
    int addrlen = sizeof(serv_addr);

    // create socket with UDP protocol (uses datagram)
    if ((network_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_aton(peer_ip_addr, &serv_addr);
    serv_addr.sin_port = htons(PORT);

    // associate server socket address with socket descriptor
    if (bind(network_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {;
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    // ready to accept connection requests
    if (listen(network_socket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // TODO: fork two different processes: receive file, send file
    if (fork()==0){
        // case for asking for file
        //printf("Wait for file name");
        //nBytes = recvfrom(network_socket, net_buf,NET_BUF_SIZE, sendrecvflag,(struct sockaddr*)&serv_addr, &addrlen);
        //FILENAME is netbuf instead in this case
        strcpy(net_buf,FILENAME);
        // open file
        fp = fopen(net_buf, "r");
        if (fp==NULL){
            printf("\nFile open failed!\n");
            exit(EXIT_FAILURE);
        }
        while(1) {
            if (sendFile(fp, net_buf, NET_BUF_SIZE)) {
                sendto(network_socket, net_buf, NET_BUF_SIZE,
                sendrecvflag, (struct sockaddr*)&serv_addr, addrlen);
                break;
            }
            // send file
            sendto(network_socket, net_buf, NET_BUF_SIZE,
            sendrecvflag, (struct sockaddr*)&serv_addr, addrlen);
            clear_buf(net_buf);
        }
    }
    if (fp!=NULL)
        fclose(fp);
    
    // receive code
    else {
        // if asking for name of file
        //printf("\nPlease enter file name to receive:\n");
        //scanf("%s", net_buf);
        //sendto(network_socket, net_buf, NET_BUF_SIZE,
        //       sendrecvflag, (struct sockaddr*)&serv_addr,
        //       addrlen);

        // receive
        while (1) {
            // receive file
            clear_buf(net_buf);
            nBytes = recvfrom(network_socket, net_buf, NET_BUF_SIZE,
            sendrecvflag, (struct sockaddr*)&serv_addr, addrlen);
            clear_buf(net_buf);

            // process file
            if (recv_file(net_buf, NET_BUF_SIZE)) {
                break;
            }
        }

    }

    atexit (close(network_socket));
    
}
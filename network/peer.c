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

char peer_ip_addr[INET_ADDRSTRLEN];
int tank_no;

struct KeyInfo {
   uint8_t  type;
   uint8_t  name;
   uint32_t data;
};

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
    int tank_no_received;
    if ((tank_no_received=recv(sock, &tank_no, sizeof(int), 0))==-1){
        perror("can't reed from host");
        exit(EXIT_FAILURE);
    }
    printf("%d\n", ntohl(tank_no));
    
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

int main(int argc, char const *argv[]){
    // find other peer
    if (!matchmaking()) {
        perror("Matchmaking failed");
        exit(EXIT_FAILURE);
    }

    // connect to peer
    int peer_sock;
    struct sockaddr_in serv_addr;
    int addrlen = sizeof(serv_addr);

    // create socket with UDP protocol (uses datagram)
    if ((peer_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_aton(peer_ip_addr, &serv_addr);
    serv_addr.sin_port = htons(PORT);

    // associate server socket address with socket descriptor
    if (bind(peer_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {;
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    // ready to accept connection requests
    if (listen(peer_sock, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //TO DO: organize by tank, apply to input and server

    // this is good reference for the packet
    int keys_buf_size = 18;
    // for keyboard
    // unpressed
    uint8_t keyboard_up_type = 0;
    uint16_t keyboard_up_data = 0; // which key
    // pressed
    uint8_t keyboard_down_type = 1;
    uint16_t keyboard_down_data = 0; // which key

    // for mouse button
    uint8_t mouse_button_type = 2;
    uint8_t mouse_button_data = 0; //1 for pressed

    // for mouse state
    // x
    uint8_t mouse_state_x_type = 3;
    uint32_t mouse_state_x_data = 0;
    // y
    uint8_t mouse_state_y_type = 4;
    uint32_t mouse_state_y_data = 0;

    // send code
    // TO DO: to be placed within input code
    if (fork()==0){
        // setup packet
        char keys_buffer[keys_buf_size];

        // make sure to update data with input code

        // put into buffer
        memcpy(&keys_buffer[0],&keyboard_up_type,1); //uint8_t
        memcpy(&keys_buffer[1],&keyboard_up_data,2); //uint16_t
        memcpy(&keys_buffer[3],&keyboard_down_type,1); //uint8_t
        memcpy(&keys_buffer[4],&keyboard_down_data,2); //uint16_t
        memcpy(&keys_buffer[6],&mouse_button_type,1); //uint8_t
        memcpy(&keys_buffer[7],&mouse_button_data,1); //uint8_t
        memcpy(&keys_buffer[8],&mouse_state_x_type,1); //uint8_t 
        memcpy(&keys_buffer[9],&mouse_state_x_data,4); //uint32_t
        memcpy(&keys_buffer[13],&mouse_state_y_type,1); //uint8_t 
        memcpy(&keys_buffer[14],&mouse_state_y_data,4); //uint32_t

        // send packet
        if (send(peer_sock, &keys_buffer, keys_buf_size, 0) == -1){
            perror("Can't read from host");
            exit(EXIT_FAILURE);
        }

        // LOOP AROUND THIS
    }
    
    // receive code
    else {
        // setup packet
        int keys_buf_size = 18;
        char keys_buffer[keys_buf_size];

        // receive packet
        if (recv(peer_sock, keys_buffer, keys_buf_size, 0) == -1){
            perror("Can't read from host");
            exit(EXIT_FAILURE);
        }
        // parse packet
        memcpy(&keyboard_up_type,&keys_buffer[0],1); //uint8_t
        memcpy(&keyboard_up_data,&keys_buffer[1],2); //uint16_t
        memcpy(&keyboard_down_type,&keys_buffer[3],1); //uint8_t
        memcpy(&keyboard_down_data,&keys_buffer[4],2); //uint16_t
        memcpy(&mouse_button_type,&keys_buffer[6],1); //uint8_t
        memcpy(&mouse_button_data,&keys_buffer[7],1); //uint8_t
        memcpy(&mouse_state_x_type,&keys_buffer[8],1); //uint8_t 
        memcpy(&mouse_state_x_data,&keys_buffer[9],4); //uint32_t
        memcpy(&mouse_state_y_type,&keys_buffer[13],1); //uint8_t 
        memcpy(&mouse_state_y_data,&keys_buffer[14],4); //uint32_t

        // LOOP AROUND THIS
    }
    atexit(close(peer_sock));
}
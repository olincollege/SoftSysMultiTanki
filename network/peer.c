// Peer to use for network communication
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

/*
main.c
input.c: record your input
structs.c: playerIndex (0,1)
player.c: Player *p
head angle
body body angle + location

app.playerIndex
ignore begining of server.

need:
 x y
 dx dy
 angle

next:
 bullets incl in player
 bp collision detection bullet points

 at best, send whole player over
*/

#define MATCHMAKING_HOST "157.245.235.105"
#define PORT 7777

char peer_ip_addr[INET_ADDRSTRLEN];
int tank_no;

struct KeyInfo {
    uint8_t keyboard_up_type;
    uint16_t keyboard_up_data; // which key
    uint8_t keyboard_down_type;
    uint16_t keyboard_down_data; // which key

    // for mouse button
    uint8_t mouse_button_type;
    uint8_t mouse_button_data; //1 for pressed

    // for mouse state
    // x
    uint8_t mouse_state_x_type;
    uint32_t mouse_state_x_data;
    // y
    uint8_t mouse_state_y_type;
    uint32_t mouse_state_y_data;
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
    tank_no = ntohl(tank_no);
    printf("%i\n", tank_no);
    
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
    if (matchmaking() == -1) {
        perror("Matchmaking failed");
        exit(EXIT_FAILURE);
    }

    /* set up server
    int network_socket;
    struct sockaddr_in server_address, peer_address;

    // create socket file descriptor with UDP protocol (uses datagram)
    if ((network_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // associate server socket address with socket descriptor
    if (bind(network_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    socklen_t serv_len = sizeof(server_address);

    // connect to peer
    peer_address.sin_family = AF_INET;
    peer_address.sin_addr.s_addr = inet_addr(peer_ip_addr);
    peer_address.sin_port = htons(PORT);

    socklen_t peer_len = sizeof(peer_address);

    //TO DO: organize by tank, apply to input and server*/

    // this is good reference for the packet
    int keys_buf_size = 18;
    char my_keys_buffer[keys_buf_size];
    char peer_keys_buffer[keys_buf_size];

    struct KeyInfo my_keys;
    struct KeyInfo peer_keys = {0,0,0,0,0,0,0,0,0,0};

    // for keyboard
    // unpressed
    my_keys.keyboard_up_type = 0;
    my_keys.keyboard_up_data = 0; // which key
    // pressed
    my_keys.keyboard_down_type = 1;
    my_keys.keyboard_down_data = 0; // which key

    // for mouse button
    my_keys.mouse_button_type = 2;
    my_keys.mouse_button_data = 0; //1 for pressed

    // for mouse state
    // x
    my_keys.mouse_state_x_type = 3;
    my_keys.mouse_state_x_data = 0;
    // y
    my_keys.mouse_state_y_type = 4;
    my_keys.mouse_state_y_data = 0;

    // sending example
    if (tank_no==1) {
        // setup connection
        int sockfd;
        char buffer[1024];
        struct sockaddr_in servaddr, cliaddr;
        char *hello = "Hello from server";
        if ((sockfd= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        servaddr.sin_family    = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(PORT);

        // Bind the socket with the server address
        if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
                sizeof(servaddr)) < 0 )
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        
        int len, n;
    
        len = sizeof(cliaddr);  //len is value/result
    
        n = recvfrom(sockfd, (char *)buffer, 1024, 
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                    &len);
        buffer[n] = '\0';
        printf("Client : %s\n", buffer);
        sendto(sockfd, (const char *)hello, strlen(hello), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                len);
        printf("Hello message sent.\n");

        // put into buffer
        memcpy(&my_keys_buffer[0],&my_keys.keyboard_up_type,1); //uint8_t
        memcpy(&my_keys_buffer[1],&my_keys.keyboard_up_data,2); //uint16_t
        memcpy(&my_keys_buffer[3],&my_keys.keyboard_down_type,1); //uint8_t
        memcpy(&my_keys_buffer[4],&my_keys.keyboard_down_data,2); //uint16_t
        memcpy(&my_keys_buffer[6],&my_keys.mouse_button_type,1); //uint8_t
        memcpy(&my_keys_buffer[7],&my_keys.mouse_button_data,1); //uint8_t
        memcpy(&my_keys_buffer[8],&my_keys.mouse_state_x_type,1); //uint8_t 
        memcpy(&my_keys_buffer[9],&my_keys.mouse_state_x_data,4); //uint32_t
        memcpy(&my_keys_buffer[13],&my_keys.mouse_state_y_type,1); //uint8_t 
        memcpy(&my_keys_buffer[14],&my_keys.mouse_state_y_data,4); //uint32_t
        // send packet
        sendto(sockfd, (const char *)my_keys_buffer, strlen(my_keys_buffer), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                len);
        printf("Packet sent.\n");
        
    }
    if (tank_no == 2) {
        // setup connection
        int sockfd;
        char buffer[1024];
        char *hello = "Hello from client";
        struct sockaddr_in     servaddr;
    
        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }
    
        memset(&servaddr, 0, sizeof(servaddr));
        
        // Filling server information
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;
        
        int n,m, len;
        
        sendto(sockfd, (const char *)hello, strlen(hello),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
                sizeof(servaddr));
        printf("Hello message sent.\n");
            
        n = recvfrom(sockfd, (char *)buffer, 1024, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);

        m = recvfrom(sockfd, (char *)peer_keys_buffer, keys_buf_size, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);
        //peer_keys_buffer[m] = '\0';
        printf("Server : %s\n", peer_keys_buffer);

        // parse packet
        //apply ntohl for int

        memcpy(&peer_keys.keyboard_up_type,&peer_keys_buffer[0],1); //uint8_t
        memcpy(&peer_keys.keyboard_up_data,&peer_keys_buffer[1],2); //uint16_t
        memcpy(&peer_keys.keyboard_down_type,&peer_keys_buffer[3],1); //uint8_t
        memcpy(&peer_keys.keyboard_down_data,&peer_keys_buffer[4],2); //uint16_t
        memcpy(&peer_keys.mouse_button_type,&peer_keys_buffer[6],1); //uint8_t
        memcpy(&peer_keys.mouse_button_data,&peer_keys_buffer[7],1); //uint8_t
        memcpy(&peer_keys.mouse_state_x_type,&peer_keys_buffer[8],1); //uint8_t 
        memcpy(&peer_keys.mouse_state_x_data,&peer_keys_buffer[9],4); //uint32_t
        memcpy(&peer_keys.mouse_state_y_type,&peer_keys_buffer[13],1); //uint8_t 
        memcpy(&peer_keys.mouse_state_y_data,&peer_keys_buffer[14],4); //uint32_t
        
        printf("%i", peer_keys.keyboard_down_type);
        close(sockfd);
    }
    // receiving
    // TO DO: to be placed within input code
   
}
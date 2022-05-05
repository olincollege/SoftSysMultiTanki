#include "server.h"

#define KEYS_BUF_SIZE 13
#define MAXLINE 100

/*// for debugging
void btox(char *xp, char *bb, int n) {
    const char xx[]= "0123456789ABCDEF";
    while (--n >= 0) xp[n] = xx[(bb[n>>1] >> ((1 - (n&1)) << 2)) & 0xF];
}*/

// convert from host to network byte order
struct KeyInfo h_to_n(struct KeyInfo key_info){
    key_info.mouse_state_x_data = htonl(key_info.mouse_state_x_data);
    key_info.mouse_state_y_data = htonl(key_info.mouse_state_y_data);
    return key_info;
}

// convert from network to host byte order
struct KeyInfo n_to_h(struct KeyInfo key_info){
    key_info.mouse_state_x_data = ntohl(key_info.mouse_state_x_data);
    key_info.mouse_state_y_data = ntohl(key_info.mouse_state_y_data);
    return key_info;
}

// return buffer from KeyInfo input values, to send
char * input_to_buf(char * my_keys_buffer){
    // get current inputs
    struct KeyInfo my_keys;
    // mouse state
    my_keys.mouse_button_data = app.playerInputs[app.playerIndex].mouse.button[SDL_BUTTON_LEFT];
    my_keys.mouse_state_x_data = app.playerInputs[app.playerIndex].mouse.x;
    my_keys.mouse_state_y_data = app.playerInputs[app.playerIndex].mouse.y;
    // keyboard state
    my_keys.w_key_data = app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_W];
    my_keys.a_key_data = app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_A];
    my_keys.s_key_data = app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_S];
    my_keys.d_key_data = app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_D];

    // convert to network byte order
    my_keys = h_to_n(my_keys);

    // put into buffer
    memcpy(&my_keys_buffer[0], &my_keys.w_key_data, 1); //uint8_t
    memcpy(&my_keys_buffer[1], &my_keys.a_key_data, 1); //uint8_t
    memcpy(&my_keys_buffer[2], &my_keys.s_key_data, 1); //uint8_t
    memcpy(&my_keys_buffer[3], &my_keys.d_key_data, 1); //uint8_t
    memcpy(&my_keys_buffer[4], &my_keys.mouse_button_data, 1); //uint8_t
    memcpy(&my_keys_buffer[5], &my_keys.mouse_state_x_data, 4); //int32_t
    memcpy(&my_keys_buffer[9], &my_keys.mouse_state_y_data, 4); //int32_t

    return(my_keys_buffer);
}

// return inputs in KeyInfo struct from buffer, run input from
void buf_to_input(char * peer_keys_buffer){
    // create empty KeyInfo struct
    struct KeyInfo peer_keys;

    // parse packet
    memcpy(&peer_keys.w_key_data, &peer_keys_buffer[0], 1); //uint8_t
    memcpy(&peer_keys.a_key_data, &peer_keys_buffer[1], 1); //uint8_t
    memcpy(&peer_keys.s_key_data, &peer_keys_buffer[2], 1); //uint8_t
    memcpy(&peer_keys.d_key_data, &peer_keys_buffer[3], 1); //uint8_t
    memcpy(&peer_keys.mouse_button_data, &peer_keys_buffer[4], 1); //uint8_t
    memcpy(&peer_keys.mouse_state_x_data, &peer_keys_buffer[5], 4); //int32_t
    memcpy(&peer_keys.mouse_state_y_data, &peer_keys_buffer[9], 4); //int32_t

    // convert to host byte order
    peer_keys = n_to_h(peer_keys);

    // apply inputs
    for (int i = 0; i < app.maxPlayer; i++)
    {
        if (app.playerIndex == i)
        {
            // continue if my index
            continue;
        }

        // run input
        app.playerInputs[i].keyboard[SDL_SCANCODE_W] = peer_keys.w_key_data;
        app.playerInputs[i].keyboard[SDL_SCANCODE_A] = peer_keys.a_key_data;
        app.playerInputs[i].keyboard[SDL_SCANCODE_S] = peer_keys.s_key_data;
        app.playerInputs[i].keyboard[SDL_SCANCODE_D] = peer_keys.d_key_data;
        app.playerInputs[i].mouse.button[SDL_BUTTON_LEFT] = peer_keys.mouse_button_data;

        app.playerInputs[i].mouse.x = peer_keys.mouse_state_x_data;
        app.playerInputs[i].mouse.y = peer_keys.mouse_state_y_data;
    }
}

struct MultiplayerInfo doMatchmaking(void)
{
    struct sockaddr_in server;
    int tank_no;
    struct MultiplayerInfo peer_info;
    char peer_ip_addr[INET_ADDRSTRLEN]; // store peer ipv4
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // create socket file descriptor
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
        perror("Can't read from host");
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

    // add info to struct
    peer_info.tank_no = tank_no;
    strcpy(peer_info.peer_ip_addr, peer_ip_addr);

    // return
    return peer_info;
}

// server code for tank 0
int server(struct MultiplayerInfo server_info){
    // create packet
    char my_buffer[KEYS_BUF_SIZE], peer_buffer[KEYS_BUF_SIZE];

    // setup
    int listenfd;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));

    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);        
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    // bind server address to socket descriptor
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    len = sizeof(cliaddr);
    
    while (1){
    // set up input buffer
    strcpy(my_buffer, input_to_buf(my_buffer));
    /* // debug
    char hex_buffer[1024];
    int m = KEYS_BUF_SIZE;
    btox(hex_buffer, my_buffer, m*2);
    printf("\nServer %d bytes: %s\n", m, hex_buffer);*/
    // receive message
    int n = recvfrom(listenfd, peer_buffer, KEYS_BUF_SIZE,
            0, (struct sockaddr*)&cliaddr,&len);
    peer_buffer[n] = '\0';
    // run peer inputs
    buf_to_input(peer_buffer);
    //send message
    sendto(listenfd, my_buffer, (size_t)KEYS_BUF_SIZE, 0,
          (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    }

    return(0);
}

// client code for tank 1
int client(struct MultiplayerInfo client_info){
    // create packet
    char my_buffer[KEYS_BUF_SIZE], peer_buffer[KEYS_BUF_SIZE];
    // setup
    int sockfd;
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr)); //clear server addr

    // set up server
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    // create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // connect to server, store peer ip and port
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }

    while (1) {
        // get inputs
        strcpy(my_buffer, input_to_buf(my_buffer));
        /*// debug
        char hex_buffer[1024];
        int m = KEYS_BUF_SIZE;
        btox(hex_buffer, my_buffer, m*2);
        printf("\nServer %d bytes: %s\n", m, hex_buffer);*/
        // request to send datagram
        sendto(sockfd, my_buffer, (size_t) KEYS_BUF_SIZE, MSG_DONTWAIT, (struct sockaddr*)NULL, sizeof(servaddr));
        // waiting for response
        int n = recvfrom(sockfd, peer_buffer, (size_t) KEYS_BUF_SIZE, 0, (struct sockaddr*)NULL, NULL);
        peer_buffer[n] = '\0';
        // run peer inputs
        buf_to_input(peer_buffer);
    }
  
    // close the descriptor
    close(sockfd);

    return(0);
}

// create and run server and client
int setupConnection(struct MultiplayerInfo net_info){
    // server case
    if (net_info.tank_no == 0){
        if (server(net_info) != 0){
            perror("Server issue");
            exit(EXIT_FAILURE);
        }
    }
    // client case
    if (net_info.tank_no == 1){
        if (client(net_info) != 0){
            perror("Client issue");
            exit(EXIT_FAILURE);
        }
    }
    return(0);
}
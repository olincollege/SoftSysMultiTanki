#include "server.h"

char peer_ip_addr[INET_ADDRSTRLEN]; // store peer ipv4
struct KeyInfo my_keys;
struct KeyInfo peer_keys;

int keys_buf_size = 13;

void btox(char *xp, char *bb, int n) {
    const char xx[]= "0123456789ABCDEF";
    while (--n >= 0) xp[n] = xx[(bb[n>>1] >> ((1 - (n&1)) << 2)) & 0xF];
}

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

struct MultiplayerInfo doMatchmaking(void)
{
    struct sockaddr_in server;
    int tank_no;
    struct MultiplayerInfo peer_info;
    
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

    // add info to struct
    peer_info.tank_no = tank_no;
    strcpy(peer_info.peer_ip_addr, peer_ip_addr);

    // return
    return peer_info;
}

// create socket file descriptor, server and client
struct MultiplayerInfo setupConnection(struct MultiplayerInfo net_info){
    // create file descriptor
    if ((net_info.sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    // clear
    memset(&net_info.servaddr, 0, sizeof(net_info.servaddr));
    memset(&net_info.cliaddr, 0, sizeof(net_info.cliaddr));
    // servaddr info
    net_info.servaddr.sin_family = AF_INET; // IPv4
    net_info.servaddr.sin_port   = htons(PORT);

    // server
    if (net_info.tank_no == 0){
        // set to localhost
        net_info.servaddr.sin_addr.s_addr = INADDR_ANY;
        // bind the socket with the server address
        if (bind(net_info.sockfd, (const struct sockaddr *)&net_info.servaddr, sizeof(net_info.servaddr)) < 0 ){
            perror("Bind failed");
            exit(EXIT_FAILURE);
        }
    }
    //client
    if (net_info.tank_no == 1){
        // connect to host ip
        net_info.servaddr.sin_addr.s_addr = inet_addr(net_info.peer_ip_addr);
    }
    return net_info;
}

// network code to send current values to server
void doSendToServer(struct MultiplayerInfo net_info)
{
    // get inputs
    char my_keys_buffer[keys_buf_size];

    for (;;) {
    memset(my_keys_buffer, 0, keys_buf_size);
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
    // clean later - use sizeof(typeof(my_keys.etc) in last arg)

    //send packet
    //server
    fflush(stdout);
    if (app.playerIndex == 0){
        // send
        sendto(net_info.sockfd, (const char *)my_keys_buffer, keys_buf_size, 
            MSG_CONFIRM, (const struct sockaddr *) &net_info.cliaddr,
            (socklen_t) sizeof(net_info.cliaddr));
    }
    //client
    if (app.playerIndex == 1){
        sendto(net_info.sockfd, (const char *)my_keys_buffer, keys_buf_size,
            MSG_CONFIRM, (const struct sockaddr *) &net_info.servaddr, 
            (socklen_t) sizeof(net_info.servaddr));
    }
    /* debugging
    char hex_buffer[1024];
    int m = keys_buf_size;
    btox(hex_buffer, my_keys_buffer, m*2);
    printf("Server %d bytes: %s\n", m, hex_buffer);*/
    }
}

// network code to receive values from server and apply
void doReceiveFromServer(struct MultiplayerInfo net_info)
{
    // receive inputs
    char peer_keys_buffer[keys_buf_size];
    for (;;){
    // receive packet
    //server
    int n;
    socklen_t len;
    memset(peer_keys_buffer, 0, keys_buf_size);
    if (app.playerIndex == 0){
        len = sizeof(net_info.cliaddr);
        printf("hello!");
        fflush(stdout);
        n = recvfrom(net_info.sockfd, (char *)peer_keys_buffer, keys_buf_size, 
                    0, ( struct sockaddr *) &net_info.cliaddr,&len);
        peer_keys_buffer[n] = '\0';
        printf("bye!");
    }
    //client
    if (app.playerIndex == 1){
        printf("hello!");
        fflush(stdout);
        n = recvfrom(net_info.sockfd, (char *)peer_keys_buffer, keys_buf_size, 
                    0, (struct sockaddr *) &net_info.servaddr,&len);
        peer_keys_buffer[n] = '\0';
        printf("bye!");
    }

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
    // debuggy
    if (app.playerIndex == 1){
        printf("%i\n", peer_keys.w_key_data);
    }
    }
}
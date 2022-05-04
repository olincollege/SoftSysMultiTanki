#include "server.h"

#define PEER_PORT 8888
#define MY_PORT 7777

char peer_ip_addr[INET_ADDRSTRLEN]; // store peer ipv4
struct KeyInfo my_keys;
struct KeyInfo peer_keys;

int keys_buf_size = 13;
pthread_t sender, receiver;

void *doReceiveFromServer(void *arguments);
void *doSendToServer(void *arguments);

struct args_struct {
  int *network_socket;
  struct sockaddr_in *peer_address;
};

// for debugging
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

// create socket file descriptor, server and client
int setupConnection(struct MultiplayerInfo net_info){
    // create file descriptor
    int network_socket;
    if ((network_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_address, peer_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // localhost
    server_address.sin_port = htons(MY_PORT);

    // bind socket to server address
    if (bind(network_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0 ){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    socklen_t len = sizeof(server_address);
    getsockname(network_socket, (struct sockaddr *)&server_address, &len);

    // connect to peer
    peer_address.sin_family = AF_INET;
    peer_address.sin_addr.s_addr = inet_addr(net_info.peer_ip_addr);
    peer_address.sin_port = htons(PEER_PORT);

    // set up pthread args
    struct args_struct p_thread_args;
    p_thread_args.network_socket = &network_socket;
    p_thread_args.peer_address = &peer_address;
    
    while (1) {
    //pthread create: for both send and receive
    pthread_create(&sender, NULL, doSendToServer, (void *)&p_thread_args);
    pthread_create(&receiver, NULL, doReceiveFromServer, (void *)&p_thread_args);
    
    //join pthread
    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);
    }

    //close socket
    close(network_socket);

    return 0;
}

// network code to send current values to server
void *doSendToServer(void *arguments)
{
    // get inputs
    struct args_struct *args = arguments;
    char my_keys_buffer[keys_buf_size];

    //while (1) {
    //memset(my_keys_buffer, 0, keys_buf_size);
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

    //send packet
    //if (my_keys_buffer[0] == '\0')
    //{
    //  continue;
    //}

    unsigned int peer_struct_length = sizeof(*args->peer_address);
    int res = sendto(*args->network_socket, my_keys_buffer, keys_buf_size, 0, (struct sockaddr *)args->peer_address, peer_struct_length);
    if (res < 0)
    {
      printf("Unable to send message\n");
      exit(-1);
    }

    //memset(my_keys_buffer, 0, keys_buf_size);
    // debugging
    char hex_buffer[1024];
    int m = keys_buf_size;
    btox(hex_buffer, my_keys_buffer, m*2);
    printf("Server %d bytes: %s\n", m, hex_buffer);
    //}
    return(NULL);
}

// network code to receive values from server and apply
void *doReceiveFromServer(void *arguments)
{
    // receive inputs
    struct args_struct *args = arguments;
    //while (1){
        // receive packet
        printf("Hello!");
        char peer_keys_buffer[keys_buf_size];
        unsigned int peer_struct_length = sizeof(*args->peer_address);
        recvfrom(*args->network_socket, peer_keys_buffer, keys_buf_size, 0, (struct sockaddr *)args->peer_address, &peer_struct_length);

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
        char hex_buffer[1024];
        int m = keys_buf_size;
        btox(hex_buffer, peer_keys_buffer, m*2);
        printf("Server %d bytes: %s\n", m, hex_buffer);

        //if (app.playerIndex == 1){
        //    printf("%i\n", peer_keys.w_key_data);
        //}
        // clear
        //memset(peer_keys_buffer, 0, sizeof(peer_keys_buffer));
    //}
    return(NULL);
}

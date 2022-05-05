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

void determine_client_ip();

int main(int argc, char const *argv[]){
    int server_fd;
    struct sockaddr_in address;
    int client_counter = 0;
    int opt = TRUE;

    // create a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    //set socket to allow multiple connections
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )  
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
    if (listen(server_fd, 1024) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    // accept clients

    // pipes for talking between children (clients)
    int fd1[2]; // Used to store two ends of first pipe
    int fd2[2]; // Used to store two ends of second pipe
 
    if (pipe(fd1) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    if (pipe(fd2) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    while (1) {
        struct sockaddr_storage client_addr;
        unsigned int address_size = sizeof(client_addr);
        int connect_d = accept(server_fd, (struct sockaddr *)&client_addr, &address_size);
        char client_ip_addr[INET_ADDRSTRLEN]; // big enough for ipv4

        // fork process for different clients
        client_counter++;
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        // check if child
        if (pid==0) {
            char peer_ip[INET_ADDRSTRLEN];
            // figure out what ip connected
            if ((recv(connect_d, &client_ip_addr, INET_ADDRSTRLEN, 0) < 0)){
                perror("Can't receive from client");
                exit(EXIT_FAILURE);
            }
            client_ip_addr[INET_ADDRSTRLEN] = '\0';

            printf("IP address: %s\n", client_ip_addr); // print out ip address
            fflush(stdout);
            if (client_counter == 1) {
                // close reading of pipe 1
                close (fd1[0]);
                // write ip address to writing end of pipe 1
                write(fd1[1], client_ip_addr, strlen(client_ip_addr) + 1);
                close(fd1[1]);
                // tank no.
                int converted_number = htonl(client_counter-1);
                fflush(stdout);
                send(connect_d,&converted_number,sizeof(converted_number), 0);

                // wait until pipe 2 has information
                while (read(fd2[0],&peer_ip,INET_ADDRSTRLEN) == 0) {
                    // do nothing
                    sleep(0.1);
                }
                // close writing of pipe 2
                close(fd2[1]);
                // close reading of pipe 1
                close(fd2[0]);


            } else if (client_counter == 2) {
                // close reading of pipe 1
                close (fd2[0]);
                // write ip address to writing end of pipe 1
                write(fd2[1], client_ip_addr, strlen(client_ip_addr) + 1);
                close(fd2[1]);
                // tank no.
                int converted_number = htonl(client_counter-1);
                fflush(stdout);
                send(connect_d,&converted_number,sizeof(converted_number), 0);
                // wait until pipe 2 has information
                while (read(fd1[0],&peer_ip,INET_ADDRSTRLEN) == 0) {
                    // do nothing
                    sleep(0.1);
                }
                // close writing of pipe 1
                close(fd1[1]);
                // close reading of pipe 1
                close(fd1[0]);
            }
            // send ip address of client 2
            printf("Peer ip: %s\n", peer_ip);
            fflush(stdout);
            send(connect_d,peer_ip,strlen(peer_ip), 0);
            exit(EXIT_SUCCESS);
        }
        // reset client counter
        if (client_counter == MAX_PLAYERS) {
            client_counter = 0;
        }
        // TODO: alarm which resets client_counter
    }
}
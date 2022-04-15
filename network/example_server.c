#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

//Following code referring to IP Address from geekpage
//Rest of core code from HeadfirstC

int main(int argc, char *argv[]) {
    // replace with getnameinfo(), getaddrinfo()
    int fd;
	struct ifreq ifr;
	
	char iface[] = "wlo1"; // for wifi connected
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	//Get IP4 IP address
	ifr.ifr_addr.sa_family = AF_INET;

	//Copy the interface name
	strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);
	
	//get the ip address
	ioctl(fd, SIOCGIFADDR, &ifr);
	
	//display ip address
	printf("IP address of %s - %s\n" , iface , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr) );
	
	close(fd);
    
    // Main program
    char *advice[] = {
        "Take smaller bites\r\n",
        "Go for the tight jeans. No they do NOT make you look fat.\r\n",
        "One word: inappropriate\r\n",
        "Just for today, be honest. Tell your boss what you *really* think\r\n",
        "You might want to rethink that haircut\r\n"
    };
    int listener_d;
    if ((listener_d = socket(PF_INET, SOCK_STREAM, 0))==-1){
        perror("Can't open socket");
        return -1;
    }
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t)htons(5200);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener_d, (struct sockaddr *) &name, sizeof(name))==-1){
        perror("bind");
        return -1;
    }
    if (listen(listener_d, 10)==-1){
        perror("listen");
        return -1;
    }

    puts("Waiting for connection");
    while(1){
        struct sockaddr_storage client_addr;
        unsigned int address_size = sizeof(client_addr);
        int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
        char *msg = advice[rand() % 5];
        if (send(connect_d, msg, strlen(msg), 0)==-1){
            perror("send");
            return -1;
        }
        close(connect_d);
    }
    return 0;
}
#include <stdio.h>

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 2048

int main(int argc, char *argv[]) {
    int udp_socket, status;

    // Check commandline for correct usage
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>", argv[0]);
        return -1;
    }

    // Create a socket for IPv4 UDP
    if(((udp_socket = socket(AF_INET, SOCK_DGRAM, 0))) == -1) {
        perror("socket()");
        return -1;
    }

    // Set up the server address structure
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) == -1) {
        perror("inet_pton()");
        return -1;
    }

    // Connect to the server
    if((status = connect(udp_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
        perror("connect()");
        return -1;
    }

    printf("Connected to server %s:%s\n", argv[1], argv[2]);

    while(1) {
        char buffer[BUFSIZE];

        if(fgets(buffer, BUFSIZE, stdin) == NULL) {
            break;
        }

        if(send(udp_socket, buffer, strlen(buffer), 0) == -1) {
            perror("send()");
            return -1;
        }
    }
}
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#include "udp-chat.h"

#define BUFSIZE 2048

int main(int argc, char *argv[]) {
    int udp_socket;
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
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    // Convert IPv4 address from text to binary form
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) != 1) {
        fprintf(stderr, "Invalid IPv4 address\n");
        close(udp_socket);
        return 1;
    }

    printf("Server address: %s:%s\n", argv[1], argv[2]);

    // Send and recive messages/data from server
    int result = udp_chat(udp_socket, &serv_addr, 1);
    if(result == -1) {
        perror("udp_chat()");
    }

    close(udp_socket);
    return result == -1 ? 1 : 0;
}
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
    int client_fd, status;
    struct sockaddr_in serv_addr;

    // Check commandline for correct usage
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>", argv[0]);
        return -1;
    }

    // Create a socket for IPv4 UDP
    if(((client_fd = socket(AF_INET, SOCK_DGRAM, 0))) == -1) {
        perror("Socket");
        return -1;
    }

    // Set up the server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) == -1) {
        perror("inet_pton failed");
        return -1;
    }

    // Connect to the server
    if((status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
        perror("connect failed");
        return -1;
    }

    printf("Connected to server %s:%s\n", argv[1], argv[2]);
    
    while(1) {
        char buffer[BUFSIZE];
        if(fgets(buffer, BUFSIZE, stdin) == NULL) {
            fprintf(stderr, "failed to get input from user");
            return -1;
        }

        if(sendto(client_fd, buffer, strlen(buffer), 0, (struct sockaddr*)NULL, sizeof(serv_addr)) == -1 ) {
            perror("failed to send");
            return -1;
        }

        ssize_t n = recvfrom(client_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
        if(n < 0) {
            perror("recvfrom");
            return -1;
        }
        
        buffer[n] = '\0';
        puts(buffer);
    }
}
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

//#include "udp-chat.h"

#define BUFSIZE 2048

static void perror_exit(char msg[]);

int main(int argc, char* argv[]){

	// Port for the server to listen on
	int listen_port;
	if(argc != 2 || sscanf(argv[1], "%d", &listen_port) != 1){
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		return 1;
	}

	// Create a socket for IPv4 UDP
	int server_socket = socket(AF_INET,	SOCK_DGRAM, 0);
	if(server_socket == -1){
		perror_exit("socket()");
	}

	/* Optional step, allow address reuse
	   (needed if you want to use the same port after an unclean shutdown of your program).
	 */
	int reuseaddr = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1) {
		    perror_exit("setsockopt(reuseaddr)");
	}

	// Bind socket to port 'listen_port'
	struct sockaddr_in addr, client_addr = {0}; //init fields to zero
	addr.sin_family = AF_INET; //the address is ipv4
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //use any address, eg. 0.0.0.0 for ipv4
	addr.sin_port = htons(listen_port); //convert the port number to network-byte-order

	// Bind server address to socket descriptor
	if(bind(server_socket, (struct sockaddr*) &addr, sizeof(addr)) == -1){
		perror_exit("bind()");
	}

	while(1){
		// Get address from client
		char buffer[BUFSIZE];
		socklen_t client_len = sizeof(client_addr);

		ssize_t client_data = recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_len);

		// Check for errors
		if(client_data == -1) {
			perror("recvfrom()");
			return 1;
		}

		// Null-terminate received data
		buffer[client_data] = '\0';
		fputs(buffer, stdout);
	}

	// Send and recive messages/data from server
    //if(result = udp_chat(client_addr.sin_addr.s_addr, client_addr.sin_port) == -1) {
    //    perror("udp_chat()");
    //    close(server_socket);
    //    return 1;
    //}
	
	// Clean-up and exit
	close(server_socket);
	return 0;
	//return result == -1 ? 1 : 0;
}

/* Convenience function for error handling */
static void perror_exit(char msg[]) {
	perror(msg);
	exit(1);
}
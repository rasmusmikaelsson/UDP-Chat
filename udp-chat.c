#include "udp-chat.h"

#include <errno.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFSIZE 2048

int udp_chat(int socket_fd, struct sockaddr_in *peer_addr, int peer_known) {
    int stdin_open = 1;

    while(1) {
        fd_set read_fds;
        int max_fd = socket_fd;
        int ready;

        // Prepare file descriptors
        FD_ZERO(&read_fds);
        FD_SET(socket_fd, &read_fds);
        if(stdin_open) {
            FD_SET(STDIN_FILENO, &read_fds);
            if(STDIN_FILENO > max_fd) {
                max_fd = STDIN_FILENO;
            }
        }
        
        // Wait for activity
        ready = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if(ready == -1) {
            if(errno == EINTR) {
                continue;
            }
            return -1;
        }

        // Handle terminal input
        if(stdin_open && FD_ISSET(STDIN_FILENO, &read_fds)) {
            char buffer[BUFSIZE];
            
            // Read stdin
            ssize_t length = read(STDIN_FILENO, buffer, sizeof(buffer));
            if(length == -1) {
                if(errno == EINTR) {
                    continue;
                }
                return -1;
            }

            // EOF, stop monitoring stdin but continue receiving UDP datagrams.
            if(length == 0) {
                stdin_open = 0;
            } else if(peer_known) {
                if(sendto(socket_fd, buffer, (size_t)length, 0,
                        (struct sockaddr *)peer_addr,
                        sizeof(*peer_addr)) == -1) {
                    return -1;
                }
            } else {
                fprintf(stderr, "No peer address known yet\n");
            }
        }

        // Handle incoming UDP datagrams
        if(FD_ISSET(socket_fd, &read_fds)) {
            char buffer[BUFSIZE];
            socklen_t client_len = sizeof(*peer_addr);

            // Receive data and update the peer address
            ssize_t client_data = recvfrom(
                socket_fd,
                buffer,
                sizeof(buffer) - 1,
                0,
                (struct sockaddr *)peer_addr,
                &client_len
            );

            if(client_data == -1) {
                if(errno == EINTR) {
                    continue;
                }
                return -1;
            }

            // Remember that we now know the peer's address
            peer_known = 1;

            // Null-terminate and print received data
            buffer[client_data] = '\0';
            fputs(buffer, stdout);
            fflush(stdout);
        }
    }
}
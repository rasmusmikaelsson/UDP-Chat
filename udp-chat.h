#ifndef UDP_CHAT_H
#define UDP_CHAT_H

#include <netinet/in.h>

int udp_chat(int socket_fd, struct sockaddr_in *peer_addr, int peer_known);

#endif

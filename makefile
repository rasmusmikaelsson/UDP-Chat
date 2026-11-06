CC      = gcc
CFLAGS  = -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic \
          -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

OBJ 	= udp-server.o udp-client.o

.PHONY: all clean

all: udp-server udp-client

udp-server: udp-server.o
	$(CC) -o udp-server udp-server.o

udp-client: udp-client.o
	$(CC) -o udp-client udp-client.o

udp-server.o: udp-server.c
	$(CC) $(CFLAGS) -c udp-server.c

udp-client.o: udp-client.c
	$(CC) $(CFLAGS) -c udp-client.c

clean: 
	rm -f udp-server udp-client $(OBJ)
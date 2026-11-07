CC      = gcc
CFLAGS  = -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic \
          -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

OBJ = udp-server.o udp-client.o udp-chat.o

.PHONY: all clean

all: udp-server udp-client

udp-server: udp-server.o udp-chat.o
	$(CC) -o udp-server udp-server.o udp-chat.o

udp-client: udp-client.o udp-chat.o
	$(CC) -o udp-client udp-client.o udp-chat.o

udp-server.o: udp-server.c udp-chat.h
	$(CC) $(CFLAGS) -c udp-server.c

udp-client.o: udp-client.c udp-chat.h
	$(CC) $(CFLAGS) -c udp-client.c

udp-chat.o: udp-chat.c udp-chat.h
	$(CC) $(CFLAGS) -c udp-chat.c

clean:
	rm -f udp-server udp-client $(OBJ)
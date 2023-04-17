#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

#ifndef SOCKET_H
#define SOCKET_H

typedef int socket_t;
int start_sockets(char* port);

void writes(socket_t socket, char* data);
char* readsn(socket_t socket, int n);
char* reads(socket_t socket);
void closes(socket_t socket);

void socket_listener(socket_t socket);

#endif
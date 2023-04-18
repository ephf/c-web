#ifndef SOCKET_H
#define SOCKET_H

#include <pthread.h>
static pthread_t s_tid;

// a socket from a client (client server connection) (file descriptor)
typedef int socket_t;
// start the socket server at a certain port
int start_sockets(char* port);

// write to a socket
void writes(socket_t socket, char* data);
/* read n bytes from a socket
returns a malloc (make sure to free data after use)*/
char* readsn(socket_t socket, int n);
/* read from a socket (default 65535 bytes) (see readsn)
returns a malloc (make sure to free data after use)*/
char* reads(socket_t socket);
// close a socket connection
void closes(socket_t socket);

// funtion for listening for new socket connections (threaded)
void socket_listener(socket_t socket);

#endif
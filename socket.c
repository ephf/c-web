#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

void* handle_connection(void* connection);

int start_sockets(char* port) {
    struct addrinfo *response, *res_link, hints = { AI_PASSIVE, AF_INET, SOCK_STREAM };
    if(getaddrinfo(NULL, port, &hints, &response) != 0) return 1;

    int listenfd;
    for(res_link = response; res_link != NULL; res_link = res_link->ai_next) {
        listenfd = socket(res_link->ai_family, res_link->ai_socktype, 0);
        int opt = 1;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        if(listenfd == -1) continue;
        if(bind(listenfd, res_link->ai_addr, res_link->ai_addrlen) == 0) break;
    }
    if(res_link == NULL) return 2;

    freeaddrinfo(response);

    if(listen(listenfd, 1000000) != 0) return 3;
    signal(SIGCHLD, SIG_IGN);

    printf("socket server listening on port %s\n", port);

    while(1) {
        socklen_t addrlen = sizeof(struct sockaddr_in);
        long connection = accept(listenfd, NULL, &addrlen);
        if(connection < 0) continue;
        pthread_create(&s_tid, NULL, handle_connection, (void*) connection);
    }
}

void* handle_connection(void* connection) {
    socket_listener((long) connection);
    return NULL;
}

void writes(socket_t socket, char* data) {
    write(socket, data, strlen(data));
}

char* readsn(socket_t socket, int n) {
    char* buffer = malloc(n);
    int recieved = read(socket, buffer, n);

    if(recieved <= 0) return NULL;

    buffer[recieved] = '\0';
    return buffer;
}

char* reads(socket_t socket) {
    return readsn(socket, 65535);
}

void closes(socket_t socket) {
    shutdown(socket, SHUT_RDWR);
    close(socket);
}
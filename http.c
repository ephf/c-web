#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* sparse(char** str, char* token) {
    char* ptr = *str;
    int tl = strlen(token);
    for(int i = 0; (*str)[i] != '\0'; i++) {
        if(strncmp((*str) + i, token, tl) == 0) {
            memset((*str) + i, 0, tl);
            *str += i + tl;
            return ptr;
        }
    }
    return NULL;
}

void socket_listener(socket_t socket) {
    char* data = reads(socket);
    if(data == NULL) closes(socket);

    request_t req = { data, sparse(&data, " "), sparse(&data, " "), sparse(&data, "\r\n") };
    req.socket = socket;

    char* name;
    while((name = sparse(&data, ": ")) != NULL) {
        req.headers[req.total_headers++] = (header_t) { name, sparse(&data, "\r\n") };
    }

    sparse(&data, "\r\n");
    req.body = data;

    request_listener(req);
}

char* get_header(request_t req, char* name) {
    for(int i = 0; i < req.total_headers; i++) {
        if(strcmp(req.headers[i].name, name) == 0) return req.headers[i].value;
    }
    return NULL;
}

void set_header(request_t* req, char* name, char* value) {
    req->res_headers[req->total_res_headers++] = (header_t) { name, value };
}

void writer(request_t req, char* data) {
    writes(req.socket, data);
}

void writer_head(request_t req, char* status) {
    char head[13] = "HTTP/1.1 ";
    writer(req, strcat(head, status));

    for(int i = 0; i < req.total_res_headers; i++) {
        header_t header = req.res_headers[i];
        char header_str[4 + strlen(header.name) + strlen(header.value)];
        sprintf(header_str, "\r\n%s: %s", header.name, header.value);
        writer(req, header_str);
    }

    writer(req, "\r\n\r\n");
}

void endr(request_t req) {
    closes(req.socket);
    free(req.data);
}

char* readrn(request_t req, int n) {
    return readsn(req.socket, n);
}

char* readr(request_t req) {
    return reads(req.socket);
}

void rnprintf(request_t req, int n, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char data[n];
    vsprintf(data, fmt, args);
    va_end(args);
    writer(req, data);
}

void rprintf(request_t req, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char data[65535];
    vsprintf(data, fmt, args);
    va_end(args);
    writer(req, data);
}
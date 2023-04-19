#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char* _sparse(char** str, char* token) {
    char* ptr = *str;
    int tl = strlen(token);
    for(int i = 0; ptr[i] != '\0'; i++) {
        if(strncmp(ptr + i, token, tl) == 0) {
            memset(ptr + i, 0, tl);
            *str += i + tl;
            return ptr;
        }
    }
    return NULL;
}

void socket_listener(socket_t socket) {
    char* data = reads(socket);
    char* initial_data_ptr = data;
    if(data == NULL) closes(socket);

    request_t req = { _sparse(&data, " "), _sparse(&data, " "), _sparse(&data, "\r\n"), socket };
    req.res_headers = malloc(sizeof(header_list_t));
    req._last_res_header = req.res_headers;

    header_list_t* header_linked_list = malloc(sizeof(header_list_t));
    header_list_t* header_link = header_linked_list;

    char* name;
    while((name = _sparse(&data, ": ")) != NULL) {
        req.total_headers++;
        header_link->header.name = name;
        header_link->header.value = _sparse(&data, "\r\n");
        header_list_t* temp = header_link;
        temp->next = (header_link = malloc(sizeof(header_list_t)));
    }

    header_link = header_linked_list;

    header_t headers[req.total_headers];
    for(int i = 0; i < req.total_headers; i++) {
        headers[i] = header_link->header;
        header_list_t* temp = header_link;
        header_link = header_link->next;
        free(temp);
    }
    free(header_link);
    req.headers = headers;

    _sparse(&data, "\r\n");
    req.body = data;

    request_listener(req);
    free(initial_data_ptr);
}

char* get_header(request_t req, char* name) {
    for(int i = 0; i < req.total_headers; i++) {
        if(strcmp(req.headers[i].name, name) == 0) return req.headers[i].value;
    }
    return NULL;
}

void set_header(request_t* req, char* name, char* value) {
    req->total_res_headers++;
    req->_last_res_header->header.name = name;
    req->_last_res_header->header.value = value;
    header_list_t* temp = req->_last_res_header;
    temp->next = (req->_last_res_header = malloc(sizeof(header_list_t)));
}

void writer(request_t req, char* data) {
    writes(req.socket, data);
}

void writer_head(request_t req, char* status) {
    char head[13] = "HTTP/1.1 ";
    writer(req, strcat(head, status));

    while(req.res_headers != req._last_res_header) {
        header_t header = req.res_headers->header;
        rprintf(req, "\r\n%s: %s", header.name, header.value);
        header_list_t* temp = req.res_headers;
        req.res_headers = temp->next;
        free(temp);
    }
    free(req.res_headers);

    writer(req, "\r\n\r\n");
}

void endr(request_t req) {
    closes(req.socket);
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

#include "socket.h"
#include <stdarg.h>

#ifndef HTTP_H
#define HTTP_H

#define HEADMAX 40

typedef struct {
    char* name;
    char* value;
} header_t;

typedef struct {
    char* data;

    char* method;
    char* url;
    char* httpv;

    socket_t socket;

    header_t headers[HEADMAX];
    int total_headers;
    char* body;

    header_t res_headers[HEADMAX];
    int total_res_headers;
} request_t;

void request_listener(request_t req);

char* get_header(request_t req, char* name);
void set_header(request_t* req, char* name, char* value);
void writer(request_t req, char* data);
void writer_head(request_t req, char* status);
void endr(request_t req);
char* readrn(request_t req, int n);
char* readr(request_t req);

void rnprintf(request_t req, int n, char* fmt, ...);
void rprintf(request_t req, char* fmt, ...);

#endif
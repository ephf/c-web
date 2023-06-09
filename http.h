#include "socket.h"

#ifndef HTTP_H
#define HTTP_H

typedef struct {
    char* name;
    char* value;
} header_t;

struct HEADER_LIST;
typedef struct HEADER_LIST {
    header_t header;
    struct HEADER_LIST* next;
} header_list_t;

// Incomming http request type
typedef struct {
    char* method;
    char* url;
    // the request's http version (eg. HTTP/1.1)
    char* httpv;

    // the request's socket (see socket_t)
    socket_t socket;

    // a list of incomming headers
    header_t* headers;
    // the total amount of incomming headers
    int total_headers;

    char* body;

    // a list of outgoing headers (response headers) (linked list)
    header_list_t* res_headers;
    header_list_t* _last_res_header;
    // the total amount of outgoing headers
    int total_res_headers;
} request_t;

// function for listening to incomming http requests
void request_listener(request_t req);

// get an incomming header from a request
char* get_header(request_t req, char* name);
// set an outgoing header from a request
void set_header(request_t* req, char* name, char* value);
// write to a request's socket (response body)
void writer(request_t req, char* data);
/* write a response header to a request's socket (response)
adds all set headers in the request so new headers will not be added*/
void writer_head(request_t req, char* status);
// end a request's response
void endr(request_t req);
// read n bytes of data from a request
char* readrn(request_t req, int n);
// read data from a request (reads a default of 65535 bytes) (see readrn)
char* readr(request_t req);

// printf n bytes into a request's socket (response body)
void rnprintf(request_t req, int n, char* fmt, ...);
// printf into a request's socket (response body) (see rnprintf)
void rprintf(request_t req, char* fmt, ...);

#endif
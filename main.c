#include "http.h"

void request_listener(request_t req) {
    set_header(&req, "Content-Type", "text/html");
    writer_head(req, "200");
    
    rprintf(req, "<h1>Hello from host `%s`</h1>", get_header(req, "Host"));
    endr(req);
}

int main() {
    return start_sockets("80");
}
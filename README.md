# c web

```c
#include "http.h"

// listen to the http server
void request_listener(request_t req /* <--- request object */) {
    set_header(&req, "Content-Type", "text/html"); // <--- sets the Content-Type header
    writer_head(req, "200"); // <--- sends the headers and a status code (200)
    
    rprintf(req, "<h1>Hello from host `%s`</h1>", get_header(req, "Host")); // <--- sends html
    endr(req); // <--- closes the connection and frees the initial request buffer
}

int main() {
    // starts the socket server on port 80
    return start_sockets("80");
}
```
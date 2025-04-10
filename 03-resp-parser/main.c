#include "server.h" // Include the server header file
#include "resp_parser.h" // Include the RESP parser header file
int main() {
    start_server(6379); // Start the server on port 6379
    return 0; //success
}
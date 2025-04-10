#include <stdio.h> // Include standard I/O library// Include standard I/O library
#include <stdlib.h> // Include standard library for functions like malloc
#include <string.h> // Include string library for string manipulation functions
#include <unistd.h> // Include POSIX standard symbolic constants and types
#include <netinet/in.h> // Include definitions for internet operations
#include "resp_parser.h" // Include the RESP parser header file
#include "server.h" // Include the server header file


#define BUFFER_SIZE 1024 // Define a buffer size for reading data

const char* handle_command(int argc, char* argv[]){
    if (argc ==0) return "-ERR empty command\r\n"; // Check if the command is empty
    if (strcmp(argv[0], "PING") == 0){
        return "+PONG\r\n"; // Check for "PING" command
    }else if(strcmp(argv[0], "HELLO") == 0 && argc >= 2){
        static char response[BUFFER_SIZE]; // Static buffer for response
        snprintf(response , sizeof(response), "+%s\r\n", argv[1]); // Format the response
        return response; // Return the formatted response
    }else{
        return "-ERR unknown command\r\n"; // Return error for unknown command
    }
}

void start_server(int port){
    int server_fd, client_fd;
    struct sockaddr_in addr; // Define a structure for the server address
    char buffer[BUFFER_SIZE]; // Buffer for reading data
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
    if (server_fd == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET; // Set the address family to IPv4
    addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any address
    addr.sin_port = htons(port); // Convert port number to network byte order

    // Bind the socket to the address and port
    if(bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // Set the socket to listen for incoming connections, up to 3 queued connections
    listen(server_fd, 3); // Listen for incoming connections
    printf("Server started on port %d\n", port);
    while(1){
        //Wait until a client connects (blocking call)
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0){
            perror("accept failed");
            continue; // Continue to the next iteration if accept fails
        }

        char* argv[8];
        int valread = read(client_fd, buffer, BUFFER_SIZE);
        buffer[valread] = '\0';
        printf("Raw input:\n%s", buffer);
        
        int argc = parse_resp(buffer, argv, 8);
        const char* response = handle_command(argc, argv);
        send(client_fd, response, strlen(response), 0);
        //close the client socket
        close(client_fd);
        printf("Client disconnected\n");
    }
}
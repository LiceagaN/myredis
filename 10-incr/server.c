#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "../03-resp-parser/resp_parser.h"
#include "../04-db/db.h"
#include "server.h"

#define BUFFER_SIZE 1024

HashMap db; // Redis memory database

const char* handle_command(int argc, char* argv[]) {
    if (argc == 0) return "-ERR empty command\r\n";

    // 🧪 Depuración
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: '%s'\n", i, argv[i]);
    }

    if (strcmp(argv[0], "PING") == 0) {
        return "+PONG\r\n";

    } else if (strcmp(argv[0], "HELLO") == 0 && argc >= 2) {
        static char response[BUFFER_SIZE];
        snprintf(response, sizeof(response), "+%s\r\n", argv[1]);
        return response;

    } else if (strcmp(argv[0], "SET") == 0 && argc == 3) {
        hashmap_set(&db, argv[1], argv[2]);
        return "+OK\r\n";

    } else if (strcmp(argv[0], "GET") == 0 && argc == 2) {
        const char* value = hashmap_get(&db, argv[1]);
        if (value) {
            static char response[BUFFER_SIZE];
            snprintf(response, sizeof(response), "$%lu\r\n%s\r\n", strlen(value), value);
            return response;
        } else {
            return "$-1\r\n";
        }
    } else if(strcmp(argv[0],"DEL") == 0 && argc == 2){
        int deleted = hashmap_del(&db, argv[1]);
        static char response[BUFFER_SIZE];
        snprintf(response, sizeof(response),":%d\r\n", deleted);
        return response;
    } else if(strcmp(argv[0], "EXISTS") == 0 && argc == 2){
        int exists = hashmap_exists(&db, argv[1]);
        static char response[BUFFER_SIZE];
        snprintf(response, sizeof(response), ":%d\r\n", exists);
        return response;
    } else if(strcmp(argv[0], "EXPIRE") == 0 && argc == 3){
        int seconds = atoi(argv[2]);
        int result = hashmap_expire(&db, argv[1], seconds);
        static char response[BUFFER_SIZE];
        snprintf(response, sizeof(response), ":%d\r\n", result);
        return response;
    } else if(strcmp(argv[0], "TTL") == 0 && argc == 2){
        int ttl = hashmap_ttl(&db, argv[1]);
        static char response[BUFFER_SIZE];
        snprintf(response, sizeof(response), ":%d\r\n", ttl);
        return response;
    }else if(strcmp(argv[0], "SAVE") == 0 && argc == 1){
        int ok = hashmap_save(&db, "dump.rdb");
        return ok ? "+OK\r\n" : "-ERR failed to save\r\n";
    }else if(strcmp(argv[0], "LOAD") == 0 && argc == 1){
        int ok = hashmap_load(&db, "dump.rdb");
        return ok ? "+OK\r\n" : "-ERR failed to load\r\n";
    } else if(strcmp(argv[0], "INCR") == 0 && argc == 2){
        const char* current = hashmap_get(&db, argv[1]);
        int value = 0;

        if(current != NULL){
            char* endptr;
            value = strtol(current, &endptr, 10);
            if(*endptr != '\0'){
                return "-ERR value is not an integer\r\n";
            }
        }
        value++;
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d", value);
        hashmap_set(&db, argv[1], buffer);

        static char response[BUFFER_SIZE]; 
        snprintf(response, sizeof(response), ":%d\r\n", value);
        return response;
    }else {
        return "-ERR unknown command\r\n";
    }
    return "-ERR unknown command\r\n";
}

void start_server(int port) {
    hashmap_init(&db);

    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 3);
    printf("Server started on port %d\n", port);

    while (1) {
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }
        char* argv[8];
        int valread = read(client_fd, buffer, BUFFER_SIZE);
        buffer[valread] = '\0';
        printf("Raw input:\n%s", buffer);

        int argc = parse_resp(buffer, argv, 8);
        const char* response = handle_command(argc, argv);
        send(client_fd, response, strlen(response), 0);
        close(client_fd);
        printf("Client disconnected\n");
        // pid_t pid = fork(); //Use to create a new process

        // if (pid == 0){// Child process
        //     char* argv[8];
        //     int valread = read(client_fd, buffer, BUFFER_SIZE);
        //     buffer[valread] = '\0';
        //     printf("Raw input:\n%s", buffer);
    
        //     int argc = parse_resp(buffer, argv, 8);
        //     const char* response = handle_command(argc, argv);
        //     send(client_fd, response, strlen(response), 0);
        //     close(client_fd);
        //     printf("Client disconnected\n");
        //     exit(0); // Child process exits after handling the client
        // }else if(pid > 0){// Parent process
        //     close(client_fd); // Parent process closes the client socket
        //     printf("Client disconnected\n");
        // }else{// Fork failed
        //    perror("fork failed");
        //}
    }
}

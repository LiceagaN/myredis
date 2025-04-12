#include <string.h> // Include string library for string manipulation functions
#include <stdio.h> // Include standard I/O library
#include <stdlib.h> // Include standard library for functions like malloc

int parse_resp(char* input, char* argv[], int max_args){
    if (input[0] != '*') return -1;
    int argc = atoi(&input[1]);
    if (argc > max_args) return -1;

    int i = 0;
    char* ptr = strchr(input, '\n');
    if (!ptr) return -1;
    ptr++;

    while (i < argc){
        if(ptr[0] != '$') return -1;
        int len = atoi(ptr + 1);

        ptr = strchr(ptr, '\n');
        if (!ptr) return -1;
        ptr++;

        argv[i] = ptr;

        // Corregimos el \r\n al final del string RESP
        if (ptr[len - 1] == '\r') {
            ptr[len - 1] = '\0';
        } else {
            ptr[len] = '\0';
        }

        printf("Argumento %d: %s\n", i, argv[i]);

        ptr += len + 2;  // skip value + \r\n
        i++;
    }

    return argc;
}
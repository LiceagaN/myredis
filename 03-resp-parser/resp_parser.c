#include <string.h> // Include string library for string manipulation functions
#include <stdio.h> // Include standard I/O library
#include <stdlib.h> // Include standard library for functions like malloc

int parse_resp(char* input, char* argv[], int max_args){
    if (input[0] != '*') return -1; // Check if the first character is '*'
    int argc = atoi(&input[1]); // Convert the string to an integer
    if (argc > max_args) return -1; // Check if the number of arguments exceeds the maximum

    int i = 0;
    char* ptr = strchr(input, '\n'); // Find the first newline character

    if (!ptr) return -1; // Check if newline character is found
    ptr ++; // Move the pointer to the next character

    while (i < argc){
        printf("Argumento %d: %s\n", i, argv[i]);
        if(ptr[0] != '$') return -1; // Check if the first character is '$'
        int len = atoi(ptr + 1); // Convert the string to an integer
        ptr = strchr(ptr, '\n'); // Find the next newline character

        if (!ptr) return -1; // Check if newline character is found
        ptr ++; // Move the pointer to the next character

        argv[i] = ptr; // Store the pointer to the argument
        argv[i][len] = '\0'; // Null-terminate the string

        ptr += len + 2; // Move the pointer to the next argument
        i ++; // Increment the argument count 
    }

    return argc; // Return the number of arguments
}
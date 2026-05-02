#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "utils.h"
#include "priority.h"


char* get_string_input(const char* prompt) {
    printf("%s", prompt);
    char buffer[1024];
    fgets(buffer, sizeof(buffer), stdin);

    // Remove newline
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    // Allocate memory
    char* input = (char*)malloc(strlen(buffer) + 1);
    if (!input) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(input, buffer);
    return input;
}

uint16_t get_integer_input(const char* prompt) {
    printf("%s", prompt);
    int value;
    scanf("%hu", &value);
    while (getchar() != '\n'); // Clear input buffer
    return value;
}

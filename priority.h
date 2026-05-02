#ifndef PRIORITY_H
#define PRIORITY_H


typedef enum {
    PRIORITY_LOW = 1,
    PRIORITY_MEDIUM = 2,
    PRIORITY_HIGH = 3
} Priority;


const char* priority_to_string(Priority p);

#endif


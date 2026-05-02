#include "priority.h"

const char* priority_to_string(Priority p) {
    switch (p) {
        case PRIORITY_LOW:
             return "Low";
        case PRIORITY_MEDIUM:
             return "Medium";
        case PRIORITY_HIGH:
             return "High";
        default:
             return "Unknown";
    }
}


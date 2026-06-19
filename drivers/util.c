#include "util.h"

unsigned int strlen(char* str) {
    unsigned int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}
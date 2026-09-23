#include "abstract.h"

abstract_t strcmp(const abssmall_t* s1, const abssmall_t* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

abstract_t strncmp(const abssmall_t* s1, const abssmall_t* s2, abstract_t n) {
    if (n == 0) {
        return 0;
    }

    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }

    if (n == 0) {
        return 0;
    }

    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

abstract_t strlen(const char* s) {
    abstract_t len = 0;
    while (s[len]) len++;
    return len;
}
#include "abstract.h"

void* memset(void* dest, int value, unsigned int count)
{
    unsigned char* ptr = (unsigned char*)dest;
    while (count--)
        *ptr++ = (unsigned char)value;

    return dest;
}

void * memcpy(void *dest, const void *src, unsigned int n) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;
    for (unsigned int i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}
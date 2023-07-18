#include "stdstr.h"
#include <stddef.h>

char* strcat(char* dest, const char* src) 
{
    // Find end of dest string
    char* d = dest;
    while(*d != '\0') {
        d++;
    }

    // Append src to end of dest
    while(*src != '\0') {
        *d++ = *src++;
    }

    // Add terminating null byte
    *d = '\0';

    return dest;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void itoa(char *buf, unsigned long int n, int base)
{
    unsigned long int tmp;
    int i, j;

    tmp = n;
    i = 0;

    do
    {
        tmp = n % base;
        buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (n /= base);
    buf[i--] = 0;

    for (j = 0; j < i; j++, i--)
    {
        tmp = buf[j];
        buf[j] = buf[i];
        buf[i] = tmp;
    }
}
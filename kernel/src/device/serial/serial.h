#include <stdbool.h>

#define SERIAL_PORT 0x3f8

int init_serial();
bool serial_is_ready(void);
void serial_putc(char c);
void serial_puts(const char *str);
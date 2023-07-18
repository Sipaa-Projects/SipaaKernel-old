#include <architecture/ioport.h>
#include <device/serial/serial.h>
#include <stdbool.h>
#include <stddef.h>

int init_serial()
{
    serial_puts("init_serial() Setting up serial port...");
    outportb(SERIAL_PORT + 1, 0x00);
    outportb(SERIAL_PORT + 3, 0x80);
    outportb(SERIAL_PORT + 0, 0x01);
    outportb(SERIAL_PORT + 1, 0x00);
    outportb(SERIAL_PORT + 3, 0x03);
    outportb(SERIAL_PORT + 2, 0xC7);
    outportb(SERIAL_PORT + 4, 0x0B);
    serial_puts(" OK\n");

    return 0;
}

bool serial_is_ready(void)
{
    return inportb(SERIAL_PORT + 5) & 0x20;
}

void serial_putc(char c)
{
    while (!serial_is_ready())
        ;
    outportb(SERIAL_PORT, c);
}

void serial_puts(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        serial_putc(str[i]);
    }
}
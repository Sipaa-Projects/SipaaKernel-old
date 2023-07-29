#include "kb.h"
#include <architecture/ioport.h>
#include <architecture/x86_32/isr.h>
#include <architecture/x86_32/idt.h>
#include <architecture/x86_32/8259_pic.h>
#include <device/serial/serial.h>
#include <stdint.h>

const char keyboard_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
  	'9', '0', '-', '=', '\b',
  	'\t',
  	'q', 'w', 'e', 'r',
  	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
  	  0,
  	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
 '	\'', '`',   0,
 '	\\', 'z', 'x', 'c', 'v', 'b', 'n',
  	'm', ',', '.', '/',   0,
  	'*',
  	  0,
  	' ',
  	  0,
  	  0,
  	  0,   0,   0,   0,   0,   0,   0,   0,
  	  0,
  	  0,
  	  0,
  	  0,
  	  0,
  	  0,
  	'-',
  	  0,
  	  0,
  	  0,
  	'+',
  	  0,
  	  0,
  	  0,
  	  0,
  	  0,
  	  0,   0,   0,
  	  0,
  	  0,
  	  0,
};

void handler(REGISTERS *r)
{
    uint8_t scancode = inportb(0x60);
    
    if (scancode & 0x80)
    {
        pic8259_eoi(IRQ_BASE + 1);
        return;
    }

    if (scancode == 0x0E)
    {
        serial_puts("*backspace*");
    }
    else if (scancode == 0x1C)
    {
        serial_puts("\n");
    }
    else
    {
        char ascii = keyboard_map[scancode];
        serial_putc(ascii);
    }

    pic8259_eoi(IRQ_BASE + 1);
}

int init_kb()
{
    isr_register_interrupt_handler(IRQ_BASE + 1, handler);
    return 0;
}

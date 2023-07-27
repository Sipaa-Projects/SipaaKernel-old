#include "syscall.h"
#include "architecture/x86_32/isr.h"
#include "architecture/x86_32/idt.h"
#include "device/serial/serial.h"
#include "stdlib/stdstr.h"

void syscall_handler(REGISTERS *r)
{
    char* eaxChar;
    itoa(eaxChar, r->eax, 10);
    serial_puts(strcat("EAX : ", eaxChar));
}

int init_syscall()
{
    isr_register_interrupt_handler(0x80, syscall_handler);
    return 0;
}
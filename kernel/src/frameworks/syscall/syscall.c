#include "syscall.h"
#include "architecture/x86_32/isr.h"
#include "architecture/x86_32/idt.h"
#include "architecture/x86_32/8259_pic.h"
#include "device/serial/serial.h"
#include "stdlib/stdstr.h"
#include <stdint.h>

SYSCALL_F syscalls[SYSCALL_TABLE_LENGTH];

void syscall_handler(REGISTERS *r)
{
    char* eaxChar;
    itoa(eaxChar, r->eax, 10);
    //serial_puts(strcat("EAX : ", eaxChar));

    SYSCALL_F callfunc = syscalls[r->eax];
    if (callfunc == (SYSCALL_F)0)
    {
        serial_puts("syscall_handler() An invalid syscall has been called. (EAX : ");
        serial_puts(eaxChar);
        serial_puts(")\n");
        return;
    }
    callfunc(r);
}

void register_syscall(int id, SYSCALL_F handler)
{
    if (syscalls[id] != (SYSCALL_F)0)
    {
        return;
    }
    syscalls[id] = handler;
}

int init_syscall()
{
    for (size_t i = 0; i < SYSCALL_TABLE_LENGTH; i++)
    {
        syscalls[i] = (SYSCALL_F)0;
    }

    isr_register_interrupt_handler(0x80, syscall_handler);
    return 0;
}
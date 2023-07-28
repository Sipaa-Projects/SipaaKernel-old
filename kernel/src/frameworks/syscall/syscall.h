#include <stdint.h>
#include "architecture/x86_32/isr.h"
#include "architecture/x86_32/idt.h"

#define SYSCALL_TABLE_LENGTH 32767

typedef void (*SYSCALL_F)(REGISTERS *);

void register_syscall(int id, SYSCALL_F handler);
int init_syscall();
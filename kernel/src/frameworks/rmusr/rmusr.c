#include "rmusr.h"

extern void switch_to_user();

void init_usr(void (*function)())
{
    switch_to_user();
}

void usr_main()
{
    int syscallNumber = 1;
    char* text = "Welcome to userspace!\nTrying a ring 0 instruction...\n";

    __asm__ volatile(
        "mov %1, %%ebx\n"   // Move the address of 'text' into EBX
        "int $0x80\n"       // Invoke the syscall with int 0x80
        :
        : "r" (syscallNumber), "r" (text)
        : "%ebx"    // Clobbered registers
    );

    while (1)
    {
      
    }
}
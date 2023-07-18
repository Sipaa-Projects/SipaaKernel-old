/**
 * Global Descriptor Table(GDT) setup
 */
#include "gdt.h"
#include <device/serial/serial.h>

GDT g_gdt[NO_GDT_DESCRIPTORS];
GDT_PTR g_gdt_ptr;

/**
 * fill entries of GDT 
 */
void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    GDT *this = &g_gdt[index];

    this->segment_limit = limit & 0xFFFF;
    this->base_low = base & 0xFFFF;
    this->base_middle = (base >> 16) & 0xFF;
    this->access = access;

    this->granularity = (limit >> 16) & 0x0F;
    this->granularity = this->granularity | (gran & 0xF0);

    this->base_high = (base >> 24 & 0xFF);
}

// initialize GDT
int init_gdt() {
    serial_puts("init_gdt() Setting up GDT pointer...");
    g_gdt_ptr.limit = sizeof(g_gdt) - 1;
    g_gdt_ptr.base_address = (uint32_t)g_gdt;
    serial_puts(" OK\n");

    serial_puts("init_gdt() Setting up GDT entries...");
    // NULL segment
    gdt_set_entry(0, 0, 0, 0, 0);
    // code segment
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // data segment
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // user code segment
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // user data segment
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    serial_puts(" OK\n");

    serial_puts("init_gdt() Loading the GDT into the CPU...");
    load_gdt((uint32_t)&g_gdt_ptr);
    serial_puts(" OK\n");

    return 0;
}
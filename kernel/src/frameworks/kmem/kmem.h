#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <multiboot/boot_info.h>

#define PAGE_SIZE 4096

struct mem_block
{
    struct mem_block *prev;
    struct mem_block *next;
    size_t size;
    bool used;
};

int init_kmem();
void kmem_set_boot_info(multiboot_info_t *mb_info);
void *malloc(size_t size);
void free(void *ptr);
uint32_t get_total_memory();
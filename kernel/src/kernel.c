#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include <multiboot/multiboot.h>
#include <stdlib/stdio.h>
#include <stdlib/stdstr.h>
#include <architecture/x86_32/gdt.h>
#include <architecture/x86_32/idt.h>
#include <architecture/x86_32/tss.h>
#include <architecture/x86_32/rtc.h>
#include <architecture/ioport.h>
#include <frameworks/kmem/kmem.h>
#include <frameworks/libasg/libasg.h>
#include <frameworks/rmusr/rmusr.h>
#include <device/pit/pit.h>
#include <device/ps2/mouse/mouse.h>
#include <device/ps2/kb/kb.h>
#include <device/serial/serial.h>
#include <res/res.h>
#include <tasking/tasking.h>
#include <multiboot/boot_info.h>
#include <debug/debug.h>
#include <frameworks/syscall/syscall.h>

void init(int (*func)(), char* name)
{
    int r = func();
    if (r == 0)
    {
        serial_puts("kstart() It sucessfully loaded!\n");
    }
    else
    {
        serial_puts("kstart() It failed the loading...\n");
    }
}

int frames = 0;
int fps = 0;
char *fps_str = "";

void number_handler()
{
    fps = frames;
    itoa(fps_str, fps, 10);
    fps_str = strcat(fps_str, " FPS");
    frames = 0;
}

void kstart(multiboot_info_t *mboot_info)
{
    set_bootinfo(mboot_info);
    init_libasg(mboot_info);

    serial_puts("kstart() Initializing syscalls...\n");
    init(init_syscall, "Syscalls");

    serial_puts("kstart() Initializing serial...\n");
    init(init_serial, "serial");

    serial_puts("kstart() Initializing GDT...\n");
    init(init_gdt, "GDT");

    serial_puts("kstart() Initializing IDT...\n");
    init(init_idt, "IDT");

    serial_puts("kstart() Initializing PIT...\n");
    init(init_pit, "PIT");

    serial_puts("kstart() Registering PIT timers...\n");
    pit_register_timer(0, 1000, number_handler);
    serial_puts("kstart() It sucessfully happened!\n");

    serial_puts("kstart() Initializing TSS...\n");
    init(init_tss, "TSS");

    serial_puts("kstart() Initializing KMem...\n");
    kmem_set_boot_info(mboot_info);
    init(init_kmem, "kmem");
    
    serial_puts("kstart() Initializing PS/2 Mouse...\n");
    set_screen_size((int)mboot_info->framebuffer_width, (int)mboot_info->framebuffer_height);
    init(init_mouse, "mouse");

    serial_puts("kstart() Initializing PS/2 Keyboard...\n");
    init(init_kb, "kn");

    //__asm__ volatile("int3");

    init_usr();

    /**
    **/
}

/**
 * Userspace entry point
*/
void usr_main()
{
    int syscallNumber = 1;
    char* text = "usr_main() Welcome to userspace!\n           Remember, don't return the function or it will invoke the Invalid Opcode handler & Don't use ring0 instructions like hlt.\n";
    serial_puts(text);

    Color white = from_argb(255, 255, 255, 255);
    Color black = from_argb(255, 0, 0, 0);

    rtc_time_t time = get_current_time();
    time.second = 0;
    set_current_time(time);

    Font f;
    f.charwidth = DEFFONT_CHARWIDTH;
    f.charheight = DEFFONT_CHARHEIGHT;
    f.pixels = deffont;

    while (1)
    {
        clear_screen(black);
        set_pixel(mouse_getx(), mouse_gety(), white);
        set_string(fps_str, 10, 10, f, white);
        flush();

        frames++;
        time = get_current_time();
    }
}
#include "system.h"
#include <architecture/x86_32/isr.h>
#include <frameworks/libasg/libasg.h>
#include <device/serial/serial.h>
#include <multiboot/boot_info.h>
#include <res/res.h>

void _panic(char *message, char *file, char *line, REGISTERS *regs)
{
    Font f;
    f.charwidth = DEFFONT_CHARWIDTH;
    f.charheight = DEFFONT_CHARHEIGHT;
    f.pixels = &deffont;

    Image err;
    err.width = ERROR_WIDTH;
    err.height = ERROR_HEIGHT;
    err.pixels = &error;

    Color bg = from_argb(255, 0, 0, 0);
    Color white = from_argb(255, 255, 255, 255);

    clear_screen(bg);
    Position pos1 = set_string("panic(0) Your computer ran into a big problem.\npanic(0) Message : ", 10, 10, f, white);
    Position pos2 = set_string(message, pos1.x, pos1.y, f, white);
    Position pos3 = set_string("panic(0) File : ", 10, pos2.y + 16, f, white);
    Position pos4 = set_string(file, pos3.x, pos3.y, f, white);
    Position pos5 = set_string("panic(0) Line : ", 10, pos4.y + 16, f, white);
    Position pos6 = set_string(line, pos5.x, pos5.y, f, white);
    set_string("panic(0) System halted!", 10, pos6.y + 16, f, white);
    set_image(&err, mb_info->framebuffer_width / 2 - ERROR_WIDTH / 2, mb_info->framebuffer_height / 2 - ERROR_HEIGHT / 2);

    flush();

    serial_puts("panic(0) Your computer ran into a big problem.\npanic(0) Message : ");
    serial_puts(message);
    serial_puts("\npanic(0) File : ");
    serial_puts(file);
    serial_puts("\npanic(0) Line : ");
    serial_puts(line);
    serial_puts("\npanic(0) System halted!");

    while (1)
    {
        __asm__("hlt");
    }
}
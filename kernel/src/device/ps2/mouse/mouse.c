/**
 * PS2 mouse setup
 * see https://wiki.osdev.org/PS/2_Mouse
 */

#include "mouse.h"

#include <architecture/x86_32/8259_pic.h>
#include <architecture/x86_32/idt.h>
#include <architecture/x86_32/isr.h>
#include <architecture/ioport.h>
#include <device/serial/serial.h>
#include <stdbool.h>

#define TRUE 1
#define FALSE 0

int g_mouse_x_pos = 0, g_mouse_y_pos = 0;
MOUSE_STATUS g_status;
int screenwidth = 0;
int screenheight = 0;
int mstatus = 0;
char sbyte;

int mouse_getx() {
    return g_mouse_x_pos;
}

int mouse_gety() {
    return g_mouse_y_pos;
}

char getMouseStateByte() {
    return sbyte;
}

void mouse_wait(int type) {
    uint32_t time_out = 100000;
    if (type == FALSE) {
        // suspend until status is 1
        while (time_out--) {
            if ((inportb(PS2_CMD_PORT) & 1) == 1) {
                return;
            }
        }
        return;
    } else {
        while (time_out--) {
            if ((inportb(PS2_CMD_PORT) & 2) == 0) {
                return;
            }
        }
    }
}

void mouse_write(uint8_t data) {
    // sending write command
    mouse_wait(TRUE);
    outportb(PS2_CMD_PORT, 0xD4);
    mouse_wait(TRUE);
    // finally write data to port
    outportb(MOUSE_DATA_PORT, data);
}

uint8_t mouse_read() {
    mouse_wait(FALSE);
    return inportb(MOUSE_DATA_PORT);
}

void get_mouse_status(char status_byte, MOUSE_STATUS *status) {
    mstatus = sizeof(status);
    sbyte = status_byte;
    if (status_byte & 0x01)
        status->left_button = 1;
    if (status_byte & 0x02)
        status->right_button = 1;
    if (status_byte & 0x04)
        status->middle_button = 1;
    if (status_byte & 0x08)
        status->always_1 = 1;
    if (status_byte & 0x10)
        status->x_sign = 1;
    if (status_byte & 0x20)
        status->y_sign = 1;
    if (status_byte & 0x40)
        status->x_overflow = 1;
    if (status_byte & 0x80)
        status->y_overflow = 1;
}

void mouse_handler(REGISTERS *r) {
    static uint8_t mouse_cycle = 0;
    static char mouse_byte[3];

    switch (mouse_cycle) {
        case 0:
            mouse_byte[0] = mouse_read();
            get_mouse_status(mouse_byte[0], &g_status);
            mouse_cycle++;
            break;
        case 1:
            mouse_byte[1] = mouse_read();
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[2] = mouse_read();
            g_mouse_x_pos = g_mouse_x_pos + mouse_byte[1];
            g_mouse_y_pos = g_mouse_y_pos - mouse_byte[2];

            if (g_mouse_x_pos < 0)
                g_mouse_x_pos = 0;
            if (g_mouse_y_pos < 0)
                g_mouse_y_pos = 0;
            if (g_mouse_x_pos > screenwidth)
                g_mouse_x_pos = screenwidth - 1;
            if (g_mouse_y_pos > screenheight)
                g_mouse_y_pos = screenheight - 1;

            mouse_cycle = 0;
            break;
    }
    isr_end_interrupt(IRQ_BASE + 12);
}

/**
 * available rates 10, 20, 40, 60, 80, 100, 200
 */
void set_mouse_rate(uint8_t rate) {
    uint8_t status;

    outportb(MOUSE_DATA_PORT, MOUSE_CMD_SAMPLE_RATE);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        //printf("error: failed to send mouse sample rate command\n");
        return;
    }
    outportb(MOUSE_DATA_PORT, rate);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        //printf("error: failed to send mouse sample rate data\n");
        return;
    }
}

void set_screen_size(int w, int h)
{
    screenwidth = w;
    screenheight = h;
}

int init_mouse() {
    uint8_t status;

    g_mouse_x_pos = 5;
    g_mouse_y_pos = 2;
    
    ////printf("initializing mouse...\n");

    // enable mouse device
    serial_puts("init_mouse() Enabling mouse device...");
    mouse_wait(TRUE);
    outportb(PS2_CMD_PORT, 0xA8);

    outportb(MOUSE_DATA_PORT, MOUSE_CMD_MOUSE_ID);
    status = mouse_read();

    set_mouse_rate(10);
    serial_puts(" OK\n");

    // enable the interrupt
    serial_puts("init_mouse() Enabling mouse interrupt...");
    mouse_wait(TRUE);
    outportb(PS2_CMD_PORT, 0x20);
    mouse_wait(FALSE);
    serial_puts(" OK\n");
    // get and set second bit
    status = (inportb(MOUSE_DATA_PORT) | 2);
    // write status to port
    mouse_wait(TRUE);
    outportb(PS2_CMD_PORT, MOUSE_DATA_PORT);
    mouse_wait(TRUE);
    outportb(MOUSE_DATA_PORT, status);

    // set mouse to use default settings
    serial_puts("init_mouse() Making the mouse use the default settings...");
    mouse_write(MOUSE_CMD_SET_DEFAULTS);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        serial_puts(" FAIL\n");
        return 1;
    }
    serial_puts(" OK\n");

    // enable packet streaming to receive
    serial_puts("init_mouse() Enabling packet streaming...");
    mouse_write(MOUSE_CMD_ENABLE_PACKET_STREAMING);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        serial_puts(" FAIL\n");
        return 1;
    }
    serial_puts(" OK\n");

    // set mouse handler
    serial_puts("init_mouse() Register mouse handler into IDT...");
    serial_puts(" OK\n");
    isr_register_interrupt_handler(IRQ_BASE + 12, mouse_handler);

    return 0;
}

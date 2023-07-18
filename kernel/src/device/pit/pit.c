#include "pit.h"

#include <architecture/ioport.h>
#include <architecture/x86_32/isr.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

pit_timer_t timers[PIT_MAX_TIMERS];

void pit_handler(REGISTERS *r) 
{
    for(int i=0; i<PIT_MAX_TIMERS; i++) {
        if(timers[i].interval > 0) {
            timers[i].ticks++;
            if(timers[i].ticks >= timers[i].interval) {
                timers[i].ticks = 0;
                timers[i].callback();
            }
        }
    }
}

int init_pit() {
    uint32_t divisor = PIT_FREQUENCY / 1000; // Set interval to 1ms
    outportb(PIT_COMMAND, 0x36); // Channel 0, mode 3, binary countdown
    outportb(PIT_CHANNEL0, divisor & 0xFF); // Set low byte of divisor
    outportb(PIT_CHANNEL0, (divisor >> 8) & 0xFF); // Set high byte of divisor
    isr_register_interrupt_handler(32, pit_handler);

    return 0;
}

void pit_register_timer(uint8_t timer_id, uint32_t interval, void (*callback)()) {
    if(timer_id < PIT_MAX_TIMERS) {
        timers[timer_id].interval = interval;
        timers[timer_id].ticks = 0;
        timers[timer_id].callback = callback;
    }
}

void pit_unregister_timer(uint8_t timer_id) {
    if(timer_id < PIT_MAX_TIMERS) {
        timers[timer_id].interval = 0;
        timers[timer_id].ticks = 0;
        timers[timer_id].callback = NULL;
    }
}

void pit_wait(uint32_t ms) {
    uint32_t start = timers[0].ticks;
    while((timers[0].ticks - start) < ms);
}
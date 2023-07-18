#include <stdint.h>
#include <stdbool.h>

#define PIT_CHANNEL0    0x40
#define PIT_COMMAND     0x43
#define PIT_FREQUENCY   1193182
#define PIT_MAX_TIMERS  6

typedef struct {
    uint32_t interval;
    uint32_t ticks;
    void (*callback)();
} pit_timer_t;

void pit_handler();
int init_pit();
void pit_unregister_timer(uint8_t timer_id);
void pit_register_timer(uint8_t timer_id, uint32_t interval, void (*callback)());
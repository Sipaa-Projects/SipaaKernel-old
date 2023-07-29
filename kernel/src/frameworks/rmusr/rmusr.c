#include "rmusr.h"
#include <frameworks/libasg/libasg.h>

extern void switch_to_user();

void init_usr(void (*function)())
{
    switch_to_user();
}


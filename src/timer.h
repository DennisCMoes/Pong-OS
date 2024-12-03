#ifndef TIMER_H
#define TIMER_H

#include "system.h"

// Number to use as divisor of PIC frequency
#define TIMER_TPS 363

u64 timer_get();
void timer_init();

#endif

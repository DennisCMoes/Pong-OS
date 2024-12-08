#ifndef TIMER_H
#define TIMER_H

#include "system.h"

// Number to use as divisor of PIC frequency
// #define TIMER_TPS 363
#define TIMER_TPS 1024 // Timer Ticks per Second

// Retrieves the current number of timer ticks since initialization
u64 timer_get();

// Initializes the timer with the specified frequency
void timer_init();

#endif

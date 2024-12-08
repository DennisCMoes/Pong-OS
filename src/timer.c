#include "timer.h"
#include "isr.h"
#include "irq.h"
#include "system.h"

// PIT ports
#define PIT_A       0x40  // Data channel 0
#define PIT_B       0x41  // Data channel 1 (unused)
#define PIT_C       0x42  // Data channel 2 (unused)
#define PIT_CONTROL 0x43  // Control register

// PIT constantss
#define PIT_MASK    0xFF  // Byte mask
#define PIT_SET     0x36  // Mode 3 (Square Wave Generator)
#define PIT_HZ 1193181    // PIT base frequency in Hz

// Timer state struct
static struct {
  u64 frequency;  // Configured frequency of the timer in Hz
  u64 divisor;    // Divisor used to achieve the configured frequency
  u64 ticks;      // Number of ticks since the timer was initialized
} state;

// Set the PIT frequency using the specified frequency divisor
static void timer_set(int hz) {
  port_byte_out(PIT_CONTROL, PIT_SET);

  u16 divisor = (u16) (PIT_HZ / hz);
  port_byte_out(PIT_A, divisor & PIT_MASK);         // Low byte
  port_byte_out(PIT_A, (divisor >> 8) & PIT_MASK);  // High byte
}

// Retrieves the current number of ticks
u64 timer_get() {
  return state.ticks;
}

// Timer interrupt handler called on each timer tick
static void timer_handler(struct Registers *regs) {
  state.ticks++;
}

// Initializes the timer with the specified Ticks Per Second (TIMER_TPS)
void timer_init() {
  const u64 freq = (PIT_HZ / (PIT_HZ / TIMER_TPS));

  state.frequency = freq;
  state.divisor = PIT_HZ / TIMER_TPS;
  state.ticks = 0;

  timer_set(TIMER_TPS);
  irq_install(0, timer_handler);
}


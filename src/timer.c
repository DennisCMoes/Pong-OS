#include "timer.h"
#include "isr.h"
#include "irq.h"
#include "system.h"

#define PIT_A       0x40
#define PIT_B       0x41
#define PIT_C       0x42
#define PIT_CONTROL 0x43

#define PIT_MASK    0xFF
#define PIT_SET     0x36

#define PIT_HZ 1193181

static struct {
  u64 frequency;
  u64 divisor;
  u64 ticks;
} state;

static void timer_set(int hz) {
  port_byte_out(PIT_CONTROL, PIT_SET);

  u16 divisor = (u16) (PIT_HZ / hz);
  port_byte_out(PIT_A, divisor & PIT_MASK);
  port_byte_out(PIT_A, (divisor >> 8) & PIT_MASK);
}

u64 timer_get() {
  return state.ticks;
}

static void timer_handler(struct Registers *regs) {
  state.ticks++;
}

void timer_init() {
  const u64 freq = (PIT_HZ / (PIT_HZ / TIMER_TPS));

  state.frequency = freq;
  state.divisor = PIT_HZ / TIMER_TPS;
  state.ticks = 0;

  timer_set(TIMER_TPS);
  irq_install(0, timer_handler);
}


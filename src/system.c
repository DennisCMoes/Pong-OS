#include "system.h"

// Randomness uses Linear Congruential Generator (LCG)
static u32 rseed = 1;

void seed(u32 seed) {
  rseed = seed ? seed : 1;
}

u32 rand() {
  rseed = (rseed * 1664525 + 1013904223) & 0xFFFFFFFF;
  return rseed;
}

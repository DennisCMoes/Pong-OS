#include "keyboard.h"
#include "system.h"
#include "irq.h"

// Simplified US keyboard layout
char keyboard_layout[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, 9,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 13, 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c',
    'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0
};

struct KeyboardState keyboard_state;

static void keyboard_handler() {
  // Read scancode from the keyboard port
  u8 scancode = port_byte_in(0x60);

  if (scancode >= 128) {
    return;
  }

  if (keyboard_state.buffer_index < KEYBOARD_BUFFER_SIZE - 1) {
    keyboard_state.key_buffer[keyboard_state.buffer_index++] = keyboard_layout[scancode];
  }
}

void keyboard_init() {
  keyboard_state.buffer_index = 0;
  irq_install(1, keyboard_handler);
}

char keyboard_get_key() {
  if (keyboard_state.buffer_index > 0) {
    u8 key = keyboard_state.key_buffer[0];

    // Shift the buffer
    for (u8 i = 0; i < keyboard_state.buffer_index - 1; i++) {
      keyboard_state.key_buffer[i] = keyboard_state.key_buffer[i + 1];
    }

    keyboard_state.buffer_index--;
    return key;
  }

  // No key pressed
  return 0;
}

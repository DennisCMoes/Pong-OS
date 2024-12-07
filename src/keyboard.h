#ifndef KEYBOARD_H
#define KEYBOARD_H

// Defining some basic keys
#define KEY_ESC 27
#define KEY_UP 72
#define KEY_DOWN 80

#define KEYBOARD_BUFFER_SIZE 128

struct KeyboardState {
  char key_buffer[KEYBOARD_BUFFER_SIZE];
  int buffer_index;
};

extern struct KeyboardState keyboard_state;

void keyboard_init();
char keyboard_get_key();

#endif

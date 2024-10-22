#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lib.h>

enum KEYBOARD_OPTIONS {
    SHOW_BUFFER_WHILE_TYPING = 0b00000001,
    AWAIT_RETURN_KEY = 0b00000010
};

int8_t getKeyboardCharacter(enum KEYBOARD_OPTIONS options);
void keyboardHandler();

#endif
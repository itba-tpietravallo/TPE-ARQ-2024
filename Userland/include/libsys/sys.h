#ifndef _SYS_H_
#define _SYS_H_

#include <stdint.h>

// Enum of registerable keys.
// Note: Does not include TAB or RETURN
enum REGISTERABLE_KEYS {
    ESCAPE_KEY = 0x01,
    BACKSPACE_KEY = 0x0E,
    CONTROL_KEY_L = 0x1D,
    SHIFT_KEY_L = 0x2A,
    SHIFT_KEY_R = 0x36,
    ALT_KEY_L = 0x38,
    META_L_KEY = (ALT_KEY_L | 0x80),
    CAPS_LOCK_KEY = 0x3A,
    F1_KEY = 0x3B,
    F2_KEY = 0x3C,
    F3_KEY = 0x3D,
    F4_KEY = 0x3E,
    F5_KEY = 0x3F,
    F6_KEY = 0x40,
    F7_KEY = 0x41,
    F8_KEY = 0x42,
    F9_KEY = 0x43,
    F10_KEY = 0x44,
    NUM_LOCK_KEY = 0x45,
    SCROLL_LOCK_KEY = 0x46,
    KP_HOME_KEY = 0x47,
    KP_UP_KEY = 0x48,
    KP_PAGE_UP_KEY = 0x49,
    KP_LEFT_KEY = 0x4B,
    KP_BEGIN_KEY = 0x4C,
    KP_RIGHT_KEY = 0x4D,
    KP_END_KEY = 0x4F,
    KP_DOWN_KEY = 0x50,
    KP_PAGE_DOWN_KEY = 0x51,
    KP_INSERT_KEY = 0x52,
    KP_DELETE_KEY = 0x53,
    F11_KEY = 0x57,
    F12_KEY = 0x58
};

void beep(void);
void setTextColor(uint32_t color);
void setBackgroundColor(uint32_t color);
void increaseFontSize(void);
void decreaseFontSize(void);
void getDate(int * hour, int * minute, int * second);
void clearScreen(void);
void drawRectangle(int color, long long int width_pixels, long long int height_pixels);
void exec(void (*fnPtr)(void));
void registerKey(enum REGISTERABLE_KEYS scancode, void (*fn)(void));
void clearInputBuffer(void);

#endif
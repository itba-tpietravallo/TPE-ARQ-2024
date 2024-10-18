#include <naiveConsole.h>
#include <keyboard.h>
#include <fonts.h>

#define ESCAPE_KEY 0x01
#define BACKSPACE_KEY 0x0E
#define TABULATOR_KEY 0x0F
#define RETURN_KEY 0x1C
#define CONTROL_KEY_L 0x1D
#define SHIFT_KEY_L 0x2A
#define SHIFT_KEY_R 0x36
#define ALT_KEY_L 0x38
#define META_L_KEY (ALT_KEY_L | 0x80)
#define CAPS_LOCK_KEY 0x3A
#define F1_KEY 0x3B
#define F2_KEY 0x3C
#define F3_KEY 0x3D
#define F4_KEY 0x3E
#define F5_KEY 0x3F
#define F6_KEY 0x40
#define F7_KEY 0x41
#define F8_KEY 0x42
#define F9_KEY 0x43
#define F10_KEY 0x44
#define NUM_LOCK_KEY 0x45
#define SCROLL_LOCK_KEY 0x46
#define KP_HOME_KEY 0x47
#define KP_UP_KEY 0x48
#define KP_PAGE_UP_KEY 0x49
#define KP_LEFT_KEY 0x4B
#define KP_BEGIN_KEY 0x4C
#define KP_RIGHT_KEY 0x4D
#define KP_END_KEY 0x4F
#define KP_DOWN_KEY 0x50
#define KP_PAGE_DOWN_KEY 0x51
#define KP_INSERT_KEY 0x52
#define KP_DELETE_KEY 0x53
#define F11_KEY 0x57
#define F12_KEY 0x58

#define IS_ALPHA(c) ('a' <= (c) && (c) <= 'z') 
#define TO_UPPER(c) (IS_ALPHA(c) ? ((c) - 'a' + 'A') : (c))

static char SHIFT_KEY_PRESSED, CAPS_LOCK_KEY_PRESSED, CONTROL_KEY_PRESSED;

// QEMU source https://github.com/qemu/qemu/blob/master/pc-bios/keymaps/en-us
// http://flint.cs.yale.edu/feng/cos/resources/BIOS/Resources/assembly/makecodes.html
// Array of scancodes to ASCII - Shift-Modified-ASCII
// Note: this is NOT the complete QEMU scancode set, but it does include all printable characters and most control keys
static const uint8_t scancodeMap[][2] = {
    /* 0x00 */ { 0, 0 },
    /* 0x01 */ { ESCAPE_KEY, ESCAPE_KEY },
    /* 0x02 */ { '1', '!' },
    /* 0x03 */ { '2', '@' },
    /* 0x04 */ { '3', '#' },
    /* 0x05 */ { '4', '$' },
    /* 0x06 */ { '5', '%' },
    /* 0x07 */ { '6', '^' },
    /* 0x08 */ { '7', '&' },
    /* 0x09 */ { '8', '*' },
    /* 0x0A */ { '9', '(' },
    /* 0x0B */ { '0', ')' },
    /* 0x0C */ { '-', '_' },
    /* 0x0D */ { '=', '+' },
    /* 0x0E */ { BACKSPACE_KEY, BACKSPACE_KEY },
    /* 0x1F */ { TABULATOR_KEY, TABULATOR_KEY },
    /* 0x10 */ { 'q', 'Q' },
    /* 0x11 */ { 'w', 'W' },
    /* 0x12 */ { 'e', 'E' },
    /* 0x13 */ { 'r', 'R' },
    /* 0x14 */ { 't', 'T' },
    /* 0x15 */ { 'y', 'Y' },
    /* 0x16 */ { 'u', 'U' },
    /* 0x17 */ { 'i', 'I' },
    /* 0x18 */ { 'o', 'O' },
    /* 0x19 */ { 'p', 'P' },
    /* 0x1A */ { '[', '{' },
    /* 0x1B */ { ']', '}' },
    /* 0x1C */ { RETURN_KEY, RETURN_KEY },
    /* 0x1D */ { CONTROL_KEY_L, CONTROL_KEY_L },
    /* 0x1E */ { 'a', 'A' },
    /* 0x1F */ { 's', 'S' },
    /* 0x20 */ { 'd', 'D' },
    /* 0x21 */ { 'f', 'F' },
    /* 0x22 */ { 'g', 'G' },
    /* 0x23 */ { 'h', 'H' },
    /* 0x24 */ { 'j', 'J' },
    /* 0x25 */ { 'k', 'K' },
    /* 0x26 */ { 'l', 'L' },
    /* 0x27 */ { ';', ':' },
    /* 0x28 */ { '\'', '"' },
    /* 0x29 */ { '`', '~' },
    /* 0x2A */ { SHIFT_KEY_L, SHIFT_KEY_L },
    /* 0x2B */ { '\\', '|' },
    /* 0x2C */ { 'z', 'Z' },
    /* 0x2D */ { 'x', 'X' },
    /* 0x2E */ { 'c', 'C' },
    /* 0x2F */ { 'v', 'V' },
    /* 0x30 */ { 'b', 'B' },
    /* 0x31 */ { 'n', 'N' },
    /* 0x32 */ { 'm', 'M' },
    /* 0x33 */ { ',', '<' },
    /* 0x34 */ { '.', '>' },
    /* 0x35 */ { '/', '?' },
    /* 0x36 */ { SHIFT_KEY_R, SHIFT_KEY_R },
    /* 0x37 */ { '*', '*' },
    /* 0x38 */ { ALT_KEY_L, META_L_KEY }, // Left Alt
    /* 0x39 */ { ' ', ' ' },
    /* 0x3A */ { CAPS_LOCK_KEY, CAPS_LOCK_KEY }, // Caps Lock
    /* 0x3B */ { F1_KEY, F1_KEY },
    /* 0x3C */ { F2_KEY, F2_KEY },
    /* 0x3D */ { F3_KEY, F3_KEY },
    /* 0x3E */ { F4_KEY, F4_KEY },
    /* 0x3F */ { F5_KEY, F5_KEY },
    /* 0x40 */ { F6_KEY, F6_KEY },
    /* 0x41 */ { F7_KEY, F7_KEY },
    /* 0x42 */ { F8_KEY, F8_KEY },
    /* 0x43 */ { F9_KEY, F9_KEY },
    /* 0x44 */ { F10_KEY, F10_KEY },
    /* 0x45 */ { NUM_LOCK_KEY, NUM_LOCK_KEY },
    /* 0x46 */ { SCROLL_LOCK_KEY, SCROLL_LOCK_KEY },
    /* 0x47 */ { KP_HOME_KEY, '7' },
    /* 0x48 */ { KP_UP_KEY, '8' },
    /* 0x49 */ { KP_PAGE_UP_KEY, '9' },
    /* 0x4A */ { '-', '-' },
    /* 0x4B */ { KP_LEFT_KEY, '4' },
    /* 0x4C */ { KP_BEGIN_KEY, '5' },
    /* 0x4D */ { KP_RIGHT_KEY, '6' },
    /* 0x4E */ { '+', '+' },
    /* 0x4F */ { KP_END_KEY, '1' },
    /* 0x50 */ { KP_DOWN_KEY, '2'}, 
    /* 0x51 */ { KP_PAGE_DOWN_KEY, '3' },
    /* 0x52 */ { KP_INSERT_KEY, '0' },
    /* 0x53 */ { KP_DELETE_KEY, '.' },
    /* 0x54 */ { 0, 0 }, // 0x54
    /* 0x55 */ { 0, 0 }, // 0x55
    /* 0x56 */ { '-', '-' },
    /* 0x57 */ { F11_KEY, F11_KEY },
    /* 0x58 */ { F12_KEY, F12_KEY },
};

static uint8_t isReleased(uint8_t scancode) {
    return scancode & 0x80;
}

static uint8_t isPressed(uint8_t scancode) {
    return !(isReleased(scancode));
}

static uint8_t isShift(uint8_t scancode){
    uint8_t aux = scancode & 0x7F;
    return aux == SHIFT_KEY_L || aux == SHIFT_KEY_R;
}

static uint8_t isCapsLock(uint8_t scancode){
    return (scancode & 0x7F) == CAPS_LOCK_KEY;
}

static uint8_t isControl(uint8_t scancode){
    return (scancode & 0x7F) == CONTROL_KEY_L;
}

void keyboardHandler(){
    uint8_t scancode = getKeyboardBuffer();
    uint8_t is_pressed = isPressed(scancode);
    // TODO: implement shift-capslock-control array
    if (isShift(scancode)){
        if (is_pressed){
            SHIFT_KEY_PRESSED = 1;
        } else{
            SHIFT_KEY_PRESSED = 0;
        }
    } else if (isCapsLock(scancode)){
        if (is_pressed){
            CAPS_LOCK_KEY_PRESSED = !CAPS_LOCK_KEY_PRESSED;
        }
    } else if (isControl(scancode)){
        if (is_pressed){
            CONTROL_KEY_PRESSED = 1;
        } else{
            CONTROL_KEY_PRESSED = 0;
        }
    } else if (is_pressed){
        char c = scancodeMap[scancode][SHIFT_KEY_PRESSED];
        if(CAPS_LOCK_KEY_PRESSED == 1){
            c = TO_UPPER(c);
        }
        putChar(c);
    }
}

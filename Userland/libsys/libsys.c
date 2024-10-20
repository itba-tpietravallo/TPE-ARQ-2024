#include <sys.h>
#include <syscalls.h>

void beep(void) {
    sys_beep();
}

void setTextColor(uint32_t color) {
    sys_fonts_text_color(color);
}

void setBackgroundColor(uint32_t color) {
    sys_fonts_background_color(color);
}

void increaseFontSize(void) {
    sys_fonts_increase_size();
}

void decreaseFontSize(void) {
    sys_fonts_decrease_size();
}

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

void getDate(int * hour, int * minute, int * second) {
    sys_hour(hour);
    sys_minute(minute);
    sys_second(second);
}

void clearScreen(void) {
    sys_clear_screen();
}

void drawRectangle(int color, long long int width_pixels, long long int height_pixels){
    sys_rectangle(color, width_pixels, height_pixels);
}
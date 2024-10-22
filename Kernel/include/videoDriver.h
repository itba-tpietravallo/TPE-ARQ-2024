#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

uint16_t getWindowWidth(void);
uint16_t getWindowHeight(void);

void scrollVideoMemoryUp(uint16_t scroll, uint32_t fillColor);

#endif
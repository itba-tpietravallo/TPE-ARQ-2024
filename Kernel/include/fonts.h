#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>
#include <videoDriver.h>

#define DEFAULT_TEXT_COLOR 0x00FFFFFF
#define DEFAULT_ERROR_COLOR 0x00DE382B
#define DEFAULT_BACKGROUND_COLOR 0x00000000

void putChar(char ascii);
void print(const char * string);
int32_t printToFd(int32_t fd, const char * string, int32_t count);
void newLine();
void printDec(uint64_t value);
void printHex(uint64_t value);
void printBin(uint64_t value);
void clear(void);

void clearPreviousPixel(void);

void increaseFontSize(void);
void decreaseFontSize(void);
void setTextColor(uint32_t color);
void setBackgroundColor(uint32_t color);

void __DEBUG__renderTicks(uint64_t ticks);

#endif
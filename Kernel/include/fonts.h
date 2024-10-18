#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>
#include <videoDriver.h>

void putChar(char ascii);
void print(char * string);
void newLine();
void printDec(uint64_t value);
void printHex(uint64_t value);
void printBin(uint64_t value);
void clear();

#endif
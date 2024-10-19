#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <fonts.h>

#define EOF -1

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void printf(const char * string);
// int64_t strcmp(const char * s1, const char * s2);

char *cpuVendor(char *result);

uint8_t getKeyboardBuffer(void);
uint8_t getKeyboardStatus(void);

char getSecond(void);
char getMinute(void);
char getHour(void);

#endif
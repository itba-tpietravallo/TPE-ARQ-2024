#ifndef _SYS_H_
#define _SYS_H_

#include <stdint.h>

void beep(void);
void setTextColor(uint32_t color);
void setBackgroundColor(uint32_t color);
void increaseFontSize(void);
void decreaseFontSize(void);

#endif
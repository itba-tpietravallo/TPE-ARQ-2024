#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void sleep(int seconds);
void sleepTicks(uint64_t sleep_t);

#endif

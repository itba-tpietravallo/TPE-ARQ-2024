#include <time.h>
#include <interrupts.h>

#include <fonts.h>

#define SECONDS_TO_TICKS 18

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;

	// note: this should always be at the end of the timer handler
	__DEBUG__renderTicks(ticks);
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / SECONDS_TO_TICKS;
}

void sleepTicks(uint64_t sleep_t) {
	unsigned long start = ticks;
	while (ticks < start + sleep_t) _hlt();
	return;
}

void sleep(int seconds) {
	sleepTicks(seconds * SECONDS_TO_TICKS);
	return;
}

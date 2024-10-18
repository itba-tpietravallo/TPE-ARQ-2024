#include <time.h>
#include <naiveConsole.h>

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
	return ticks / 18;
}

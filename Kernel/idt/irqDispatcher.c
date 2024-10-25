#include <time.h>
#include <stdint.h>
#include <keyboard.h>

static void int_20();
static void int_21();

static void (*interruptions[]) (void) = {
	int_20,
	int_21
};

void irqDispatcher(uint64_t irq) {
	if (irq < 2) {
		interruptions[irq]();
	}
	return;
}

static void int_20() {
	timer_handler();
}

static void int_21() {
	keyboardHandler();
}

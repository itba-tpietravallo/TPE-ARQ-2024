#include <fonts.h>


#define ZERO_EXCEPTION_ID 0

static void zero_division();

void exceptionDispatcher(int exception) {
	switch(exception) {
		case ZERO_EXCEPTION_ID:
			return zero_division();
			break;

		default:
			print("Triggered exception dispatcher, but no syscall found");

	}
}

static void zero_division() {
	print("Estas en zero division");
}
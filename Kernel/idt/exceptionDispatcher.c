#include <fonts.h>
#include <interrupts.h>
#include <syscallDispatcher.h>
#include <keyboard.h>

static int (*const shellModuleAddress)(void) = (int(*)(void)) 0x400000;

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

static void zero_division(Registers * registers, int errorCode);
static void invalid_opcode(Registers * registers, int errorCode);

void printExceptionData(Registers * registers, int errorCode);

void exceptionDispatcher(int exception, Registers * registers) {
	switch(exception) {
		case ZERO_EXCEPTION_ID:
			return zero_division(registers, exception);
			break;
		case INVALID_OPCODE_ID:
			return invalid_opcode(registers, exception);
			break;
		default:
			print("Triggered exception dispatcher, but no syscall found");
	}
}

// TODO beautify (and enchance) this



static void zero_division(Registers * registers, int errorCode) {
	clear();
	setTextColor(0x00FF0000);
	print("Zero Dividing exception number "); 
	printExceptionData(registers, errorCode);
}

static void invalid_opcode(Registers * registers, int errorCode) {
	clear();
	setTextColor(0x00FF6600);
	print("Invalid Opcode exception number "); 
	printExceptionData(registers, errorCode);
}


void printExceptionData(Registers * registers, int errorCode) {
	printDec(errorCode); print(" ocurred\n");
	print("Current registers values are\n");
	print("rip = ");	printHex(registers-> rip); print("\n");
	print("\n");

	print("rax = ");	printHex(registers-> rax); print("\n");
	print("rbx = ");	printHex(registers-> rbx); print("\n");
	print("rcx = ");	printHex(registers-> rcx); print("\n");
	print("rdx = ");	printHex(registers-> rdx); print("\n");
	print("rbp = ");	printHex(registers-> rbp); print("\n");
	print("rdi = ");	printHex(registers-> rdi); print("\n");
	print("rsi = ");	printHex(registers-> rsi); print("\n");
	print("r8 = ");		printHex(registers-> r8); print("\n");
	print("r9 = ");		printHex(registers-> r9); print("\n");
	print("r10 = ");	printHex(registers-> r10); print("\n");
	print("r11 = ");	printHex(registers-> r11); print("\n");
	print("r12 = ");	printHex(registers-> r12); print("\n");
	print("r13 = ");	printHex(registers-> r13); print("\n");
	print("r14 = ");	printHex(registers-> r14); print("\n");
	print("r15 = ");	printHex(registers-> r15); print("\n");
	setTextColor(0x00FFFFFF);

	print("Press r to go back to Shell");

	char a;
	while ((a = getKeyboardCharacter(0)) != 'r') {}

	sys_exec(shellModuleAddress);

	__builtin_unreachable();	
}
#include <fonts.h>
#include <interrupts.h>
#include<syscallDispatcher.h>
#include<keyboard.h>

static void * const sampleCodeModuleAddress = (void*)0x400000;

typedef int (*EntryPoint)();


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
	print("Zero Dividing Exception number "); 
	printExceptionData(registers, errorCode);
}

static void invalid_opcode(Registers * registers, int errorCode) {

	clear();
	setTextColor(0x00FF6600);
	print("Invalid Opcode Exception number  "); 
	printExceptionData(registers, errorCode);
}


void printExceptionData(Registers * registers, int errorCode) {

	printDec(errorCode); print("Ocurred \n");
	print("Current registers values are\n");
	print("rax = ");	printDec(registers-> rax); print("\n");
	print("rbx = ");	printDec(registers-> rbx); print("\n");
	print("rcx = ");	printDec(registers-> rcx); print("\n");
	print("rdx = ");	printDec(registers-> rdx); print("\n");
	print("rbp = ");	printDec(registers-> rbp); print("\n");
	print("rdi = ");	printDec(registers-> rdi); print("\n");
	print("rsi = ");	printDec(registers-> rsi); print("\n");
	print("r8 = ");		printDec(registers-> r8); print("\n");
	print("r9 = ");		printDec(registers-> r9); print("\n");
	print("r10 = ");	printDec(registers-> r10); print("\n");
	print("r11 = ");	printDec(registers-> r11); print("\n");
	print("r12 = ");	printDec(registers-> r12); print("\n");
	print("r13 = ");	printDec(registers-> r13); print("\n");
	print("r14 = ");	printDec(registers-> r14); print("\n");
	print("r15 = ");	printDec(registers-> r15); print("\n");
	setTextColor(0x00FFFFFF);

	print("Press r to go back to Shell");

	char a;
	while ((a = getKeyboardCharacter()) != 'r'){}

	((EntryPoint)sampleCodeModuleAddress)();
	// while(1) {} _hlt();
	
}
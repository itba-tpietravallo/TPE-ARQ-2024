#include <syscallDispatcher.h>

#include <fonts.h>

// @todo Note: Technically.. registers on the stack are modifiable (since its a struct pointer, not struct). 
void syscallDispatcher(Registers * registers) {
	switch(registers->rax){
		case 1: 
			print("Hello from syscall 1\n");
			break;
		default:
			print("Triggered but not RAX=1");
			break;
	}
}

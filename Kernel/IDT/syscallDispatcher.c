#include <syscallDispatcher.h>

#include <fonts.h>

static int32_t sys_write(int32_t fd, char * __user_buf, int32_t count);

// @todo Note: Technically.. registers on the stack are modifiable (since its a struct pointer, not struct). 
int64_t syscallDispatcher(Registers * registers) {
	switch(registers->rax){
		case 4:
            // Note: Register parameters are 64-bit
			return sys_write(registers->rdi, (char *) registers->rsi, registers->rdx);
		default:
			print("Triggered syscall dispatcher, \e[0;31mbut no syscall found\e[0m");
            return 0;
	}
}

static int32_t sys_write(int32_t fd, char * __user_buf, int32_t count) {
    return printToFd(fd, __user_buf, count);
}

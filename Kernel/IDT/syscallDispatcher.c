#include <syscallDispatcher.h>
#include <keyboard.h>

#include <fonts.h>

static int32_t sys_write(int32_t fd, char * __user_buf, int32_t count);
static int32_t sys_read(int32_t fd, char * __user_buf, int32_t count);

// @todo Note: Technically.. registers on the stack are modifiable (since its a struct pointer, not struct). 
int64_t syscallDispatcher(Registers * registers) {
	switch(registers->rax){
		case 3:
			return sys_read(registers->rdi, (char *) registers->rsi, registers->rdx);
		case 4: 
			// Note: Register parameters are 64-bit
			return sys_write(registers->rdi, (char *) registers->rsi, registers->rdx);
		case 13: 
			return sys_time(registers->rdi, (int *) registers->rsi);
		default:
			print("Triggered syscall dispatcher, \e[0;31mbut no syscall found\e[0m");
            return 0;
	}
}

static int32_t sys_write(int32_t fd, char * __user_buf, int32_t count) {
    return printToFd(fd, __user_buf, count);
}

static int32_t sys_read(int32_t fd, char * __user_buf, int32_t count) {
	int32_t i;
	int8_t c;
	for(i = 0; i < count && (c = getKeyboardCharacter()) != EOF; i++){
		*(__user_buf + i) = c;
	}
    return i;
}

static int32_t sys_time(int64_t feature, void * buf){
	int32_t to_return = 1;
	switch (feature){
	case 0:
		/* code */
		break;
	
	default:
		to_return = -1;
		break;
	}
}

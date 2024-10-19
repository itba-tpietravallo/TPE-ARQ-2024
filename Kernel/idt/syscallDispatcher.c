#include <syscallDispatcher.h>

#include <sound.h>
#include <keyboard.h>
#include <fonts.h>

// Linux syscall prototypes
static int32_t sys_write(int32_t fd, char * __user_buf, int32_t count);
static int32_t sys_read(int32_t fd, char * __user_buf, int32_t count);

// Custom syscall prototypes
static int32_t sys_beep(void);

// @todo Note: Technically.. registers on the stack are modifiable (since its a struct pointer, not struct). 
int64_t syscallDispatcher(Registers * registers) {
	switch(registers->rax){
		case 3:
			return sys_read(registers->rdi, (char *) registers->rsi, registers->rdx);
		case 4: 
			// Note: Register parameters are 64-bit
			return sys_write(registers->rdi, (char *) registers->rsi, registers->rdx);
		case 0x80000000:
			return sys_beep();
		default:
			print("Triggered syscall dispatcher, \e[0;31mbut no syscall found\e[0m");
            return 0;
	}
}

// ==================================================================
// Linux syscalls
// ==================================================================

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

// ==================================================================
// Custom system calls
// ==================================================================

static int32_t sys_beep(void) {
	beep();
	return 0;
}
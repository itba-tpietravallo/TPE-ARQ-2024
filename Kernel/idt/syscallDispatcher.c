#include <syscallDispatcher.h>

#include <sound.h>
#include <keyboard.h>
#include <fonts.h>
#include <lib.h>

// Linux syscall prototypes
static int32_t sys_write(int32_t fd, char * __user_buf, int32_t count);
static int32_t sys_read(int32_t fd, char * __user_buf, int32_t count);

// Custom syscall prototypes
static int32_t sys_beep(void);
static int32_t sys_fonts_text_color(uint32_t color);
static int32_t sys_fonts_background_color(uint32_t color);
static int32_t sys_fonts_decrease_size(void);
static int32_t sys_fonts_increase_size(void);

// Date syscall prototypes
static uint8_t sys_hour(int * hour);
static uint8_t sys_minute(int * minute);
static uint8_t sys_second(int * second);

// @todo Note: Technically.. registers on the stack are modifiable (since its a struct pointer, not struct). 
int64_t syscallDispatcher(Registers * registers) {
	switch(registers->rax){
		case 3: return sys_read(registers->rdi, (char *) registers->rsi, registers->rdx);
		// Note: Register parameters are 64-bit
		case 4: return sys_write(registers->rdi, (char *) registers->rsi, registers->rdx);
		
		case 0x80000000: return sys_beep();
		case 0x80000001: /* Reserved for sys_beep_frequency_time */ return -1;
		case 0x80000002: return sys_fonts_text_color(registers->rdi);
		case 0x80000003: return sys_fonts_background_color(registers->rdi);
		case 0x80000004: /* Reserved for sys_set_italics */
		case 0x80000005: /* Reserved for sys_set_bold */
		case 0x80000006: /* Reserved for sys_set_underline */
			return -1;
		case 0x80000007: return sys_fonts_decrease_size();
		case 0x80000008: return sys_fonts_increase_size();

		case 0x80000010: return sys_hour((int *) registers->rdi);
		case 0x80000011: return sys_minute((int *) registers->rdi);
		case 0x80000012: return sys_second((int *) registers->rdi);

		default:
			setBackgroundColor(0x00FF0000); setTextColor(0x00FFFFFF);
			// @todo This should probably be an exception handler
			print("Triggered syscall dispatcher, but no syscall found");
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
		putChar(c);
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

static int32_t sys_fonts_text_color(uint32_t color) {
	setTextColor(color);
	return 0;
}

static int32_t sys_fonts_background_color(uint32_t color) {
	setBackgroundColor(color);
	return 0;
}

static int32_t sys_fonts_decrease_size(void) {
	decreaseFontSize();
	return 0;
}

static int32_t sys_fonts_increase_size(void) {
	increaseFontSize();
	return 0;
}

// ==================================================================
// Date system calls
// ==================================================================

static uint8_t sys_hour(int * hour) {
	*hour = getHour();
	printHex(*hour);
	return 0;
}

static uint8_t sys_minute(int * minute) {
	*minute = getMinute();
	printHex(*minute);
	return 0;
}

static uint8_t sys_second(int * second) {
	*second = getSecond();
	printHex(*second);
	return 0;
}

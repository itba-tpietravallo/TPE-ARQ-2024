#include <syscallDispatcher.h>

#include <sound.h>
#include <keyboard.h>
#include <fonts.h>
#include <lib.h>
#include <videoDriver.h>

// @todo Note: Technically.. registers on the stack are modifiable (since its a struct pointer, not struct). 
int32_t syscallDispatcher(Registers * registers) {
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
		case 0x80000009: /* Reserved for sys_fonts_set_size */
		case 0x8000000A: return sys_clear_screen();
		case 0x8000000B: return sys_clear_input_buffer();

		case 0x80000010: return sys_hour((int *) registers->rdi);
		case 0x80000011: return sys_minute((int *) registers->rdi);
		case 0x80000012: return sys_second((int *) registers->rdi);

		case 0x80000020: return sys_rectangle(registers->rdi, registers->rsi, registers->rdx);

		case 0x800000A0: return sys_exec((int (*)(void)) registers->rdi);
		case 0x800000B0: return sys_register_key((uint8_t) registers->rdi, (SpecialKeyHandler) registers->rsi);

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

int32_t sys_write(int32_t fd, char * __user_buf, int32_t count) {
    return printToFd(fd, __user_buf, count);
}

int32_t sys_read(int32_t fd, char * __user_buf, int32_t count) {
	int32_t i;
	int8_t c, stop = 0;
	for(i = 0; i < count && !stop; i++){
		c = getKeyboardCharacter(AWAIT_RETURN_KEY | SHOW_BUFFER_WHILE_TYPING);
		*(__user_buf + i) = c;
		if(c == EOF){
			stop = 1;
		}
	}
    return i;
}

// ==================================================================
// Custom system calls
// ==================================================================

int32_t sys_beep(void) {
	beep();
	return 0;
}

int32_t sys_fonts_text_color(uint32_t color) {
	setTextColor(color);
	return 0;
}

int32_t sys_fonts_background_color(uint32_t color) {
	setBackgroundColor(color);
	return 0;
}

int32_t sys_fonts_decrease_size(void) {
	decreaseFontSize();
	return 0;
}

int32_t sys_fonts_increase_size(void) {
	increaseFontSize();
	return 0;
}

int32_t sys_clear_screen(void) {
	clear();
	return 0;
}

int32_t sys_clear_input_buffer(void) {
	while(clearBuffer() != 0);
	return 0;
}

// ==================================================================
// Date system calls
// ==================================================================

int32_t sys_hour(int * hour) {
	*hour = getHour();
	printHex(*hour);
	return 0;
}

int32_t sys_minute(int * minute) {
	*minute = getMinute();
	printHex(*minute);
	return 0;
}

int32_t sys_second(int * second) {
	*second = getSecond();
	printHex(*second);
	return 0;
}

// ==================================================================
// Draw system calls
// ==================================================================

int32_t sys_rectangle(uint32_t color, uint64_t width_pixels, uint64_t height_pixels){
	// print("Hello from sys_dispatcher    ");
	drawRectangle(color, width_pixels, height_pixels);
	return 0;
}

// ==================================================================
// Custom exec system call
// ==================================================================

int32_t sys_exec(int32_t (*fnPtr)(void)) {
	clearKeyFnMap(); // avoid """processes/threads/apps""" registering keys across each other over time. reset the map every time
	return fnPtr();
}

// ==================================================================
// Custom keyboard system calls
// ==================================================================

int32_t sys_register_key(uint8_t scancode, SpecialKeyHandler fn){
	registerSpecialKey(scancode, fn, 0);
	return 0;
}

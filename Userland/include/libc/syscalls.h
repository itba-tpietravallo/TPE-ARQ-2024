#ifndef _LIBC_SYSCALLS_H_
#define _LIBC_SYSCALLS_H_

#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

#include <stdint.h>

// Linux syscall prototypes
int64_t sys_write(int64_t fd, const void * buf, int64_t count);
int64_t sys_read(int64_t fd, void * buf, int64_t count);

// Custom syscall prototypes
/* 0x80000000 */
int64_t sys_beep(void);
/* 0x80000002 */
int64_t sys_fonts_text_color(uint32_t color);
/* 0x80000003 */
int64_t sys_fonts_background_color(uint32_t color);
/* 0x80000007 */
int64_t sys_fonts_decrease_size(void);
/* 0x80000008 */
int64_t sys_fonts_increase_size(void);

// Date syscall prototypes
/* 0x80000010 */
uint8_t sys_hour(int * hour);
/* 0x80000011 */
uint8_t sys_minute(int * minute);
/* 0x80000012 */
uint8_t sys_second(int * second);

#endif
#ifndef _LIBC_SYS_H_
#define _LIBC_SYS_H_

#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

#include <stdint.h>

int64_t sys_write(int64_t fd, const void * buf, int64_t count);
int64_t sys_read(int64_t fd, void * buf, int64_t count);

#endif
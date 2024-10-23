#ifndef _LIBC_STDIO_H_
#define _LIBC_STDIO_H_

#include <string.h>

#define FD_STDIN  0
#define FD_STDOUT 1
#define FD_STDERR 2

void puts(const char * str);
void printf(const char * str);
void fprintf(int fd, const char * str);
int getchar();
void putchar(const char c);

#endif
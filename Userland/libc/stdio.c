#include <stdio.h>
#include <string.h>

// LD with NO entry @todo. .ld file ENTRY(_start)
void puts(char * str) {
    int len = strlen(str);
    // sys_write(1, str, len);
    // sys_write
    // while (*str) {
    //     putchar(*str);
    //     str++;
    // }
    // putchar('\n');
}

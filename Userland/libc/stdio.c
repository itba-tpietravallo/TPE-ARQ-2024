#include <stdio.h>
#include <string.h>
#include <syscalls.h>

#ifdef ANSI_8_BIT_COLOR_SUPPORT
    #include <ansiColors.h>
#endif

void puts(const char * str) {
    printf(str);
    sys_write(FD_STDOUT, "\n", 1);
}

void printf(const char * str) {
    #ifndef ANSI_8_BIT_COLOR_SUPPORT
        int len = strlen(str);
        sys_write(FD_STDOUT, str, len);
    #else
        int i = 0;
        while(str[i] != 0){
            if(str[i] == '\e' || str[i] == '\033'){
                parseANSI(str, &i);
            } else {
                sys_write(FD_STDOUT, &str[i], 1);
                i++;
            }
        }
    #endif
}

void fprintf(int fd, const char * str) {
    sys_write(fd, str, strlen(str));
}

void perror(const char * s1) {
    sys_write(FD_STDERR, s1, strlen(s1));
}

int getchar(void) {
    char c[1];
    while(sys_read(FD_STDIN, c, 1) == -1);
    return c[0];
}

void putchar(const char c) {
    sys_write(FD_STDOUT, &c, 1);
};
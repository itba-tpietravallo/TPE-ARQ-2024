#include <stdio.h>
#include <string.h>
#include <sys.h>

void puts(const char * str) {
    printf(str);
    sys_write(FD_STDOUT, "\n", 1);
}

void printf(const char * str){
    int len = strlen(str);
    sys_write(FD_STDOUT, str, len);
}

char getchar(){
    char c[1];
    // puts("GETCHAR");
    while(sys_read(FD_STDIN, c, 1) == -1);
    return c[0];
}

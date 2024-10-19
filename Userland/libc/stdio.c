#include <stdio.h>
#include <string.h>
#include <sys.h>

void puts(char * str) {
    int len = strlen(str);
    sys_write(FD_STDOUT, str, len);
    sys_write(FD_STDOUT, "\n", 1);
}

#include <sys.h>
#include <syscalls.h>

void beep(void) {
    sys_beep();
}
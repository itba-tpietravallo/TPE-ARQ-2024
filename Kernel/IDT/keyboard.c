#include <naiveConsole.h>
#include <keyboard.h>

void keyboard_handler(){
    char c = getKey();
	ncPrintChar(c);
}

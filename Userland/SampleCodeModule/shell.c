#include <stdio.h>
#include <stdlib.h>
#include <sys.h>

#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];
static int buffer_dim;

int main() {
	// puts("Hello, World!\n"); // compiles! (does not print yet)
	
	printf("shell \e[0;32m$\e[0m ");
    beep();

    char c, buffer_has_space = 1;

    while(buffer_has_space && (c = getchar()) != '\n'){
        if(buffer_dim == BUFFER_SIZE - 1){
            printf("Shell buffer overflow");
            buffer_has_space = 0;
        } else{
            buffer[buffer_dim++] = c;
        }
    }

    buffer[buffer_dim] = 0;
    puts((char *)buffer);

	return one();
}

void date(){
	
}
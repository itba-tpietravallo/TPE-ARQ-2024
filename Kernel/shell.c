#include <shell.h>

#define BUFFER_SIZE 1024

static uint8_t buffer[BUFFER_SIZE];
static uint8_t buffer_dim;

void shell(){
    printf("shell \e[0;32m$\e[0m ");

    uint8_t c, buffer_has_space = 1;

    while(buffer_has_space && (c = getChar()) != '\n'){
        if(buffer_dim == BUFFER_SIZE - 1){
            printf("Shell buffer overflow");
            buffer_has_space = 0;
        } else{
            buffer[buffer_dim++] = c;
        }
    }

    buffer[buffer_dim] = 0;
    printf(buffer);
}

#include <shell.h>

#define BUFFER_SIZE 1024

static uint8_t buffer[BUFFER_SIZE];
static uint8_t buffer_dim;

void shell(){
    printf("shell ~ ");

    uint8_t c, buffer_has_space = 1;

    while(buffer_has_space && (c = getChar()) != '\n'){
        if(buffer_dim == BUFFER_SIZE - 1){
            printf("Shell buffer overflow");
            buffer_has_space = 0;
        } else{
            buffer[buffer_dim++] = c;
        }
    }

    // int8_t c = getChar();
    // while(c == EOF){
    //     c = getChar();
    // }

    // uint8_t c, can_continue = 1;
    // while(can_continue && (c = getChar()) != '\n'){
    //     if(c != EOF){
    //         if(buffer_dim == BUFFER_SIZE - 1){
    //             printf("Shell buffer overflow");
    //             can_continue = 0;
    //         } else{
    //             buffer[buffer_dim++] = c;
    //         }
    //     } 
    // }

    // if(can_continue){
    //     buffer[buffer_dim] = 0;
    //     printf(buffer);
    // }
}

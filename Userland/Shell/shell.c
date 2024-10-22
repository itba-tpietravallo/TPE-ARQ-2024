#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys.h>
#include <exceptions.h>

#define MAX_BUFFER_SIZE 1024

static char buffer[MAX_BUFFER_SIZE];
static int buffer_dim = 0;

void date(void);

int main() {
	while (1) {
        printf("\e[0mshell \e[0;32m$\e[0m ");

        char c;

        while(buffer_dim < MAX_BUFFER_SIZE && (c = getchar()) != '\n'){
            buffer[buffer_dim++] = c;
        }

        if(buffer_dim == 1024){
            printf("\n\e[0;31mShell buffer overflow\e[0m\n");
            return 1;
        };

        buffer[buffer_dim] = 0;
        
        char * command = strtok(buffer, " ");

        if (strcmp(command, "echo") == 0) {
            printf(buffer + strlen(command) + 1);
        } else if (strcmp(command, "divzero") == 0) {
            _divzero();
        } else {
            printf("Command not found");
        } 
    
        printf("\n");
        buffer[0] = buffer_dim = 0;
    }

	return one();
}

void date(void){
	int hour, minute, second;
    getDate(&hour, &minute, &second);
    // call to printf()
}
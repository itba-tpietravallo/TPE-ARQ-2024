#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys.h>
#include <exceptions.h>

#define MAX_BUFFER_SIZE 1024

static char buffer[MAX_BUFFER_SIZE];
static int buffer_dim = 0;

int date(void);
int echo(void);
int clear(void);
int help(void);

typedef struct {
    char * name;
    int (*function)(void);
    char * description;
} Command;

Command commands[] = {
    { .name = "echo",       .function = (int (*)(void))(unsigned long long) echo,       .description = "Prints the input string" },
    { .name = "divzero",    .function = (int (*)(void))(unsigned long long) _divzero,   .description = "Generates a division by zero exception" },
    { .name = "invalidopcode",    .function = (int (*)(void))(unsigned long long) _invalidopcode,   .description = "Generates an invalid Opcode exception" },
    { .name = "date",       .function = (int (*)(void))(unsigned long long)date,        .description = "Prints the current date" },
    { .name = "clear",      .function = (int (*)(void))(unsigned long long)clear,       .description = "Clears the screen" },
    { .name = "help",       .function = (int (*)(void))(unsigned long long)help,        .description = "Prints the available commands" }
};

static uint64_t last_command_output = 0;

int main() {

    clear();
	while (1) {
        printf("\e[0mshell \e[0;32m$\e[0m ");

        char c;

        while(buffer_dim < MAX_BUFFER_SIZE && (c = getchar()) != '\n'){
            buffer[buffer_dim++] = c;
        }

        if(buffer_dim == MAX_BUFFER_SIZE){
            printf("\n\e[0;31mShell buffer overflow\e[0m\n");
            return 1;
        };

        buffer[buffer_dim] = 0;
        
        char * command = strtok(buffer, " ");
        int i = 0;

        for (; i < sizeof(commands) / sizeof(Command); i++) {
            if (strcmp(commands[i].name, command) == 0) {
                last_command_output = commands[i].function();
                break;
            }
        }

        // If the command is not found, ignore \n
        if (i == sizeof(commands) / sizeof(Command) && command != NULL && *command != '\0' ) {
            printf("\e[0;33mCommand not found\e[0m\n");
        }
    
        buffer[0] = buffer_dim = 0;
    }

	return one();
}

int date(void){
	int hour, minute, second;
    getDate(&hour, &minute, &second);
    printf("\n");
    return 0;
}

int echo(void){
    // printf(buffer + strlen("echo") + 1);
    for (int i = strlen("echo") + 1; i < buffer_dim; i++) {
        switch (buffer[i]) {
            case '\\':
                if (buffer[i + 1] == 'n') {
                    printf("\n");
                    i++;
                } else {
                    putchar(buffer[i]);
                }
                break;
            case '$':
                if (buffer[i + 1] == '?'){
                    putchar(last_command_output + '0');
                    i++;
                    break;
                }
            default:
                putchar(buffer[i]);
                break;
        }
    }
    printf("\n");
    return 0;
}

int help(void){
    printf("Available commands:\n");
    for (int i = 0; i < sizeof(commands) / sizeof(Command); i++) {
        printf(commands[i].name); printf("\t - - -\t "); printf(commands[i].description); printf("\n");
    }
    printf("\n");
    return 0;
}

int clear(void) {
    clearScreen();
    return 0;
}
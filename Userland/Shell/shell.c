#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <sys.h>
#include <exceptions.h>

#define MAX_BUFFER_SIZE 1024
#define HISTORY_SIZE 5

#define INC_MOD(x, m) x = (((x) + 1) % (m))
#define SUB_MOD(a, b, m) ((a) - (b) < 0 ? (m) - (b) + (a) : (a) - (b))
#define DEC_MOD(x, m) ((x) = SUB_MOD(x, 1, m))

static char buffer[MAX_BUFFER_SIZE];
static int buffer_dim = 0;

int clear(void);
int date(void);
int echo(void);
int exit(void);
int help(void);
int history(void);
int rectangle(void);

static void printPreviousCommand();
static void printNextCommand();

static uint8_t last_command_arrowed = 0;

typedef struct {
    char * name;
    int (*function)(void);
    char * description;
} Command;

/* All available commands. Sorted alphabetically by their name */
Command commands[] = {
    { .name = "clear",          .function = (int (*)(void))(unsigned long long)clear,           .description = "Clears the screen" },
    { .name = "date",           .function = (int (*)(void))(unsigned long long)date,            .description = "Prints the current date" },
    { .name = "divzero",        .function = (int (*)(void))(unsigned long long)_divzero,        .description = "Generates a division by zero exception" },
    { .name = "echo",           .function = (int (*)(void))(unsigned long long)echo ,           .description = "Prints the input string" },
    { .name = "exit",           .function = (int (*)(void))(unsigned long long)exit,            .description = "Exist with the provided exit code or zero (default)" },
    { .name = "help",           .function = (int (*)(void))(unsigned long long)help,            .description = "Prints the available commands" },
    { .name = "history",        .function = (int (*)(void))(unsigned long long)history,         .description = "Prints the command history" },
    { .name = "invalidopcode",  .function = (int (*)(void))(unsigned long long)_invalidopcode,  .description = "Generates an invalid Opcode exception" },
    { .name = "rectangle",      .function = (int (*)(void))(unsigned long long)rectangle,       .description = "Prints a blue rectangle" },
};

char command_history[HISTORY_SIZE][MAX_BUFFER_SIZE] = {0};
char command_history_buffer[MAX_BUFFER_SIZE] = {0};
uint8_t command_history_last = 0;

static uint64_t last_command_output = 0;

int main() {
    clear();

	while (1) {
        registerKey(KP_UP_KEY, printPreviousCommand);
        registerKey(KP_DOWN_KEY, printNextCommand);

        printf("\e[0mshell \e[0;32m$\e[0m ");

        char c;

        while(buffer_dim < MAX_BUFFER_SIZE && (c = getchar()) != '\n'){
            command_history_buffer[buffer_dim] = c;
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
                last_command_output = exec(commands[i].function);
                strncpy(command_history[command_history_last], command_history_buffer, 255);
                INC_MOD(command_history_last, HISTORY_SIZE);
                last_command_arrowed = command_history_last;
                break;
            }
        }

        // If the command is not found, ignore \n
        if (i == sizeof(commands) / sizeof(Command) && command != NULL && *command != '\0' ) {
            printf("\e[0;33mCommand not found\e[0m\n");
        }
    
        buffer[0] = buffer_dim = 0;
    }

    __builtin_unreachable();
    return 0;
}

static void printPreviousCommand(enum REGISTERABLE_KEYS scancode) {
    clearInputBuffer();
    last_command_arrowed = SUB_MOD(last_command_arrowed, 1, HISTORY_SIZE);
    if (command_history[last_command_arrowed][0] != 0) {
        fprintf(FD_STDIN, command_history[last_command_arrowed]);
    }
}

static void printNextCommand(enum REGISTERABLE_KEYS scancode) {
    clearInputBuffer();
    last_command_arrowed = (last_command_arrowed + 1) % HISTORY_SIZE;
    if (command_history[last_command_arrowed][0] != 0) {
        fprintf(FD_STDIN, command_history[last_command_arrowed]);
    }
}

int history(void) {
    uint8_t last = command_history_last;
    DEC_MOD(last, HISTORY_SIZE);
    uint8_t i = 0;
    while (i < HISTORY_SIZE && command_history[last][0] != 0) {
        putchar('0' + i); printf(". "); printf(command_history[last]); printf("\n");
        DEC_MOD(last, HISTORY_SIZE);
        i++;
    }
    return 0;
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

int rectangle(void){
    drawRectangle(0xFF, 16, 16);
    return 0;
}

// Only exit codes 0-9 are valid
int exit(void) {
    char * exit_code = strtok(NULL, " ");
    if (exit_code == NULL || *exit_code == '\0') {
        return 0;
    } else {
        return *exit_code - '0';
    }
}

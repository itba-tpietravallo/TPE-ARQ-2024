#include <string.h>

int strlen(char * str) {
    int i = 0;
    while (str[i] != 0) {
        i++;
    }
    return i;
}

int strcmp(char * str1, char * str2) {
    int i = 0;
    while (str1[i] != 0 && str2[i] != 0) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    return str1[i] - str2[i];
}

void strcpy(char * dest, char * src) {
    int i = 0;
    while (src[i] != 0) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
}
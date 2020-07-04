#include <stdint.h>
#include <stdbool.h>

void printf(const char* str) {

    uint16_t* ptr = (uint16_t*)0xB8000;

    while (*str) {

        *ptr = (*ptr & 0xFF00) | *str;
        str++;
        ptr++;
    }

}

void kernelMain(void) {

    printf("Hello, World!");

    while (true);

}
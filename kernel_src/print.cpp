#include "print.h"

static int x = 0;
static int y = 0;

void printf(const char* str) {

    uint16_t* ptr = (uint16_t*)0xB8000;
    while (*str) {

        printChar(*str);
        str++;
    }

}

void printChar(char c) {

    uint16_t* ptr = (uint16_t*)0xB8000;
    *(ptr + (80 * y) + x) = (*(ptr + (80 * y) + x) & 0xFF00) | c;
    
    x++;
    if (x == 80) {
        x = 0;
        y++;
    }
    if (y == 25) {
        x = 0;
        y = 0;
    }
}

void printLong(uint64_t num) {

    uint64_t reverse = 0;
    int numDigits = 0;

    if (num == 0) {
        printChar('0');
        return;
    }

    while (num) {
        int digit = num % 10;

        reverse += digit;
        reverse *= 10;
        numDigits++;
        num /= 10;
    }

    reverse /= 10;

    for (int i = 0; i < numDigits; ++i) {
        int digit = reverse % 10;
        printChar('0' + digit);
        reverse /= 10;
    }

    printChar(' ');
}
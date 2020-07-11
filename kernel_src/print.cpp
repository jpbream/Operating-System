#include "print.h"

static int x = 0;
static int y = 0;

static uint16_t defChar = 0x0700;

void printf(const char* str) {

    while (*str) {

        printChar(*str);
        str++;
    }

}

void printHex(uint64_t num)
{
    printf("0x");

    uint64_t mask = 0xF000000000000000;
    for ( int i = 0; i < 16; ++i ) {

        unsigned int digit = (num & mask) >> (60 - 4 * i);
        char output = ' ';

        if ( digit < 10 ) {
            output = '0' + digit;
        }
        else {

            output = 'A' + (digit - 10);
        }

        printChar(output);
        mask >>= 4;
    }
}

void printChar(char c)
{

    static uint16_t* ptr = (uint16_t*)0xB8000;

    if ( c == '\n' ) {
        y++;
        x = 0;
    }
    else {
        *(ptr + (80 * y) + x) = (*(ptr + (80 * y) + x) & 0xFF00) | c;
        x++;
    }
    
    
    if (x == 80) {
        x = 0;
        y++;
    }
    if (y == 25) {

        x = 0;
        y = 0;

        for ( int i = 0; i < 80 * 25; ++i ) {
            *(ptr + i) = defChar;
        }
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
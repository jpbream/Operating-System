#include <stdint.h>
#include <stdbool.h>

#include "gdt.h"

// make the gdt object visible to the assembler
// file so it can be loaded
extern "C" GDT gdt;

// create the gdt
GDT gdt;

static int cursor = 0;

void printf(const char* str) {

    uint16_t* ptr = (uint16_t*)0xB8000;

    while (*str) {

        *(ptr + cursor) = (*(ptr + cursor) & 0xFF00) | *str;
        str++;
        cursor++;
    }

}

void printChar(char c) {

    uint16_t* ptr = (uint16_t*)0xB8000;
    *(ptr + cursor) = (*(ptr + cursor) & 0xFF00) | c;
    cursor++;
}

void printLong(uint64_t num) {

    uint64_t reverse = 0;
    int numDigits = 0;

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

}

class A {
public:

    int x;
    A(int x) : x(x) {}

    ~A() {
        printf("bye");
    }
};

A a(5);

extern "C" void kernelMain(void) {

    printf("Hello, World! ");
    printLong(12345000);
    printChar(' ');
    printLong(gdt.code.descriptor);
    

while (true);

}
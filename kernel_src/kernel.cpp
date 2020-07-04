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

   // printf("Hello, World!");
   if (a.x == 5)
    printf("a is 5");

    //while (true);

}
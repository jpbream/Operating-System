#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdint.h>
#include <stdarg.h>

void printf(const char* seq, ...) __attribute__ ((format (printf, 1, 2)));;
void putchar(char c);

void clearScreen();

 
#endif
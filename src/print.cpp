#include "print.h"
#include "ps2_lookup.h"

static int x = 0;
static int y = 0;

static uint16_t defChar = 0x0700;

void printLong(int64_t num)
{

    uint64_t reverse = 0;
    int numDigits = 0;

    if ( num < 0 ) {
        putchar('-');
        num = -num;
    }

    if ( num == 0 ) {
        putchar('0');
        return;
    }

    while ( num ) {
        int digit = num % 10;

        reverse += digit;
        reverse *= 10;
        numDigits++;
        num /= 10;
    }

    reverse /= 10;

    for ( int i = 0; i < numDigits; ++i ) {
        int digit = reverse % 10;
        putchar('0' + digit);
        reverse /= 10;
    }

}

void printHex(int64_t num)
{
    printf("0x");

    if (num == 0) {
        putchar('0');
        putchar('0');
        return;
    }

    bool inLeadingZero = true;
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

        if (output != '0')
            inLeadingZero = false;

        if (!inLeadingZero)
            putchar(output);

        mask >>= 4;
    }
}

void printf(const char* seq, ...)
{

    va_list args;
    va_start(args, seq);

    while ( *seq ) {

        if ( *seq == '%' ) {

            seq++;
            char type = *seq;

            if ( type == 'd' ) {
               
                int num = va_arg(args, int);
                printLong(num);

            }
            else if ( type == 's' ) {

                const char* str = va_arg(args, const char*);
                while ( *str ) {
                    putchar(*str);
                    str++;
                }

            }
            else if ( type == 'c' ) {
                char c = va_arg(args, int);
                putchar(c);
            }
            else if ( type == 'x' ) {

                int num = va_arg(args, int);
                printHex(num);

            }

        }
        else {
            putchar(*seq);
        }

        seq++;
    }

    va_end(args);

}

void clearScreen()
{

    static uint16_t* ptr = (uint16_t*)0xB8000;

    x = 0;
    y = 0;

    for ( int i = 0; i < 80 * 25; ++i ) {
        *(ptr + i) = defChar;
    }
}

void putchar(char c)
{

    static uint16_t* ptr = (uint16_t*)0xB8000;

    if ( c == '\n' ) {
        y++;
        x = 0;
    }
    else if ( c == '\t' ) {

        putchar(' ');
        putchar(' ');
        putchar(' ');
        putchar(' ');

        return;
    }
    else if ( c == '\b' ) {

        x--;
        if ( x == -1 ) {
            x = 79;
            y--;

            if ( y == -1 ) {
                y = 0;
                x = 0;
            }
        }

        *(ptr + (80 * y) + x) = defChar;
       
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

        clearScreen();
    }
}
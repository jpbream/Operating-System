#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdint.h>

#define COLOR(R,G,B) (Color){B,G,R,255}
#define COLORA(R,G,B,A) (Color){B,G,R,A}

struct Color {
    union {
        struct {
            uint8_t b;
            uint8_t g;
            uint8_t r;
            uint8_t a;
        };
        uint32_t value;
    };
};

#endif
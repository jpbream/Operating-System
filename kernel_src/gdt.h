#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>

#define CODE_ACCESS 0b10011000
#define CODE_FLAGS ((0b0100) << 8)

#define DATA_ACCESS 0b10010010
#define DATA_FLAGS ((0b0100) << 8)

class GDT {

public:
    
    class GDT_Selector {

    public:
        uint64_t descriptor;

    public:
        GDT_Selector(uint32_t base, uint32_t limit, uint16_t flag);


    } __attribute__((__packed__));

    GDT_Selector null;
    GDT_Selector code;
    GDT_Selector data;

public:

    GDT();

};


#endif
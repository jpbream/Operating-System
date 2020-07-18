#ifndef _PS2_LOOKUP_H_
#define _PS2_LOOKUP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

	//bool IsKeyPress(uint8_t code);
	//bool IsKeyRelease(uint8_t code);
	unsigned char LookUpKey(uint8_t code, bool shiftDown);

#ifdef __cplusplus
}
#endif

#endif
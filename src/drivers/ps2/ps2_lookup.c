#include "ps2_lookup.h"

static unsigned char KEY_LOOKUP[] =
{
	// [0x01] = Escape,
	[0x02] = '1',
	[0x03] = '2',
	[0x04] = '3',
	[0x05] = '4',
	[0x06] = '5',
	[0x07] = '6',
	[0x08] = '7',
	[0x09] = '8',
	[0x0A] = '9',
	[0x0B] = '0',
	[0x0C] = '-',
	[0x0D] = '=',
	[0x0E] = '\b',
	[0x0F] = '\t',
	[0x10] = 'Q',
	[0x11] = 'W',
	[0x12] = 'E',
	[0x13] = 'R',
	[0x14] = 'T',
	[0x15] = 'Y',
	[0x16] = 'U',
	[0x17] = 'I',
	[0x18] = 'O',
	[0x19] = 'P',
	[0x1A] = '[',
	[0x1B] = ']',
	[0x1C] = '\n',
	// [0x1D] = left control,
	[0x1E] = 'A',
	[0x1F] = 'S',
	[0x20] = 'D',
	[0x21] = 'F',
	[0x22] = 'G',
	[0x23] = 'H',
	[0x24] = 'J',
	[0x25] = 'K',
	[0x26] = 'L',
	[0x27] = ';',
	[0x28] = '\'',
	[0x29] = '`',
	// [0x2A] = left shift,
	[0x2B] = '\\',
	[0x2C] = 'Z',
	[0x2D] = 'X',
	[0x2E] = 'C',
	[0x2F] = 'V',
	[0x30] = 'B',
	[0x31] = 'N',
	[0x32] = 'M',
	[0x33] = ',',
	[0x34] = '.',
	[0x35] = '/',
	// [0x36] = right shift
	// [0x37] = keypad *
	// [0x38] = left alt,
	[0x39] = ' ',
	// [0x3A] = caps lock,
	// [0x3B] = F1
	// [0x3C] = F2,
	// [0x3D] = F3,
	// [0x3E] = F4,
	// [0x3F] = F5,
	// [0x40] = F6,
	// [0x41] = F7,
	// [0x42] = F8,
	// [0x43] = F9,
	// [0x44] = F10,
	// [0x45] = numlock,
	// [0x46] = scrolllock
	// [0x47] = keypad 7
	// [0x48] = keypad 8,
	// [0x49] = keypad 9
	// [0x4A] = keypad -
	// [0x4B] = keypad 4
	// [0x4C] = keypad 5,
	// [0x4D] = keypad 6
	// [0x4E] = keypad +
	// [0x4F] = keypad 1
	// [0x50] = keypad 2,
	// [0x51] = keypad 3
	// [0x52] = keypad 0
	// [0x53] = keypad .

	// [0x57] = F11
	// [0x58] = F12,
};

unsigned char LookUpKey(uint8_t code, bool shiftDown)
{
	return KEY_LOOKUP[code];
}

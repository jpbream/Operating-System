#include "keyboard.h"
#include "print.h"
#include "ps2_lookup.h"

#define KBD_COMMAND 0x64
#define KBD_DATA 0x60

#define ENABLE_KBD 0xAE
#define DISABLE_KBD 0xAD

Keyboard::Keyboard()
	:
	data(KBD_DATA), command(KBD_COMMAND)
{
	command.Write8(DISABLE_KBD);

	while ( command.Read8() & 0x01 ) {
		data.Read8();
	}

	command.Write8(ENABLE_KBD);

	command.Write8(0x20);
	uint8_t status = (data.Read8() | 1) & 0b01110111;
	command.Write8(0x60);
	data.Write8(status);

	data.Write8(0xF4);
}

void Keyboard::Handle(uint8_t interrupt)
{
	uint8_t key = data.Read8();
	
	if ( key < 0x81 ) {
		
		printChar(LookUpKeyPress(key));

	}
	else {
		// key released
	}
}
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

	// enable keyboard interface
	WritePS2Command(0xAE);

	// read command byte
	WritePS2Command(0x20);
	// turn on the send interrupts flag and turn off the don't enable flag
	uint8_t status = (ReadData() | 1) & ~0x10;

	// write command byte
	WritePS2Command(0x60);
	WriteData(status);

	// another enable command
	WriteKeyboardCommand(0xF4);
	ReadData();

	// turn off led's
	WriteKeyboardCommand(0xED);
	WriteData(0);
	ReadData();

	while ( command.Read8() & 1 )
		data.Read8();
}

void Keyboard::WritePS2Command(uint8_t command)
{
	// wait for input to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( (Keyboard::command.Read8() & 0x02) == 0 ) {

			Keyboard::command.Write8(command);
			return;

		}
	}
}

void Keyboard::WriteKeyboardCommand(uint8_t command)
{
	// wait for keyboard input to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( (Keyboard::command.Read8() & 0x02) == 0 ) {

			Keyboard::data.Write8(command);
			return;

		}
	}
}

void Keyboard::WriteData(uint8_t data)
{
	// wait for keyboard input to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( (command.Read8() & 0x02) == 0 ) {

			Keyboard::data.Write8(data);
			return;

		}
	}
}
uint8_t Keyboard::ReadData()
{
	// wait for keyboard output to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( command.Read8() & 0x01 ) {

			return data.Read8();

		}
	}
}

void Keyboard::Handle(uint8_t interrupt)
{
	uint8_t key = data.Read8();
	
	if ( key < 0x81 ) {
		
		putchar(LookUpKey(key, false));

	}
	else {
		// key released
	}
}
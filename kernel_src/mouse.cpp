#include "mouse.h"
#include "print.h"

#define MOUSE_COMMAND 0x64
#define MOUSE_DATA 0x60

static uint16_t* screen = (uint16_t*)0xB8000;
#define WIDTH 80
#define HEIGHT 25

#define PACKET_START 0

static void FlipColor(double sx, double sy)
{
	int x = (int)(sx * WIDTH);
	int y = (int)(sy * HEIGHT);

	uint16_t lower = *(screen + y * WIDTH + x) & 0x0F00;
	uint16_t upper = *(screen + y * WIDTH + x) & 0xF000;

	uint16_t newUpper = lower << 4;
	uint16_t newLower = upper >> 4;

	*(screen + y * WIDTH + x) &= 0x00FF;
	*(screen + y * WIDTH + x) |= (newUpper | newLower);

}

Mouse::Mouse()
	:
	command(MOUSE_COMMAND), data(MOUSE_DATA)
{
	x = 0.5;
	y = 0.5;

	FlipColor(x, y);

	// enable mouse device
	WritePS2Command(0xA8);

	// query command register
	WritePS2Command(0x20);
	// enable interrupt 12, disable don't enable flag
	uint8_t status = (ReadData() | 0x02) & ~0x20;
	
	// set command register
	WritePS2Command(0x60);
	// write command value back
	WriteData(status);

	WriteMouseCommand(0xF6);
	ReadData();

	// enable packet streaming
	WriteMouseCommand(0xF4);
	ReadData();

	// read up any data still in the buffer
	while ( command.Read8() & 0x20 )
		data.Read8();

}

void Mouse::WritePS2Command(uint8_t command)
{
	// wait for mouse input to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( (Mouse::command.Read8() & 0x02) == 0 ) {
			
			Mouse::command.Write8(command);
			return;

		}
	}
}

void Mouse::WriteMouseCommand(uint8_t command)
{
	// wait for mouse input to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( (Mouse::command.Read8() & 0x02) == 0 ) {

			// tell ps2 the parameter is a command
			// for the mouse using D4
			Mouse::command.Write8(0xD4);
			break;

		}
	}

	timeout = 100000;
	while ( timeout-- ) {
		if ( (Mouse::command.Read8() & 0x02) == 0 ) {

			data.Write8(command);
			return;

		}
	}
}

void Mouse::WriteData(uint8_t data)
{
	// wait for mouse input to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( (command.Read8() & 0x02) == 0 ) {
			
			Mouse::data.Write8(data);
			return;

		}
	}
}
uint8_t Mouse::ReadData()
{
	// wait for mouse output to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( command.Read8() & 0x01 ) {
			
			return data.Read8();

		}
	}
}

void Mouse::Handle(uint8_t interrupt)
{

	static int8_t packet[3];
	static int byte = PACKET_START;

	uint8_t status = command.Read8();
	if ( !(status & 0x20) ) {
		return;
	}
	
	packet[byte] = ReadData();

	byte = (byte + 1) % 3;

	if ( byte == 2 ) {

		FlipColor(x, y);

		x += packet[1] / 1000.0;
		y -= (WIDTH * packet[2] / (1000.0 * HEIGHT));

		if ( x < 0 )
			x = 0;
		if ( x >= 1 )
			x = 0.999;
		if ( y < 0 )
			y = 0;
		if ( y >= 1 )
			y = 0.999;

		FlipColor(x, y);

		if ( packet[0] & 1 ) {
			//printf("Left Click. ");
		}
		if ( packet[0] & 2 ) {
			//printf("Right Click. ");
		}
		if ( packet[0] & 4 ) {
			//printf("Middle Click. ");
		}

	}

}
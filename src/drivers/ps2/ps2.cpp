#include "ps2.h"
#include "port.h"

#define PS2_COMMAND_PORT 0x64
#define PS2_DATA_PORT 0x60

static Port command(PS2_COMMAND_PORT);
static Port data(PS2_DATA_PORT);

// the status byte can be querired with command.Read8()

// bit that indicates whether the ps2 output buffer is full
#define STATUS_OBF 0x01

// bit that indicates whether the ps2 input buffer is full
#define STATUS_IBF 0x02

// bit that indicates whether the ps2 mouse output buffer is full
#define STATUS_MOBF 0x20

// the next command should go to the mouse
#define PS2_COMMAND_MOUSE 0xD4

static void WaitToWrite()
{
	// wait for input to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( (command.Read8() & STATUS_IBF) == 0 ) {
			return;
		}
	}
}

void PS2::WriteCommand(uint8_t cmd)
{
	WaitToWrite();
	command.Write8(cmd);
}
void PS2::WriteKeyboardCommand(uint8_t cmd)
{
	// keyboard commands are still sent to the data port
	WaitToWrite();
	data.Write8(cmd);
}
void PS2::WriteMouseCommand(uint8_t cmd)
{
	WriteCommand(PS2_COMMAND_MOUSE);

	// mouse commands are still sent to the data port
	WaitToWrite();
	data.Write8(cmd);
}

void PS2::WriteData(uint8_t dt)
{
	WaitToWrite();
	data.Write8(dt);
}

void PS2::WriteKeyboardData(uint8_t dt)
{
	WaitToWrite();
	data.Write8(dt);
}

void PS2::WriteMouseData(uint8_t dt)
{
	WriteCommand(PS2_COMMAND_MOUSE);

	WaitToWrite();
	data.Write8(dt);
}

uint8_t PS2::ReadData()
{
	// wait for output buffer to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( command.Read8() & STATUS_OBF ) {

			return data.Read8();

		}
	}
	return PS2_TIMEOUT;
}
uint8_t PS2::ReadKeyboardData()
{
	// wait for keyboard output to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( command.Read8() & STATUS_OBF ) {

			return data.Read8();

		}
	}
	return PS2_TIMEOUT;
}
uint8_t PS2::ReadMouseData()
{
	// wait for mouse output to be empty
	int timeout = 100000;
	while ( timeout-- ) {
		if ( command.Read8() & STATUS_OBF ) {

			return data.Read8();

		}
	}
	return PS2_TIMEOUT;
}

void PS2::ClearKeyboardOutput()
{
	while ( command.Read8() & STATUS_OBF )
		data.Read8();
}
void PS2::ClearMouseOutput()
{
	while ( command.Read8() & STATUS_MOBF )
		data.Read8();
}
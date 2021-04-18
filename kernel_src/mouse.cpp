#include "mouse.h"
#include "print.h"
#include "ps2.h"

#define PACKET_START 0

ConsoleMouseHandler::ConsoleMouseHandler()
{
	x = 0.5;
	y = 0.5;

	FlipColorAtCursor();
}

void ConsoleMouseHandler::FlipColorAtCursor()
{
	int xCoord = (int)(x * width);
	int yCoord = (int)(y * height);

	uint16_t lower = *(screen + yCoord * width + xCoord) & 0x0F00;
	uint16_t upper = *(screen + yCoord * width + xCoord) & 0xF000;

	uint16_t newUpper = lower << 4;
	uint16_t newLower = upper >> 4;

	*(screen + yCoord * width + xCoord) &= 0x00FF;
	*(screen + yCoord * width + xCoord) |= (newUpper | newLower);
}

void ConsoleMouseHandler::OnMouseMove(double dx, double dy)
{
	FlipColorAtCursor();

	x += dx;
	y -= width * dy / height;

	if ( x < 0 )
		x = 0;
	if ( x >= 1 )
		x = 0.999;
	if ( y < 0 )
		y = 0;
	if ( y >= 1 )
		y = 0.999;

	FlipColorAtCursor();
}

void ConsoleMouseHandler::OnMouseDown(int button)
{
	printf("%d", button);
}

int ConsoleMouseHandler::GetX()
{
	return x * width;
}

int ConsoleMouseHandler::GetY()
{
	return y * height;
}



Mouse::Mouse(IDT* idt)
{
	idt->SetHandler(0x2C, this);
}

void Mouse::Activate()
{
	// test the mouse
	PS2::WriteCommand(PS2_TEST_MOUSE);
	if ( PS2::ReadData() != PS2_TEST_MOUSE_PASSED ) {
		printf("The Mouse Test Failed.\n");
		while (true);
	}

	// enable mouse interface
	PS2::WriteCommand(PS2_ENABLE_MOUSE);

	// query command register
	PS2::WriteCommand(PS2_READ_CC);
	// enable interrupt 12, disable don't enable flag
	uint8_t status = (PS2::ReadData() | CC_ENABLE_INT12) & ~CC_DISABLE_MOUSE;

	// set command register
	PS2::WriteCommand(PS2_WRITE_CC);
	// write command value back
	PS2::WriteData(status);

	// mouse POST
	PS2::WriteMouseCommand(MS_RESET);
	PS2::ReadMouseData();

	// on the laptop, the POST can actually take a second
	uint8_t testResult;
	do {
		testResult = PS2::ReadMouseData();
	} while ( testResult == PS2_TIMEOUT );
	
	if ( testResult != MS_RESET_PASSED ) {
		printf("The Mouse POST Failed.\n");
		while (true);
	}
	PS2::ReadMouseData();

	//////////////////////////////////////////////////////////////////////////////////////////

	// the synaptics touchpad in my laptop does its own thing
	// for setting the sample rate, it can only be 40 or 80
	// it has something called a mode byte. Setting bit 6 of this
	// byte will make the packet rate 80 per second.

	// 4 set resolutions in a row gets the touchpads attention
	// the parameters for them form the parameter passed to the touchpad

	// we want to query the mode bit, so we need to send a Status Request
	// after the 4 set resolutions, with an encoded parameter of 0x01

	PS2::WriteMouseCommand(MS_SET_RESOLUTION);
	PS2::ReadMouseData();
	PS2::WriteMouseData(0); // encoding the parameter
	PS2::ReadMouseData();
	PS2::WriteMouseCommand(MS_SET_RESOLUTION);
	PS2::ReadMouseData();
	PS2::WriteMouseData(0); // encoding the parameter
	PS2::ReadMouseData();
	PS2::WriteMouseCommand(MS_SET_RESOLUTION);
	PS2::ReadMouseData();
	PS2::WriteMouseData(0); // encoding the parameter
	PS2::ReadMouseData();
	PS2::WriteMouseCommand(MS_SET_RESOLUTION);
	PS2::ReadMouseData();
	PS2::WriteMouseData(1); // encoding the parameter
	PS2::ReadMouseData();

	PS2::WriteMouseCommand(MS_READ_STATUS);
	PS2::ReadMouseData();

	// the third byte of the response is the mode byte
	PS2::ReadMouseData();
	PS2::ReadMouseData();
	uint8_t modeByte = PS2::ReadMouseData();

	modeByte |= 64;

	// the final command to write the mode byte is
	// set sample rate : 20
	// the mode byte is the encoded argument

	PS2::WriteMouseCommand(MS_SET_RESOLUTION);
	PS2::ReadMouseData();
	PS2::WriteMouseData((modeByte & 0xC0u) >> 6); // encoding the parameter
	PS2::ReadMouseData();
	PS2::WriteMouseCommand(MS_SET_RESOLUTION);
	PS2::ReadMouseData();
	PS2::WriteMouseData((modeByte & 0x30u) >> 4); // encoding the parameter
	PS2::ReadMouseData();
	PS2::WriteMouseCommand(MS_SET_RESOLUTION);
	PS2::ReadMouseData();
	PS2::WriteMouseData((modeByte & 0x0Cu) >> 2); // encoding the parameter
	PS2::ReadMouseData();
	PS2::WriteMouseCommand(MS_SET_RESOLUTION);
	PS2::ReadMouseData();
	PS2::WriteMouseData((modeByte & 0x03u) >> 0); // encoding the parameter
	PS2::ReadMouseData();

	PS2::WriteMouseCommand(MS_SET_SAMPLE_RATE);
	PS2::ReadMouseData();
	PS2::WriteMouseData(0x14);
	PS2::ReadMouseData();

	//////////////////////////////////////////////////////////////////////////////////////////



	// enable mouse packets
	PS2::WriteMouseCommand(MS_ENABLE_STREAMING);
	PS2::ReadMouseData();

	// enable keyboard scanning now
	PS2::WriteKeyboardCommand(KBD_ENABLE_SCANNING);
	PS2::ReadKeyboardData();

	PS2::ClearKeyboardOutput();

	// read up any data still in the buffer
	PS2::ClearMouseOutput();
}

void Mouse::Handle(uint8_t interrupt)
{

	static int8_t packet[3];
	static int byte = PACKET_START;
	
	packet[byte] = PS2::ReadMouseData();

	byte = (byte + 1) % 3;

	if ( byte == 2 ) {

		double dx = packet[1] / 1000.0;
		double dy = packet[2] / 1000.0;

		if (handler) {
			handler->OnMouseMove(dx, dy);

			if ( packet[0] & 1 ) {
				handler->OnMouseDown(0);
			}
			if ( packet[0] & 2 ) {
				handler->OnMouseDown(1);
			}
			if ( packet[0] & 4 ) {
				handler->OnMouseDown(2);
			}
		}
	}
}

void Mouse::SetEventHandler(MouseEventHandler* handler)
{
	this->handler = handler;
}

void MouseEventHandler::OnMouseDown(int button) {}
void MouseEventHandler::OnMouseUp(int button) {}
void MouseEventHandler::OnMouseMove(double x, double y) {}
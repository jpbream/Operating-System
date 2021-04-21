#include "ps2_mouse.h"
#include "ps2.h"
#include "print.h"

#define PACKET_START 0

PS2Mouse::PS2Mouse(IDT* idt)
{
	idt->SetHandler(0x2C, this);
}

void PS2Mouse::Activate()
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

#ifdef LAPTOP
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
#endif


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

void PS2Mouse::Handle(uint8_t interrupt)
{
	static bool mouseButtons[3] = {false, false, false};

	static int8_t packet[3];
	static int byte = PACKET_START;
	
	packet[byte] = PS2::ReadMouseData();

	byte = (byte + 1) % 3;

	if ( byte == 2 ) {

		// if the x or y overflow bits are set, just ignore the packet
		// virtualbox doesn't do a great job of reporting these correctly,
		// so don't move the mouse too fast in virtualbox
		if (packet[0] & 0x80 || packet[0] & 0x40) {
			return;
		}

		// properly sign extend the x and y movements
		uint32_t rawx = (packet[0] & 0x10) ? (0xFFFFFF00 | (uint32_t)packet[1]) : (uint32_t)packet[1];
		uint32_t rawy = (packet[0] & 0x20) ? (0xFFFFFF00 | (uint32_t)packet[2]) : (uint32_t)packet[2];

		int dx = *(int*)&rawx;
		int dy = *(int*)&rawy;

		// flip to screen space
		dy = -dy;

#ifdef VIRTUALBOX
		// something happens in virtualbox that makes the sign bit sometimes not correct
		if (dx > 250 || dy > 250) {
			return;
		}
#endif

		if (handler) {
			handler->OnMouseMove(dx, dy);

			if ( packet[0] & 1 && !mouseButtons[0]) {
				handler->OnMouseDown(0);
			}
			else if ((packet[0] & 1) == 0 && mouseButtons[0]) {
				handler->OnMouseUp(0);
			}

			if ( packet[0] & 2 && !mouseButtons[1]) {
				handler->OnMouseDown(1);
			}
			else if ((packet[0] & 2) == 0 && mouseButtons[1]) {
				handler->OnMouseUp(1);
			}

			if ( packet[0] & 4 && !mouseButtons[2]) {
				handler->OnMouseDown(2);
			}
			else if ((packet[0] & 4) == 0 && mouseButtons[2]) {
				handler->OnMouseUp(2);
			}

			mouseButtons[0] = (packet[0] & 1) > 0;
			mouseButtons[1] = (packet[0] & 2) > 0;
			mouseButtons[2] = (packet[0] & 4) > 0;
		}
	}
}

void PS2Mouse::SetEventHandler(MouseEventHandler* handler)
{
	this->handler = handler;
}
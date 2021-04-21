#include "keyboard.h"
#include "print.h"
#include "ps2_lookup.h"
#include "ps2.h"

Keyboard::Keyboard(IDT* idt)
{ 
	idt->SetHandler(0x21, this);
}

void Keyboard::Activate()
{
	// test the ps2
	PS2::WriteCommand(PS2_TEST_CONTROLLER);
	if ( PS2::ReadData() != PS2_TEST_PASSED ) {
		printf("The PS2 Test Failed.\n");
		while (true);
	}

	// test the keyboard
	PS2::WriteCommand(PS2_TEST_KBD);
	if ( PS2::ReadData() != PS2_TEST_KBD_PASSED ) {
		printf("The Keyboard Test Failed.\n");
		while (true);
	}

	// enable keyboard interface
	PS2::WriteCommand(PS2_ENABLE_KBD);

	// read command byte
	PS2::WriteCommand(PS2_READ_CC);
	// turn on the send interrupts flag and turn off the don't enable flag
	uint8_t status = (PS2::ReadData() | CC_ENABLE_INT1) & ~CC_DISABLE_KBD;

	// write command byte
	PS2::WriteCommand(PS2_WRITE_CC);
	PS2::WriteData(status);

	// keyboard POST
	PS2::WriteKeyboardCommand(KBD_RESET);
	PS2::ReadKeyboardData();

	uint8_t testResult;
	do {
		testResult = PS2::ReadKeyboardData();
	} while ( testResult == PS2_TIMEOUT );

	if ( testResult != KBD_RESET_PASSED ) {
		printf("The Keyboard POST Failed.\n");
		while (true);
	}

	// turn off led's
	PS2::WriteKeyboardCommand(KBD_SET_LED);
	PS2::ReadKeyboardData();
	PS2::WriteKeyboardData(0);
	PS2::ReadKeyboardData();

	// set rates
	PS2::WriteKeyboardCommand(KBD_SET_RATE);
	PS2::ReadKeyboardData();
	PS2::WriteKeyboardData((0) & KBD_REPEAT_MASK | KBD_DELAY_500);
	PS2::ReadKeyboardData();
}

void Keyboard::Handle(uint8_t interrupt)
{
	uint8_t key = PS2::ReadKeyboardData();
	
	if (handler) {
		if ( key < 0x81 ) {
			handler->OnKeyDown(LookUpKey(key, false));
		}
		else {
			handler->OnKeyUp(LookUpKey(key, false));
		}
	}
}

void Keyboard::SetEventHandler(KeyboardEventHandler* handler)
{
	this->handler = handler;
}


void KeyboardEventHandler::OnKeyDown(char key) {}
void KeyboardEventHandler::OnKeyUp(char key) {}

void ConsoleKeyboardHandler::OnKeyDown(char key)
{
	putchar(key);
}
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "interrupt_handler.h"
#include "port.h"

class Keyboard : public InterruptHandler {

private:
	Port data;
	Port command;

	void WritePS2Command(uint8_t command);
	void WriteKeyboardCommand(uint8_t command);
	void WriteData(uint8_t data);
	uint8_t ReadData();

public:

	Keyboard();
	void Handle(uint8_t interrupt) override;
};

#endif
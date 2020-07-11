#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "interrupt_handler.h"
#include "port.h"

class Keyboard : public InterruptHandler {

private:
	Port data;
	Port command;

public:

	Keyboard();
	void Handle(uint8_t interrupt) override;
};

#endif
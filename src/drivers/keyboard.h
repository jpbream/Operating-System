#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "interrupt_handler.h"
#include "driver.h"
#include "port.h"
#include "idt.h"

class KeyboardEventHandler
{
public:
	virtual void OnKeyDown(char);
	virtual void OnKeyUp(char);
};

class ConsoleKeyboardHandler : public KeyboardEventHandler
{
	void OnKeyDown(char) override;
};

class Keyboard : public InterruptHandler, public Driver 
{
private:
	KeyboardEventHandler* handler;

public:

	Keyboard(IDT* idt);
	void Handle(uint8_t interrupt) override;
	void Activate() override;
	void SetEventHandler(KeyboardEventHandler* handler);
};

#endif
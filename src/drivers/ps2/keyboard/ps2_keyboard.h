#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "keyboard_handler.h"
#include "interrupt_handler.h"
#include "driver.h"
#include "port.h"
#include "idt.h"
#include "cpu_state.h"

class PS2Keyboard : public InterruptHandler, public Driver 
{
private:
	KeyboardEventHandler* handler;

public:

	PS2Keyboard(IDT* idt);
	CPUState* Handle(uint8_t interrupt, CPUState* regs) override;
	void Activate() override;
	void SetEventHandler(KeyboardEventHandler* handler);
};

#endif
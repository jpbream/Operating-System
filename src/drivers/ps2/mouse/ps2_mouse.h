#ifndef _PS2_MOUSE_H_
#define _PS2_MOUSE_H_

#include "interrupt_handler.h"
#include "port.h"
#include "idt.h"
#include "driver.h"
#include "mouse_handler.h"
#include <stdint.h>

class PS2Mouse : public InterruptHandler, public Driver {

private:

	MouseEventHandler* handler;

public:

	PS2Mouse(IDT* idt);
	CPUState* Handle(uint8_t interrupt, CPUState* regs) override;
	void Activate() override;

	void SetEventHandler(MouseEventHandler* handler);

};

#endif
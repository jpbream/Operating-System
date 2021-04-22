#ifndef _INTERRUPT_HANDLER_H_
#define _INTERRUPT_HANDLER_H_

#include <stdint.h>
#include "cpu_state.h"

class InterruptHandler {

public:
	virtual CPUState* Handle(uint8_t interrupt, CPUState* regs) = 0;

};

class DefaultExceptionHandler : public InterruptHandler {
public:
	CPUState* Handle(uint8_t interrupt, CPUState* regs) override;
};

class DefaultInterruptHandler : public InterruptHandler {
public:
	CPUState* Handle(uint8_t interrupt, CPUState* regs) override;
};

#endif
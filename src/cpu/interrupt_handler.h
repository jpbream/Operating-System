#ifndef _INTERRUPT_HANDLER_H_
#define _INTERRUPT_HANDLER_H_

#include <stdint.h>

class InterruptHandler {

public:
	virtual void Handle(uint8_t interrupt) = 0;

};

class DefaultExceptionHandler : public InterruptHandler {
public:
	void Handle(uint8_t interrupt) override;
};

class DefaultInterruptHandler : public InterruptHandler {
public:
	void Handle(uint8_t interrupt) override;
};

#endif
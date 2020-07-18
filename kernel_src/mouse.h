#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "interrupt_handler.h"
#include "port.h"
#include <stdint.h>

class Mouse : public InterruptHandler {

private:
	double x;
	double y;

	Port data;
	Port command;

	void WritePS2Command(uint8_t command);
	void WriteMouseCommand(uint8_t command);
	void WriteData(uint8_t data);
	uint8_t ReadData();

public:

	Mouse();
	void Handle(uint8_t interrupt) override;

};

#endif
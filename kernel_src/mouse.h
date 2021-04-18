#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "interrupt_handler.h"
#include "port.h"
#include "idt.h"
#include "driver.h"
#include <stdint.h>

class MouseEventHandler
{
public:
	virtual void OnMouseMove(double dx, double dy);
	virtual void OnMouseDown(int button);
	virtual void OnMouseUp(int button);
};

class ConsoleMouseHandler : public MouseEventHandler
{
private:
	double x;
	double y;

	const int width = 80;
	const int height = 25;

	uint16_t* screen = (uint16_t*)0xB8000;

	void FlipColorAtCursor();

public:
	ConsoleMouseHandler();

	void OnMouseMove(double dx, double dy) override;
	void OnMouseDown(int button) override;

	int GetX();
	int GetY();
};

class Mouse : public InterruptHandler, public Driver {

private:

	MouseEventHandler* handler;

public:

	Mouse(IDT* idt);
	void Handle(uint8_t interrupt) override;
	void Activate() override;

	void SetEventHandler(MouseEventHandler* handler);

};

#endif
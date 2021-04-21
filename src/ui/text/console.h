#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "mouse_handler.h"
#include "keyboard_handler.h"
#include <stdint.h>

class Console : public MouseEventHandler, public KeyboardEventHandler
{
private:
	double x;
	double y;

	const int width = 80;
	const int height = 25;

	uint16_t* screen = (uint16_t*)0xB8000;

	void FlipColorAtCursor();

public:
	Console();

	void OnMouseUp(int button) override;
	void OnMouseMove(double dx, double dy) override;
	void OnMouseDown(int button) override;
    void OnKeyDown(char key) override;
	void OnKeyUp(char) override;
};

#endif
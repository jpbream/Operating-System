#ifndef _KEYBOARD_HANDLER_H_
#define _KEYBOARD_HANDLER_H_

class KeyboardEventHandler
{
public:
	virtual void OnKeyDown(char) = 0;
	virtual void OnKeyUp(char) = 0;
};

#endif
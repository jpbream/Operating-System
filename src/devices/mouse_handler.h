#ifndef _MOUSE_HANDLER_H_
#define _MOUSE_HANDLER_H_

class MouseEventHandler
{
public:
	virtual void OnMouseMove(double dx, double dy) = 0;
	virtual void OnMouseDown(int button) = 0;
	virtual void OnMouseUp(int button) = 0;
};

#endif
#ifndef _DESKTOP_H_
#define _DESKTOP_H_

#include "widget.h"
#include "mouse_handler.h"
#include "color.h"

class Desktop : public CompositeWidget, public MouseEventHandler
{
private:
	double mouseX;
	double mouseY;

public:
    Desktop(int width, int height, Color color);

    void OnMouseMove(double dx, double dy) override;
	void OnMouseDown(int button) override;
    void OnMouseUp(int button) override;

    void Draw(GraphicsContext* gfx) override;
};

#endif
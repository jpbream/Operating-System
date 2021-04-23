#include "desktop.h"

Desktop::Desktop(int width, int height, Color color)
: CompositeWidget(0, 0, 0, width, height, color)
{
    mouseX = width / 2.0;
    mouseY = height / 2.0;
}
void Desktop::Draw(GraphicsContext* gfx)
{
    CompositeWidget::Draw(gfx);
    gfx->FillRect(mouseX, mouseY, width / 250, width / 250, (Color){255, 255, 255, 255}); 
}
void Desktop::OnMouseMove(double dx, double dy)
{ 
    #ifdef VIRTUALBOX
	mouseX += dx / 10;
	mouseY += dy / 10;
    #else
    mouseX += dx;
	mouseY += dy;
    #endif

    if (mouseX < 0) mouseX = 0;
    if (mouseY < 0) mouseY = 0;
    if (mouseX >= width) mouseX = width - 1;
    if (mouseY >= height) mouseY = height - 1;

	CompositeWidget::OnMouseMove(mouseX, mouseY);
}
void Desktop::OnMouseDown(int button)
{
    CompositeWidget::OnMouseDown(mouseX, mouseY, button);
}
void Desktop::OnMouseUp(int button)
{
    CompositeWidget::OnMouseUp(mouseX, mouseY, button);
}
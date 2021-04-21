#include "desktop.h"

Desktop::Desktop(int width, int height, char color)
: CompositeWidget(0, 0, 0, width, height, color)
{
    mouseX = width / 2.0;
    mouseY = height / 2.0;
}
void Desktop::Draw(GraphicsContext* gfx)
{
    gfx->FillScreen(color);
    gfx->FillRect(mouseX, mouseY, 5, 5, 0x3F);
}
void Desktop::OnMouseMove(double dx, double dy)
{
	mouseX += dx * 1000;
	mouseY -= dy * 1000;

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
#include "window.h"

Window::Window(Desktop* parent, int x, int y, int w, int h, char c)
    : CompositeWidget(parent, x, y, w, h, c)
{
}

void Window::OnMouseDown(int x, int y, int button)
{
    CompositeWidget::OnMouseDown(x, y, button);
    mouseDown = true;
}
void Window::OnMouseUp(int x, int y, int button)
{
    CompositeWidget::OnMouseUp(x, y, button);
    mouseDown = false;
}
void Window::OnMouseMove(int x, int y)
{
    CompositeWidget::OnMouseMove(x, y);
    if (mouseDown) {
        this->x = x - width / 2;
        this->y = y - height / 2;
    }
}

void Window::OnKeyDown(char key)
{
    CompositeWidget::OnKeyDown(key);
}
void Window::OnKeyUp(char key)
{
    CompositeWidget::OnKeyUp(key);
}
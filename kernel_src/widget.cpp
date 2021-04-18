#include "widget.h"

Widget::Widget(Widget* parent, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b)
    : parent(parent), x(x), y(y), width(w), height(h), r(r), g(g), b(b)
{

}
Widget::~Widget()
{

}

void Widget::GetFocus(Widget* widget)
{
    if (parent) {
        parent->GetFocus(widget);
    }
}
void Widget::ModelToScreen(int& x, int& y)
{
    if (parent) {
        parent->ModelToScreen(x, y);
    }

    x += this->x;
    y += this->y;
}

void Widget::Draw(GraphicsContext* gc)
{
    int absX = 0;
    int absY = 0;
    ModelToScreen(x, y);
    gc->FillRect(absX, absY, width, height, 1);
}
void Widget::OnMouseDown(int x, int y)
{
    GetFocus(this);
}
void Widget::OnMouseUp(int x, int y)
{

}
void Widget::OnMouseMove(int x, int y)
{

}

void Widget::OnKeyDown(char key)
{

}
void Widget::OnKeyUp(char key)
{

}


CompositeWidget::CompositeWidget(Widget* parent, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b)
    : Widget(parent, x, y, w, h, r, g, b)
{
    focusedChild = 0;
    numChildren = 0;
}
CompositeWidget::~CompositeWidget()
{

}

void CompositeWidget::GetFocus(Widget* widget)
{
    this->focusedChild = widget;
    if (parent) {
        parent->GetFocus(widget);
    }
}

void CompositeWidget::Draw(GraphicsContext* gc)
{
    Widget::Draw(gc);

    for (int i = 0; i < numChildren; ++i) {
        children[i]->Draw(gc);
    }
}
void CompositeWidget::OnKeyDown(char key)
{
    if (focusedChild) {
        focusedChild->OnKeyDown(key);
    }
}
void CompositeWidget::OnKeyUp(char key)
{
    if (focusedChild) {
        focusedChild->OnKeyUp(key);
    }
}

void CompositeWidget::OnMouseDown(int x, int y)
{
    for (int i = numChildren - 1; i >= 0; ++i) {
        if (children[i]->ContainsPoint(x, y)) {
            children[i]->OnMouseDown(x, y);
            break;
        }
    }
}
void CompositeWidget::OnMouseUp(int x, int y)
{
    for (int i = numChildren - 1; i >= 0; ++i) {
        if (children[i]->ContainsPoint(x, y)) {
            children[i]->OnMouseUp(x, y);
            break;
        }
    }
}
void CompositeWidget::OnMouseMove(int x, int y)
{
    for (int i = numChildren - 1; i >= 0; ++i) {
        if (children[i]->ContainsPoint(x, y)) {
            children[i]->OnMouseMove(x, y);
            break;
        }
    }
}
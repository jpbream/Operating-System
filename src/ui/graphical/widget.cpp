#include "widget.h"

Widget::Widget(Widget* parent, int x, int y, int w, int h, Color color)
    : parent(parent), x(x), y(y), width(w), height(h), color(color)
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

bool Widget::ContainsPoint(int x, int y)
{
    int baseX = this->x;
    int baseY = this->y;

    return x >= baseX && x <= baseX + width && y >= baseY && y <= baseY + height;
}

void Widget::Draw(GraphicsContext* gc)
{
    int absX = 0;
    int absY = 0;
    ModelToScreen(absX, absY);
    gc->FillRect(absX, absY, width, height, color);
}
void Widget::OnMouseDown(int x, int y, int button)
{
    GetFocus(this);
}

CompositeWidget::CompositeWidget(Widget* parent, int x, int y, int w, int h, Color color)
    : Widget(parent, x, y, w, h, color)
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
void CompositeWidget::AddChild(Widget* child)
{
    children[numChildren++] = child;
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

void CompositeWidget::OnMouseDown(int x, int y, int button)
{
    for (int i = numChildren - 1; i >= 0; --i) {
        if (children[i]->ContainsPoint(x - this->x, y - this->y)) {
            children[i]->OnMouseDown(x - this->x, y - this->y, button);
            break;
        }
    }
}
void CompositeWidget::OnMouseUp(int x, int y, int button)
{
    for (int i = numChildren - 1; i >= 0; --i) {
        if (children[i]->ContainsPoint(x - this->x, y - this->y)) {
            children[i]->OnMouseUp(x - this->x, y - this->y, button);
            break;
        }
    }
}
void CompositeWidget::OnMouseMove(int x, int y)
{
    for (int i = numChildren - 1; i >= 0; --i) {
        if (children[i]->ContainsPoint(x - this->x, y - this->y)) {
            children[i]->OnMouseMove(x - this->x, y - this->y);
            break;
        }
    }
}
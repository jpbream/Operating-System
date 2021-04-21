#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <stdint.h>

#include "graphics_context.h"
#include "keyboard_handler.h"

class Widget : public KeyboardEventHandler
{
protected:
    Widget* parent;
    int x;
    int y;
    int width;
    int height;

    char color;

public:

    Widget(Widget* parent, int x, int y, int w, int h, char color);
    ~Widget();

    virtual void GetFocus(Widget* widget);
    virtual void ModelToScreen(int& x, int& y);

    virtual void Draw(GraphicsContext* gc);

    virtual bool ContainsPoint(int x, int y);

    virtual void OnMouseDown(int x, int y, int button);
    virtual void OnMouseUp(int x, int y, int button) = 0;
    virtual void OnMouseMove(int x, int y) = 0;
};


class CompositeWidget : public Widget
{
private:
    Widget* children[100];
    int numChildren;
    Widget* focusedChild;

public:
    CompositeWidget(Widget* parent, int x, int y, int w, int h, char color);
    ~CompositeWidget();

    virtual void GetFocus(Widget* widget) override;
    virtual void AddChild(Widget* widget);

    virtual void Draw(GraphicsContext* gc) override;

    virtual void OnMouseDown(int x, int y, int button) override;
    virtual void OnMouseUp(int x, int y, int button) override;
    virtual void OnMouseMove(int x, int y) override;

    virtual void OnKeyDown(char key) override;
    virtual void OnKeyUp(char key) override;
};

#endif
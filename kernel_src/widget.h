#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <stdint.h>

#include "graphics_context.h"

class Widget
{
protected:
    Widget* parent;
    int x;
    int y;
    int width;
    int height;

    uint8_t r;
    uint8_t g;
    uint8_t b;

public:

    Widget(Widget* parent, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b);
    ~Widget();

    virtual void GetFocus(Widget* widget);
    virtual void ModelToScreen(int& x, int& y);

    virtual void Draw(GraphicsContext* gc);

    virtual void OnMouseDown(int x, int y);
    virtual void OnMouseUp(int x, int y);
    virtual void OnMouseMove(int x, int y);

    virtual void OnKeyDown(char key);
    virtual void OnKeyUp(char key);
};


class CompositeWidget : public Widget
{
private:
    Widget* children[100];
    int numChildren;
    Widget* focusedChild;

public:
    CompositeWidget(Widget* parent, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b);
    ~CompositeWidget();

    virtual void GetFocus(Widget* widget) override;

    virtual void Draw(GraphicsContext* gc) override;

    virtual void OnMouseDown(int x, int y) override;
    virtual void OnMouseUp(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;

    virtual void OnKeyDown(char key) override;
    virtual void OnKeyUp(char key) override;
};

#endif
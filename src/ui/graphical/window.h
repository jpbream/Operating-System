#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "desktop.h"

class Window : public CompositeWidget
{
private:
    bool mouseDown;

public:
    Window(Desktop* parent, int x, int y, int w, int h, char c);

    void OnMouseDown(int x, int y, int button) override;
    void OnMouseUp(int x, int y, int button) override;
    void OnMouseMove(int x, int y) override;

    void OnKeyDown(char key) override;
    void OnKeyUp(char key) override;
};

#endif
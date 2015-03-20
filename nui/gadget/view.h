#ifndef NUI_VIEW_H_
#define NUI_VIEW_H_

#include "ui_object.h"
#include "focus_manager.h"
#include "event_dispatch.h"

namespace nui
{

class View : public Gadget
{
public:
    View(UIObjectHost & host);

    ~View();

    bool init();

    void fini();

    void Update(GraphicDevice & device, Rect & invalid_rect);

    void Blink();

    void HandleMouseEvent(
        MouseEvent::Type type,
        Mouse::Buttons button,
        Mouse::Buttons state,
        Point & point,
        Point & offset);

    //void HandleTextEvent(TextEvent & e);

    void HandleKeyEvent(KeyEvent::Type type, Keyboard::Keys key);

    void CaptureMouse(Gadget * v);

    void ReleaseMouse(Gadget * v);

    void SetFocus(bool focus);

    //节点内部通知
    //void Invalidate(Gadget &, Rect & rect) override;
protected:
    void ShiftFocus(Gadget *);

    void ShiftMouseOver(Gadget *);
private:
    Gadget * GetTargetObject(int x, int y);

    void UpdateObjectStatus();
private:
    Gadget * over_object_;
    Gadget * capture_object_;

    FocusManager focus_manager_;
    EventDispatch event_dispatch_;
};


}
#endif
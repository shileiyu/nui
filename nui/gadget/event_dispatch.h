#ifndef NUI_GADGET_EVENT_DISPATCH_H_
#define NUI_GADGET_EVENT_DISPATCH_H_

#include <nui/nui.h>
#include <nui/gadget/types.h>

namespace nui
{

class GadgetMouseEvent;
class GadgetKeyEvent;
class GadgetFocusEvent;
class GadgetEvent;
class GadgetWheelEvent;

class EventDispatch
{
protected:
    static void Run(EventDispatch & dispatch, Event & e);

    virtual bool Pass(ScopedGadget target, Event & e);
};

class KeyEventDispatch : public EventDispatch 
{
public:
    static void Run(KeyEvent & e);    
};

class FocusEventDispatch :public EventDispatch 
{
public:
    static void Run(FocusEvent & e);
};

class MouseEventDispatch : public EventDispatch
{
public:
    static void Run(MouseEvent & e);
};

class WheelEventDispatch : public EventDispatch
{
public:
    static void Run(WheelEvent & e);
};

}
#endif
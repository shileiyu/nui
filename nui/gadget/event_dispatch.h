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
    static void Run(EventDispatch & dispatch, GadgetEvent & e);

    virtual bool Pass(ScopedGadget target, GadgetEvent & e);
};

class KeyEventDispatch : public EventDispatch 
{
public:
    static void Run(GadgetKeyEvent & e);    
};

class FocusEventDispatch :public EventDispatch 
{
public:
    static void Run(GadgetFocusEvent & e);
};

class MouseEventDispatch : public EventDispatch
{
public:
    static void Run(GadgetMouseEvent & e);
protected:
    virtual bool Pass(ScopedGadget target, GadgetEvent & e);
};

class WheelEventDispatch : public EventDispatch
{
public:
    static void Run(GadgetWheelEvent & e);
};

}
#endif
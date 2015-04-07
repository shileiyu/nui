#ifndef NUI_GADGET_EVENT_H_
#define NUI_GADGET_EVENT_H_

#include <nui/nui.h>
#include <nui/base/mouse.h>
#include <nui/base/keyboard.h>
#include <nui/base/point.h>
#include <nui/gadget/types.h>
#include <nui/gadget/gadget.h>

namespace nui
{

enum class EventType
{
    kUndefined,
    kMouseMove,
    kMouseDown,
    kMouseUp,
    kMouseClick,
    kMouseDoubleClick,
    kMouseOver,
    kMouseOut,

    kKeyDown,
    kKeyUp,

    kFocusOut,
    kFocusIn,
    kBlur,
    kFocus,

    kWheel,
};

enum EventPhase
{
    kCapturing,
    kAtTarget,
    kBubbling,
};

struct EventFeature
{
    uint32_t bubbles : 1;
    uint32_t cancelable : 1;
};

struct EventStatus
{
    uint32_t propagating : 1;
    uint32_t canceled : 1;
};

class Event
{
    friend class EventDispatch;
public:
    void PreventDefault();

    void StopPropagation();

    ScopedGadget target();

    bool Canceled() const;

    bool Propagation() const;

    EventType type() const;

    EventPhase phase() const;
protected:
    Event();

protected:
    EventType type_;
    EventPhase phase_;
    EventFeature feature_;
    EventStatus status_;
    ScopedGadget target_;
};

class MouseEvent : public Event
{
    friend class MouseEventHelper;
    friend class MouseEventDispatch;
public:
    static MouseEvent MouseMove(
        ScopedGadget target,
        MouseState state,
        const Point & pt
    );

    static MouseEvent MouseDown(
        ScopedGadget target,
        MouseButton button,
        MouseState state,
        const Point & pt
    );

    static MouseEvent MouseUp(
        ScopedGadget target,
        MouseButton button,
        MouseState state,
        const Point & pt
    );

    static MouseEvent MouseClick(
        ScopedGadget target,
        MouseButton button,
        MouseState state,
        const Point & pt
    );

    static MouseEvent MouseDoubleClick(
        ScopedGadget target,
        MouseButton button,
        MouseState state,
        const Point & pt
    ); 
    
    static MouseEvent MouseOver(
        ScopedGadget target
    );

    static MouseEvent MouseOut(
        ScopedGadget target
    );   

    static MouseEvent * FromEvent(Event & e);

    int32_t x() const;

    int32_t y() const;

    MouseButton button() const;

    MouseState state() const;
protected:
    MouseEvent();
private:
    MouseButton button_;
    MouseState state_;
    int32_t x_;
    int32_t y_;
    int32_t global_x_;
    int32_t global_y_;
};

class KeyEvent : public Event
{
public:
    static KeyEvent KeyUp(ScopedGadget target, KeyCode key);

    static KeyEvent KeyDown(ScopedGadget target, KeyCode key);

    static KeyEvent * FromEvent(Event & e);
    
    KeyCode key_code() const;

    bool IsShiftDown() const;

    bool IsControlDown() const;

    bool IsAltDown() const;
private:
    KeyCode key_;
};

class FocusEvent : public Event
{
public:
    static FocusEvent FocusOut(ScopedGadget target, bool tab);

    static FocusEvent FocusIn(ScopedGadget target, bool tab);

    static FocusEvent Blur(ScopedGadget target, bool tab);

    static FocusEvent Focus(ScopedGadget target, bool tab);

    static FocusEvent * FromEvent(Event & e);
    
    bool IsTabTraversal() const;
protected:
    FocusEvent();
private:
    bool tab_traversal_;
};

class WheelEvent : public Event
{
public:
    static WheelEvent Make(ScopedGadget target, int dx, int dy);

    static WheelEvent * FromEvent(Event & e);
public:
    int32_t dy() const;

    int32_t dx() const;
protected:
    WheelEvent();
private:
    int32_t dx_;
    int32_t dy_;
};


}
#endif
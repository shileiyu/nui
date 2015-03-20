#ifndef NUI_GADGET_EVENT_H_
#define NUI_GADGET_EVENT_H_

#include <nui/nui.h>
#include <nui/base/mouse.h>
#include <nui/sys/keyboard.h>
#include <nui/base/point.h>
#include <nui/gadget/types.h>
#include <nui/gadget/gadget.h>

namespace nui
{

enum class GadgetEventType
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

enum GadgetEventPhase
{
    kCapturing,
    kAtTarget,
    kBubbling,
};

struct GadgetEventFeature
{
    uint32_t bubbles : 1;
    uint32_t cancelable : 1;
};

struct GadgetEventStatus
{
    uint32_t propagating : 1;
    uint32_t canceled : 1;
};

class GadgetEvent
{
    friend class EventDispatch;
public:
    void PreventDefault();

    void StopPropagation();

    ScopedGadget target();

    bool Canceled() const;

    bool Propagation() const;

    GadgetEventType type() const;

    GadgetEventPhase phase() const;
protected:
    GadgetEvent();

protected:
    GadgetEventType type_;
    GadgetEventPhase phase_;
    GadgetEventFeature feature_;
    GadgetEventStatus status_;
    ScopedGadget target_;
};

class GadgetMouseEvent : public GadgetEvent
{
    friend class MouseEventDispatch;
public:
    static GadgetMouseEvent MouseMove(
        ScopedGadget target,
        MouseState state,
        const Point & pt
    );

    static GadgetMouseEvent MouseDown(
        ScopedGadget target,
        MouseButton button,
        MouseState state,
        const Point & pt
    );

    static GadgetMouseEvent MouseUp(
        ScopedGadget target,
        MouseButton button,
        MouseState state,
        const Point & pt
    );

    static GadgetMouseEvent MouseClick(
        ScopedGadget target,
        MouseButton button,
        MouseState state,
        const Point & pt
    );

    static GadgetMouseEvent MouseDoubleClick(
        ScopedGadget target,
        MouseButton button,
        MouseState state,
        const Point & pt
    ); 
    
    static GadgetMouseEvent MouseOver(
        ScopedGadget target
    );

    static GadgetMouseEvent MouseOut(
        ScopedGadget target
    );   

    static GadgetMouseEvent * FromEvent(GadgetEvent & e);

    int32_t x() const;

    int32_t y() const;

    MouseButton button() const;

    MouseState state() const;

protected:
    GadgetMouseEvent();
private:
    MouseButton button_;
    MouseState state_;
    int32_t x_;
    int32_t y_;
};

class GadgetKeyEvent : public GadgetEvent
{
public:
    static GadgetKeyEvent KeyUp(ScopedGadget target, KeyCode key);

    static GadgetKeyEvent KeyDown(ScopedGadget target, KeyCode key);

    static GadgetKeyEvent * FromEvent(GadgetEvent & e);
    
    KeyCode key_code() const;

    bool IsShiftDown() const;

    bool IsControlDown() const;

    bool IsAltDown() const;
private:
    KeyCode key_;
};

class GadgetFocusEvent : public GadgetEvent
{
public:
    static GadgetFocusEvent FocusOut(ScopedGadget target, bool tab);

    static GadgetFocusEvent FocusIn(ScopedGadget target, bool tab);

    static GadgetFocusEvent Blur(ScopedGadget target, bool tab);

    static GadgetFocusEvent Focus(ScopedGadget target, bool tab);

    static GadgetFocusEvent * FromEvent(GadgetEvent & e);
    
    bool IsTabTraversal() const;
protected:
    GadgetFocusEvent();
private:
    bool tab_traversal_;
};

class GadgetWheelEvent : public GadgetEvent
{
public:
    static GadgetWheelEvent Make(ScopedGadget target, int dx, int dy);

    static GadgetWheelEvent * FromEvent(GadgetEvent & e);
public:
    int32_t dy() const;

    int32_t dx() const;
protected:
    GadgetWheelEvent();
private:
    int32_t dx_;
    int32_t dy_;
};


}
#endif
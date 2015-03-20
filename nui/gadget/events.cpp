
#include <nui/gadget/events.h>

namespace nui
{

GadgetEvent::GadgetEvent()
{
    type_ = GadgetEventType::kUndefined;
    phase_ = kCapturing;
    feature_.bubbles = true;
    feature_.cancelable = true;
    status_.canceled = false;
    status_.propagating = true;
}

void GadgetEvent::PreventDefault()
{
    if (feature_.cancelable)
        status_.canceled = true;
}

void GadgetEvent::StopPropagation()
{
    status_.propagating = false;
}

ScopedGadget GadgetEvent::target()
{
    return target_;
}

bool GadgetEvent::Canceled() const
{
    return status_.canceled;
}

bool GadgetEvent::Propagation() const
{
    return status_.propagating;
}

GadgetEventType GadgetEvent::type() const
{
    return type_;
}

GadgetEventPhase GadgetEvent::phase() const
{
    return phase_;
}

GadgetMouseEvent GadgetMouseEvent::MouseMove(
    ScopedGadget target,
    MouseState state,
    const Point & pt
) {
    GadgetMouseEvent e;
    e.type_ = GadgetEventType::kMouseMove;
    e.target_ = target;
    e.button_ = MouseButton::kNone;
    e.state_ = state;
    e.x_ = pt.x();
    e.y_ = pt.y();
    return e;
}

GadgetMouseEvent GadgetMouseEvent::MouseDown(
    ScopedGadget target,
    MouseButton button,
    MouseState state,
    const Point & pt
){
    GadgetMouseEvent e;
    e.type_ = GadgetEventType::kMouseDown;
    e.target_ = target;
    e.button_ = button;
    e.state_ = state;
    e.x_ = pt.x();
    e.y_ = pt.y();
    return e;
}

GadgetMouseEvent GadgetMouseEvent::MouseUp(
    ScopedGadget target,
    MouseButton button,
    MouseState state,
    const Point & pt
) {
    GadgetMouseEvent e;
    e.type_ = GadgetEventType::kMouseUp;
    e.target_ = target;
    e.button_ = button;
    e.state_ = state;
    e.x_ = pt.x();
    e.y_ = pt.y();
    return e;
}

GadgetMouseEvent GadgetMouseEvent::MouseClick(
    ScopedGadget target,
    MouseButton button,
    MouseState state,
    const Point & pt
) {
    GadgetMouseEvent e;
    e.type_ = GadgetEventType::kMouseClick;
    e.target_ = target;
    e.button_ = button;
    e.state_ = state;
    e.x_ = pt.x();
    e.y_ = pt.y();
    return e;
}

GadgetMouseEvent GadgetMouseEvent::MouseDoubleClick(
    ScopedGadget target,
    MouseButton button,
    MouseState state,
    const Point & pt
){
    GadgetMouseEvent e;
    e.type_ = GadgetEventType::kMouseDoubleClick;
    e.target_ = target;
    e.button_ = button;
    e.state_ = state;
    e.x_ = pt.x();
    e.y_ = pt.y();
    return e;
}
    
GadgetMouseEvent GadgetMouseEvent::MouseOver(
    ScopedGadget target
) {
    GadgetMouseEvent e;
    e.type_ = GadgetEventType::kMouseOver;
    e.target_ = target;
    e.button_ = MouseButton::kNone;
    e.state_ = MouseState::kPressedNone;
    e.x_ = 0;
    e.y_ = 0;
    return e;
}

GadgetMouseEvent GadgetMouseEvent::MouseOut(
    ScopedGadget target
) {
    GadgetMouseEvent e;
    e.type_ = GadgetEventType::kMouseOut;
    e.target_ = target;
    e.button_ = MouseButton::kNone;
    e.state_ = MouseState::kPressedNone;
    e.x_ = 0;
    e.y_ = 0;
    return e;
}

GadgetMouseEvent * GadgetMouseEvent::FromEvent(GadgetEvent & e)
{
    switch(e.type()){
    case GadgetEventType::kMouseMove:
    case GadgetEventType::kMouseDown:
    case GadgetEventType::kMouseUp:
    case GadgetEventType::kMouseClick:
    case GadgetEventType::kMouseDoubleClick:
    case GadgetEventType::kMouseOver:
    case GadgetEventType::kMouseOut:
        return &static_cast<GadgetMouseEvent&>(e);
    }
    return nullptr;
}

GadgetMouseEvent::GadgetMouseEvent()
    : button_(MouseButton::kPrimary), state_(MouseState::kPressedNone), 
      x_(0), y_(0)
{
}

int32_t GadgetMouseEvent::x() const
{
    return x_;
}

int32_t GadgetMouseEvent::y() const
{
    return y_;
}


MouseButton GadgetMouseEvent::button() const
{
    return button_;
}

MouseState GadgetMouseEvent::state() const
{
    return state_;
}

GadgetKeyEvent GadgetKeyEvent::KeyUp(ScopedGadget target, KeyCode key)
{
    GadgetKeyEvent e;
    e.type_ = GadgetEventType::kKeyUp;
    e.target_ = target;
    e.key_ = key;
    return e;
}

GadgetKeyEvent GadgetKeyEvent::KeyDown(ScopedGadget target, KeyCode key)
{
    GadgetKeyEvent e;
    e.type_ = GadgetEventType::kKeyDown;
    e.target_ = target;
    e.key_ = key;
    return e;
}

GadgetKeyEvent * GadgetKeyEvent::FromEvent(GadgetEvent & e)
{
    switch (e.type())
    {
    case GadgetEventType::kKeyDown:
    case GadgetEventType::kKeyUp:
        return &static_cast<GadgetKeyEvent &>(e);
    }
    return nullptr;
}

KeyCode GadgetKeyEvent::key_code() const
{
    return key_;
}

bool GadgetKeyEvent::IsShiftDown() const
{
    return !!(key_ & KeyCode::kShift);
}

bool GadgetKeyEvent::IsControlDown() const
{
    return !!(key_ & KeyCode::kControl);
}

bool GadgetKeyEvent::IsAltDown() const
{
    return !!(key_ & KeyCode::kAlt);
}

GadgetFocusEvent GadgetFocusEvent::FocusOut(ScopedGadget target, bool tab)
{
    GadgetFocusEvent e;
    e.type_ = GadgetEventType::kFocusOut;
    e.target_ = target;
    e.tab_traversal_ = tab;
    return e;
}

GadgetFocusEvent GadgetFocusEvent::FocusIn(ScopedGadget target, bool tab)
{
    GadgetFocusEvent e;
    e.type_ = GadgetEventType::kFocusIn;
    e.target_ = target;
    e.tab_traversal_ = tab;
    return e;
}

GadgetFocusEvent GadgetFocusEvent::Blur(ScopedGadget target, bool tab)
{
    GadgetFocusEvent e;
    e.type_ = GadgetEventType::kBlur;
    e.target_ = target;
    e.tab_traversal_ = tab;
    return e;
}

GadgetFocusEvent GadgetFocusEvent::Focus(ScopedGadget target, bool tab)
{
    GadgetFocusEvent e;
    e.type_ = GadgetEventType::kFocus;
    e.target_ = target;
    e.tab_traversal_ = tab;
    return e;
}

bool GadgetFocusEvent::IsTabTraversal() const
{
    return tab_traversal_;
}

GadgetFocusEvent * GadgetFocusEvent::FromEvent(GadgetEvent & e)
{
    switch (e.type()){
    case GadgetEventType::kFocusOut:
    case GadgetEventType::kFocusIn:
    case GadgetEventType::kBlur:
    case GadgetEventType::kFocus:
        return &static_cast<GadgetFocusEvent &>(e);
    }
    return nullptr;
}

GadgetFocusEvent::GadgetFocusEvent()
:tab_traversal_(false)
{
    ;
}

GadgetWheelEvent GadgetWheelEvent::Make(ScopedGadget target, int dx, int dy)
{
    GadgetWheelEvent e;
    e.target_ = target;
    e.dx_ = dx;
    e.dy_ = dy;
    return e;
}

GadgetWheelEvent::GadgetWheelEvent()
    : dx_(0), dy_(0)
{
    type_ = GadgetEventType::kWheel;
}

GadgetWheelEvent * GadgetWheelEvent::FromEvent(GadgetEvent & e)
{
    switch (e.type())
    {
    case GadgetEventType::kWheel:
        return &static_cast<GadgetWheelEvent &>(e);
    }
    return nullptr;
}

int32_t GadgetWheelEvent::dy() const
{
    return dy_;
}

int32_t GadgetWheelEvent::dx() const
{
    return dx_;
}


}

#include <nui/gadget/events.h>

namespace nui
{

Event::Event()
{
    type_ = EventType::kUndefined;
    phase_ = kCapturing;
    feature_.bubbles = true;
    feature_.cancelable = true;
    status_.canceled = false;
    status_.propagating = true;
}

void Event::PreventDefault()
{
    if (feature_.cancelable)
        status_.canceled = true;
}

void Event::StopPropagation()
{
    status_.propagating = false;
}

ScopedGadget Event::target()
{
    return target_;
}

bool Event::Canceled() const
{
    return status_.canceled;
}

bool Event::Propagation() const
{
    return status_.propagating;
}

EventType Event::type() const
{
    return type_;
}

EventPhase Event::phase() const
{
    return phase_;
}

class MouseEventHelper 
{
public:
    template <typename Event>
    static void SetPosition(Event & e, const Point & pt)
    {
        auto gpt = e.target_->MapToGlobal(pt);

        e.x_ = pt.x();
        e.y_ = pt.y();
        e.global_x_ = gpt.x();
        e.global_y_ = gpt.y();
    }
};



MouseEvent MouseEvent::MouseMove(
    ScopedGadget target,
    MouseState state,
    const Point & pt
) {
    MouseEvent e;
    e.type_ = EventType::kMouseMove;
    e.target_ = target;
    e.button_ = MouseButton::kNone;
    e.state_ = state;
    MouseEventHelper::SetPosition(e, pt);
    return e;
}

MouseEvent MouseEvent::MouseDown(
    ScopedGadget target,
    MouseButton button,
    MouseState state,
    const Point & pt
){
    MouseEvent e;
    e.type_ = EventType::kMouseDown;
    e.target_ = target;
    e.button_ = button;
    e.state_ = state;
    MouseEventHelper::SetPosition(e, pt);
    return e;
}

MouseEvent MouseEvent::MouseUp(
    ScopedGadget target,
    MouseButton button,
    MouseState state,
    const Point & pt
) {
    MouseEvent e;
    e.type_ = EventType::kMouseUp;
    e.target_ = target;
    e.button_ = button;
    e.state_ = state;
    MouseEventHelper::SetPosition(e, pt);
    return e;
}

MouseEvent MouseEvent::MouseClick(
    ScopedGadget target,
    MouseButton button,
    MouseState state,
    const Point & pt
) {
    MouseEvent e;
    e.type_ = EventType::kMouseClick;
    e.target_ = target;
    e.button_ = button;
    e.state_ = state;
    MouseEventHelper::SetPosition(e, pt);
    return e;
}

MouseEvent MouseEvent::MouseDoubleClick(
    ScopedGadget target,
    MouseButton button,
    MouseState state,
    const Point & pt
){
    MouseEvent e;
    e.type_ = EventType::kMouseDoubleClick;
    e.target_ = target;
    e.button_ = button;
    e.state_ = state;
    MouseEventHelper::SetPosition(e, pt);
    return e;
}
    
MouseEvent MouseEvent::MouseOver(
    ScopedGadget target
) {
    MouseEvent e;
    e.type_ = EventType::kMouseOver;
    e.target_ = target;
    e.button_ = MouseButton::kNone;
    e.state_ = MouseState::kPressedNone;
    return e;
}

MouseEvent MouseEvent::MouseOut(
    ScopedGadget target
) {
    MouseEvent e;
    e.type_ = EventType::kMouseOut;
    e.target_ = target;
    e.button_ = MouseButton::kNone;
    e.state_ = MouseState::kPressedNone;
    return e;
}

MouseEvent * MouseEvent::FromEvent(Event & e)
{
    switch(e.type()){
    case EventType::kMouseMove:
    case EventType::kMouseDown:
    case EventType::kMouseUp:
    case EventType::kMouseClick:
    case EventType::kMouseDoubleClick:
    case EventType::kMouseOver:
    case EventType::kMouseOut:
        return &static_cast<MouseEvent&>(e);
    }
    return nullptr;
}

MouseEvent::MouseEvent()
    : button_(MouseButton::kPrimary), state_(MouseState::kPressedNone), 
    x_(0), y_(0), global_x_(0), global_y_(0)
{
}

int32_t MouseEvent::x() const
{
    return x_;
}

int32_t MouseEvent::y() const
{
    return y_;
}


MouseButton MouseEvent::button() const
{
    return button_;
}

MouseState MouseEvent::state() const
{
    return state_;
}

KeyEvent KeyEvent::KeyUp(ScopedGadget target, KeyCode key)
{
    KeyEvent e;
    e.type_ = EventType::kKeyUp;
    e.target_ = target;
    e.key_ = key;
    return e;
}

KeyEvent KeyEvent::KeyDown(ScopedGadget target, KeyCode key)
{
    KeyEvent e;
    e.type_ = EventType::kKeyDown;
    e.target_ = target;
    e.key_ = key;
    return e;
}

KeyEvent * KeyEvent::FromEvent(Event & e)
{
    switch (e.type())
    {
    case EventType::kKeyDown:
    case EventType::kKeyUp:
        return &static_cast<KeyEvent &>(e);
    }
    return nullptr;
}

KeyCode KeyEvent::key_code() const
{
    return key_;
}

bool KeyEvent::IsShiftDown() const
{
    return !!(key_ & KeyCode::kShift);
}

bool KeyEvent::IsControlDown() const
{
    return !!(key_ & KeyCode::kControl);
}

bool KeyEvent::IsAltDown() const
{
    return !!(key_ & KeyCode::kAlt);
}

FocusEvent FocusEvent::FocusOut(ScopedGadget target, bool tab)
{
    FocusEvent e;
    e.type_ = EventType::kFocusOut;
    e.target_ = target;
    e.tab_traversal_ = tab;
    return e;
}

FocusEvent FocusEvent::FocusIn(ScopedGadget target, bool tab)
{
    FocusEvent e;
    e.type_ = EventType::kFocusIn;
    e.target_ = target;
    e.tab_traversal_ = tab;
    return e;
}

FocusEvent FocusEvent::Blur(ScopedGadget target, bool tab)
{
    FocusEvent e;
    e.type_ = EventType::kBlur;
    e.target_ = target;
    e.tab_traversal_ = tab;
    return e;
}

FocusEvent FocusEvent::Focus(ScopedGadget target, bool tab)
{
    FocusEvent e;
    e.type_ = EventType::kFocus;
    e.target_ = target;
    e.tab_traversal_ = tab;
    return e;
}

bool FocusEvent::IsTabTraversal() const
{
    return tab_traversal_;
}

FocusEvent * FocusEvent::FromEvent(Event & e)
{
    switch (e.type()){
    case EventType::kFocusOut:
    case EventType::kFocusIn:
    case EventType::kBlur:
    case EventType::kFocus:
        return &static_cast<FocusEvent &>(e);
    }
    return nullptr;
}

FocusEvent::FocusEvent()
:tab_traversal_(false)
{
    ;
}

WheelEvent WheelEvent::Make(ScopedGadget target, int dx, int dy)
{
    WheelEvent e;
    e.target_ = target;
    e.dx_ = dx;
    e.dy_ = dy;
    return e;
}

WheelEvent::WheelEvent()
    : dx_(0), dy_(0)
{
    type_ = EventType::kWheel;
}

WheelEvent * WheelEvent::FromEvent(Event & e)
{
    switch (e.type())
    {
    case EventType::kWheel:
        return &static_cast<WheelEvent &>(e);
    }
    return nullptr;
}

int32_t WheelEvent::dy() const
{
    return dy_;
}

int32_t WheelEvent::dx() const
{
    return dx_;
}


}
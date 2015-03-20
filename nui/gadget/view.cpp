#include "view.h"

#include "focus_event.h"

namespace nui
{

//class SurfacePixelRef : public SkPixelRef
//{
//public:
//    SurfacePixelRef(void * pixels);
//
//    SK_DECLARE_UNFLATTENABLE_OBJECT();
//protected:
//    virtual void * onLockPixels(SkColorTable ** color_table) override;
//
//    virtual void onUnlockPixels() override;
//
//private:
//    void * pixels_;
//};
//
//SurfacePixelRef::SurfacePixelRef(void * pixels)
//:pixels_(pixels)
//{
//    setPreLocked(pixels, nullptr);
//}
//
//void * SurfacePixelRef::onLockPixels(SkColorTable ** color_table)
//{
//    *color_table = nullptr;
//    return pixels_;
//}
//
//void SurfacePixelRef::onUnlockPixels()
//{
//    return;
//}

View::View(UIObjectHost & host)
:Gadget(host), over_object_(0), capture_object_(0)
{
    ;
}

View::~View()
{
    fini();
}

bool View::init()
{
    return Gadget::init();
}

void View::fini()
{
    Gadget::fini();
}

void View::Update(GraphicDevice & device, Rect & invalid_rect)
{
    //ISurface::Bitmap bm;
    //surface.Query(bm);

    //SkBitmap skbm;
    //skbm.setConfig(SkBitmap::kARGB_8888_Config, bm.width, bm.height, bm.pitch);
    //SurfacePixelRef pixel_ref(bm.data);

    //SkPixelRef * old_pixel_ref = skbm.pixelRef();
    //skbm.setPixelRef(&pixel_ref);
    //skbm.setIsOpaque(false);

    //SkCanvas canvas(skbm);
    //绘制
    //Gadget::Draw(canvas, invalid_rect);

    //skbm.setPixelRef(old_pixel_ref);
    Rect bounds;
    GetLocalBounds(bounds);
    bounds.Intersect(invalid_rect);
    if (!bounds.isEmpty())
        Gadget::Draw(device, invalid_rect);
}


void View::Blink()
{
    ;
}

void View::HandleMouseEvent(
    MouseEvent::Type type,
    Mouse::Buttons button,
    Mouse::Buttons state,
    Point & point,
    Point & offset)
{
    if (MouseEvent::kMouseEnter == type)
    {
        ShiftMouseOver(this);
        return;
    }
    else if (MouseEvent::kMouseLeave == type)
    {//
        ShiftMouseOver(nullptr);
        return;
    }
    
    Gadget * target = GetTargetObject(point.x(), point.y());
    if (nullptr == target)
        return;

    //产生MouseEnter和MouseLeave
    ShiftMouseOver(target);
    //处理其他鼠标事件
    MouseEvent me(target, type, point.x(), point.y(), button);
    me.set_dx(offset.x());
    me.set_dy(offset.y());
    me.set_state(state);

    event_dispatch_.Dispatch(me);

    //default处理流程
    if (!me.canceled())
    {
        if (MouseEvent::kMouseDown == type && Mouse::kLeft == button)
        {
            //左键按下 产生焦点事件
            ShiftFocus(target);
        }
        else if (MouseEvent::kMouseMove == type)
        {//鼠标移动 要设置光标
            //if (client_)
            //    client_->SetCursor(*this,
            //    handler ? handler->QueryCursor() : Cursor::kArrow);
        }
    }
    UpdateObjectStatus();
}

void View::HandleTextEvent(TextEvent & e)
{
    Gadget * current = focus_manager_.Current();
    if (current)
    {
        TextEvent te(current);
        //te.set_value();

        event_dispatch_.Dispatch(te);
    }
    UpdateObjectStatus();
}

void View::HandleKeyEvent(KeyEvent::Type type, Keyboard::Keys key)
{
    Gadget * current = focus_manager_.Current();
    if (current)
    {
        KeyEvent ke(current);
        ke.set_type(type);
        ke.set_value(key);
        event_dispatch_.Dispatch(ke);
    }
    UpdateObjectStatus();
}

void View::ShiftMouseOver(Gadget * target)
{
    if (target == over_object_)
        return;

    //离开旧控件
    if (over_object_)
    {
        MouseEvent me(over_object_);
        me.set_type(MouseEvent::kMouseLeave);
        event_dispatch_.Dispatch(me);
    }

    over_object_ = target;
    //进入新控件
    if (target)
    {
        MouseEvent me(target);
        me.set_type(MouseEvent::kMouseEnter);
        event_dispatch_.Dispatch(me);
    }
}



void View::UpdateObjectStatus()
{
    if (capture_object_)
    {
        if (!capture_object_->IsVisible())
            HandleKillCapture(capture_object_);
    }

    if (over_object_)
    {
        if (!over_object_->IsVisible())
            ShiftMouseOver(nullptr);
    }

    Gadget * focus = focus_manager_.Current();
    if (focus)
    {
        if (!focus->IsVisible())
            ShiftFocus(nullptr);
    }
}

void View::SetFocus(bool focus)
{
    if (focus)
    {
        if (!focus_manager_.Current())
        {
            ShiftFocus(this);
        }
    }
    else
    {
        ShiftFocus(nullptr);
    }
}

Gadget * View::GetTargetObject(int px, int py)
{
    if (capture_object_)
        return capture_object_;
    return Gadget::GetObjByPos(px, py);
}

//void View::Invalidate(Gadget &, Rect & rect)
//{
//    //client_->Invalidate(*this, rect);
//}

void View::ShiftFocus(Gadget * v)
{
    Gadget * current = focus_manager_.Current();
    if (v == current)
        return;

    if (current)
    {
        //Sent before first target element loses focus
        FocusEvent focusout(current, FocusEvent::kFocusOut);
        event_dispatch_.Dispatch(focusout);
    }
    if (v)
    {  
        //Sent before second target element receives focus
        FocusEvent focusin(v, FocusEvent::kFocusIn);
        event_dispatch_.Dispatch(focusin);
    }

    focus_manager_.Shift(v);

    if (current)
    {
        //Sent after first target element loses focus
        FocusEvent blur(current, FocusEvent::kBlur);
        event_dispatch_.Dispatch(blur);
    }

    if (v)
    {
        //Sent after second target element receives focus
        FocusEvent focus(v, FocusEvent::kFocus);
        event_dispatch_.Dispatch(focus);
    }
}


void View::CaptureMouse(Gadget * v)
{
    capture_object_ = v;
}

void View::ReleaseMouse(Gadget * v)
{
    if (nullptr == v || v == capture_object_)
    {
        capture_object_ = 0;
    }
}

//
//bool Host::HandleRemove(Gadget * v)
//{
//    return true;
//}

}
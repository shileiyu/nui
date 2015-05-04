
#include <nui/base/painter.h>
#include <nui/gadget/event_dispatch.h>
#include <nui/gadget/events.h>

#include "world.h"

namespace nui
{


class DummyWorldClient : public WorldClient
{
public:
    void PenddingRedraw(ScopedWorld world, const Rect & rect) {};

    void SetCursor(ScopedWorld world, CursorStyles cursor) {};

    void PenddingLayout(ScopedWorld world){};
};

World::World(WorldClient * client)
:focus_manager_(*this), client_(client), cursor_(CursorStyles::kArrow)
{
    ;
}

World::~World()
{
    ;
}

void World::SetMousePos(const Point & pt)
{
    MouseEventDispatch dispatch;
    mouse_pos_ = pt;

    auto target = GetTargetByMouse();
    if (target != over_)
    {
        ShiftOver(target);
    }

    if (target)
    {
        dispatch.Run(
            MouseEvent::MouseMove(target, mouse_state_, mouse_pos_));
    }
    Client().SetCursor(this, cursor_);
}

void World::MouseButtonDown(MouseButton mouse_button)
{
    typedef MouseEvent Event;
    MouseEventDispatch dispatch;

    auto target = GetTargetByMouse();

    if (mouse_button == MouseButton::kPrimary)
        capture_ = target;

    auto e = Event::MouseDown(target, mouse_button, mouse_state_, mouse_pos_);

    if (target)
    {
        dispatch.Run(e);
    }

    if (!e.Canceled())
    {
        if (e.button() == MouseButton::kPrimary)
        {//左键按下 产生焦点事件
            ShiftFocus(target);
        }
    }
}

void World::MouseButtonUp(MouseButton mouse_button)
{
    auto target = GetTargetByMouse();
    MouseEventDispatch dispatch;
    auto e = MouseEvent::MouseUp(target, mouse_button, mouse_state_, mouse_pos_);
    if (target)
    {
        dispatch.Run(e);
    }

    if (mouse_button == MouseButton::kPrimary ||
        mouse_button == MouseButton::kSecondry)
    {//左右键弹起 清空capture
        capture_ = 0;
        auto target = GetTargetByMouse();
        if (target != over_)
        {//如果弹起时 发现目标不再是over_
            ShiftOver(target);
            Client().SetCursor(this, cursor_);
        }
    }
}

void World::Wheel(int dx, int dy)
{
    WheelEventDispatch dispatch;

    auto target = GetTargetByMouse();
    auto e = WheelEvent::Make(target, dx, dy);
    if (target)
    {
        dispatch.Run(e);
    }
}

void World::SetMouseState(MouseState state)
{
    mouse_state_ = state;
}

void World::SetFocus(bool focus)
{
    if (focus)
    {
        focus_manager_.RestoreFocusedGadget();
        //if (!focus_manager_.GetFocusedGadget())
        //{
        //    auto child = GetChild(0);
        //    if (child)
        //        focus_manager_.SetFocusedGadget(child, 
        //        FocusManager::kReasonDirectFocusChange);
        //}
    }
    else
    {
        focus_manager_.StoreFocusedGadget();
        focus_manager_.SetFocusedGadget(nullptr);
    }
}

bool World::IsDrawn() const
{
    return visible();
}

void World::Draw(Painter & painter, const Rect & inval_rect)
{
    if (inval_rect.isEmpty())
        return;

    painter.Save();
    painter.ClipRect(inval_rect);
    Rect bounds(inval_rect);
    auto left = loc_.x();
    auto top = loc_.y();
    bounds.Offset(-left, -top);
    painter.Translate(left, top);
    Gadget::Draw(painter, inval_rect);
    painter.Restore();
}

ScopedGadget World::HitTest(const Point & pt)
{
    Point root_pt = pt - GetLoc();
    return Gadget::HitTest(root_pt);
}

FocusManager * World::GetFocusManager()
{
    return &focus_manager_;
}

const FocusManager * World::GetFocusManager() const
{
    return &focus_manager_;
}

void World::Invalidate(const Rect & inval)
{
    Client().PenddingRedraw(this, inval);
}

ScopedWorld World::GetRoot()
{
    return this;
}

const ScopedWorld World::GetRoot() const
{
    return const_cast<World *>(this);
}

void World::OnDraw(Painter & painter)
{
    Rect bounds;
    GetLocalBounds(bounds);

    Paint paint;
    paint.color = 0;
    paint.mode = Paint::kSrc;
    painter.DrawRect(bounds, paint);
}

void World::OnHierarchyChanged(const HierarchyChangedDetails & details)
{
    if (!details.is_add)
    {//移除通知
        if (capture_ && details.child == capture_)
            capture_ = nullptr;

        if (over_ && details.child == over_)
            ShiftOver(nullptr);
    }

}

void World::OnVisibilityChanged(ScopedGadget starting_from, bool is_visible)
{
    if (!is_visible)
    {
        capture_ = nullptr;
        ShiftOver(nullptr);
    }
}

void World::ShiftOver(ScopedGadget gadget)
{
    MouseEventDispatch dispatch;
    if (over_ != gadget)
    {
        if (over_)
        {//离开旧控件
            dispatch.Run(MouseEvent::MouseOut(over_));
        }
        over_ = gadget;
        if (over_)
        {//进入新控件
            dispatch.Run(MouseEvent::MouseOver(over_));
        }
    }
}

void World::ShiftFocus(ScopedGadget gadget)
{
    //指定gadget 不能接受焦点则父接受焦点
    ScopedGadget focus = gadget;
    while (focus && !focus->IsFocusable())
        focus = focus->parent();
    if (!focus || focus == this)
        return;

    focus_manager_.SetFocusedGadgetWithReason(focus, 
        FocusManager::kReasonDirectFocusChange);
}

void World::HandleKeyEvent(KeyEvent & e)
{
    if (focus_manager_.HandleKeyEvent(e))
        return;
}

ScopedGadget World::GetTargetByMouse()
{
    if (capture_)
        return capture_;

    return HitTest(mouse_pos_);
}

WorldClient & World::Client()
{
    static DummyWorldClient dc;
    if (client_ == nullptr)
        return dc;
    return *client_;
}


}
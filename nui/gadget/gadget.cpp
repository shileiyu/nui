
#include <nui/base/painter.h>
#include <nui/gadget/focus_manager.h>
#include <nui/layout/layout.h>
#include "events.h"
#include "gadget.h"
#include "world.h"

namespace nui
{

Gadget::HierarchyChangedDetails::HierarchyChangedDetails()
:is_add(false)
{

}

Gadget::HierarchyChangedDetails::HierarchyChangedDetails(bool add, 
                                                         ScopedGadget & parent_gadget, 
                                                         ScopedGadget & child_gadget, 
                                                         ScopedGadget & move_gadget)
: is_add(add), parent(parent_gadget), child(child_gadget), move(move_gadget)
{

}

Gadget::Gadget()
: group_id_(-1), listerner_(nullptr), layout_(nullptr)
{
    flag_.Set(Flag::kVisible);
    flag_.Set(Flag::kFocusable);
}

ScopedGadget Gadget::HitTest(const Point & pt)
{
    if (!visible())
        return nullptr;

    ScopedGadget target = nullptr;

    for (auto iter = children_.rbegin(); iter != children_.rend(); ++iter)
    {
        auto child = *iter;
        if (child)
        {
            target = child->HitTest(pt - child->GetLoc());
            if (target)
                break;
        }
    }

    if (!target)
    {
        Rect rect;
        GetLocalBounds(rect);
        if (rect.isPointIn(pt) && OnHitTest(pt))
            target = this;
    }

    return target;
}

//void Gadget::Blink(ScopedGadget & gadget)
//{
//    if (!gadget)
//        return;
//
//    gadget->Blink();
//
//    for (uint32_t i = 0; i < gadget->GetChildrenCount(); ++i)
//    {
//        auto child = gadget->GetChild(i);
//        if (child)
//        {
//            Blink(child);
//        }
//    }
//}


Gadget::~Gadget()
{
    assert(children_.empty());
}

void Gadget::SetLoc(const Point & pt)
{
    if (loc_.x() != pt.x() ||
        loc_.y() != pt.y())
    {
        Invalidate();
        loc_ = pt;
        OnPositionChanged();
        Invalidate();
    }
}

void Gadget::SetSize(const Size & size)
{
    if (size_.width() != size.width() ||
        size_.height() != size.height())
    {
        Invalidate();
        size_ = size;
        OnSizeChanged();
        Invalidate();
    }
}

const Point & Gadget::GetLoc() const
{
    return loc_;
}

const Size & Gadget::GetSize() const
{
    return size_;
}

int Gadget::GetLeft() const
{
    return loc_.x();
}

int Gadget::GetTop() const
{
    return loc_.y();
}

int Gadget::GetWidth() const
{
    return size_.width();
}

int Gadget::GetHeight() const
{
    return size_.height();
}

void Gadget::SetVisible(bool vis)
{
    if (visible() != vis)
    {
        if (vis)
            flag_.Set(Flag::kVisible);
        else
            flag_.Clear(Flag::kVisible);

        PropagateVisibilityNotifications(this, visible());
        AdvanceFocusIfNecessary();
        Invalidate();
    }

}

void Gadget::SetFocusable(bool focusable)
{
    if (focusable)
        flag_.Set(Flag::kFocusable);
    else
        flag_.Clear(Flag::kFocusable);

    AdvanceFocusIfNecessary();
}

void Gadget::GetLocalBounds(Rect & rect) const
{
    //得到ApeView的逻辑坐标
    rect.SetLTRB(0, 0, size_.width(), size_.height());
}

void Gadget::GetGlobleBounds(Rect & rect) const
{
    int left = loc_.x();
    int top = loc_.y();

    auto parent = parent_;
    if (parent_)
    {
        while (parent)
        {
            auto parent_loc = parent->GetLoc();
            left += parent_loc.x();
            top += parent_loc.y();
            parent = parent->parent();
        }
        rect.SetXYWH(left, top, size_.width(), size_.height());
    }
    else
        rect.SetXYWH(0, 0, size_.width(), size_.height());
}

void Gadget::SetPreferredSize(const Size & size)
{
    if (size.width() != preferred_size_.width() || 
        size.height() != preferred_size_.height())
    {
        preferred_size_ = size;
        PreferredSizeChanged();
    }
}

const Size & Gadget::GetPreferredSize() const
{
    if (preferred_size_.width() == 0 && preferred_size_.height() == 0)
        return size_;
    else
        return preferred_size_;
}

void Gadget::Invalidate()
{
    Rect bounds;
    GetLocalBounds(bounds);
    Invalidate(bounds);
}

void Gadget::Invalidate(const Rect & inval)
{
    if (parent_)
    {
        Rect parent_inval(inval);
        parent_inval.Offset(loc_.x(), loc_.y());
        parent_->Invalidate(parent_inval);
    } 
}

bool Gadget::visible() const
{
    //是否设置了kVisible 可见属性
    return flag_.Test(Flag::kVisible);
}

bool Gadget::focusable() const
{
    return flag_.Test(Flag::kFocusable);
}

bool Gadget::Contains(const ScopedGadget gadget) const
{
    auto tmp = gadget;
    while (tmp)
    {
        if (tmp == this)
            return true;
        tmp = tmp->parent_;
    }
    return false;
}

bool Gadget::IsFocusable() const
{
    return focusable() && IsDrawn();
}

void Gadget::SetListener(GadgetListener * listener)
{
    listerner_ = listener;
}

void Gadget::SetLayout(nui::Layout * layout)
{
    layout_ = layout;
}

bool Gadget::IsDrawn() const
{
    return visible() && parent_ ? parent_->IsDrawn() : false;
}

ScopedWorld Gadget::GetRoot()
{
    return parent_ ? parent_->GetRoot() : nullptr;
}

const ScopedWorld Gadget::GetRoot() const
{
    return parent_ ? parent_->GetRoot() : nullptr;
}



void Gadget::Draw(Painter & painter, const Rect & inval_rect)
{
    if (!visible())
        return;

    Rect bounds;
    GetLocalBounds(bounds);
    bounds.Intersect(inval_rect);
    if (bounds.isEmpty())
        return;

    painter.Save();
    painter.ClipRect(bounds);

    painter.Save();
    OnDraw(painter);
    painter.Restore();

    for (auto iter = children_.begin(); iter != children_.end(); ++iter)
    {
        auto & child = *iter;
        auto left = child->GetLoc().x();
        auto top = child->GetLoc().y();

        Rect child_inval(inval_rect);
        child_inval.Offset(-left, -top);
        painter.Save();
        painter.Translate(left, top);
        child->Draw(painter, child_inval);
        painter.Restore();
    }

    painter.Restore();
}

void Gadget::Blink()
{
    ;
}

void Gadget::Layout()
{
    if (layout_)
        layout_->Arrange(this);
    for (auto iter = children_.begin(); iter != children_.end(); ++iter)
    {
        auto child = *iter;
        if (child)
            child->Layout();
    }
}

void Gadget::PreferredSizeChanged()
{
    if (parent_)
        parent_->OnChildPreferredSizeChanged(this);
}

ScopedGadget & Gadget::parent()
{
    return parent_;
}


void Gadget::OnDraw(Painter & painter)
{
    ;
}

CursorStyles Gadget::QueryCursor() const
{
    return CursorStyles::kArrow;
}

void Gadget::OnChildPreferredSizeChanged(ScopedGadget child)
{
    Layout();
}

//默认返回true; px, py是相对于ApeView的坐标
bool Gadget::OnHitTest(const Point & pt)
{
    return true;
}

void Gadget::OnSizeChanged()
{
    ;
}

void Gadget::OnPositionChanged()
{
    ;
}

void Gadget::OnHierarchyChanged(const HierarchyChangedDetails & details)
{
    ;
}

void Gadget::OnVisibilityChanged(ScopedGadget starting_from, bool is_visible)
{
    ;
}

void Gadget::AddChild(ScopedGadget gadget)
{
    AddChildAt(gadget, children_.size());
}

void Gadget::AddChildAt(ScopedGadget gadget, int index)
{
    if (!gadget || gadget->parent() == this || gadget == this)
        return;
    if (index > static_cast<int>(children_.size()))
        return;

    auto old_parent = gadget->parent();
    if (old_parent)
    {
        old_parent->DoRemoveChild(gadget, ScopedGadget(this));
    }

    //初始化焦点链表
    InitFocusSiblings(gadget, index);

    gadget->parent_ = this;
    children_.insert(children_.begin() + index, gadget);

    HierarchyChangedDetails details(true, ScopedGadget(this), gadget, old_parent);
    ScopedGadget tmp = this;
    while (tmp)
    {
        tmp->HierarchyChangedImpl(details);
        tmp = tmp->parent();
    }

    gadget->PropagateAddNotifications(details);

    if (gadget->visible())
        gadget->Invalidate();
}

void Gadget::RemoveChild(ScopedGadget gadget)
{
    DoRemoveChild(gadget, ScopedGadget(nullptr));
}

void Gadget::RemoveFromParent()
{
    if (parent())
        parent()->DoRemoveChild(ScopedGadget(this), ScopedGadget(nullptr));
}

void Gadget::RemoveChildren()
{
    while (!children_.empty())
        DoRemoveChild(children_.front(), ScopedGadget(nullptr));
}

void Gadget::RecursiveRemoveChildren()
{
    while (!children_.empty())
    {
        children_.front()->RecursiveRemoveChildren();
    }
    RemoveFromParent();
}

size_t Gadget::GetChildCount() const
{
    return children_.size();
}

ScopedGadget Gadget::GetChild(int index)
{
    if (index < static_cast<int>(children_.size()))
        return children_[index];
    
    return nullptr;
}

void Gadget::SetGroup(int gid)
{
    group_id_ = gid;
}

int Gadget::GetGroup() const
{
    return group_id_;
}

void Gadget::GetGadgetsInGroup(int gid, ScopedGadgets & gadgets)
{
    if (group_id_ == gid)
        gadgets.push_back(this);

    for (auto iter = children_.begin(); iter != children_.end(); ++iter)
    {
        (*iter)->GetGadgetsInGroup(gid, gadgets);
    }
}

bool Gadget::IsGroupFocusTraversable() const
{
    return true;
}

ScopedGadget Gadget::GetSelectedForGroup(int gid)
{
    ScopedGadgets gadgets;

    ScopedGadget root = this;
    while (root->parent())
        root = root->parent();

    root->GetGadgetsInGroup(gid, gadgets);
    return gadgets.empty() ? nullptr : gadgets[0];
}

ScopedGadget & Gadget::GetNextFocusable()
{
    return next_focusable_;
}

ScopedGadget & Gadget::GetPreviousFocusable()
{
    return previous_focusable_;
}

void Gadget::DoRemoveChild(ScopedGadget & child, ScopedGadget & new_parent)
{
    auto iter = std::find(children_.begin(), children_.end(), child);
    if (iter == children_.end())
        return;
    //更新焦点链表
    ScopedGadget next = child->next_focusable_;
    ScopedGadget prev = child->previous_focusable_;
    if (prev)
        prev->next_focusable_ = next;
    if (next)
        next->previous_focusable_ = prev;

    //
    if (child->visible())
        child->Invalidate();

    auto root = GetRoot();
    FocusManager * focus_manager = root ? root->GetFocusManager() : nullptr;
    if (focus_manager)
        focus_manager->RemoveGadget(child);
    //通知child所有的子
    child->PropagateRemoveNotifications(ScopedGadget(this), new_parent);
    child->parent_ = nullptr;
    children_.erase(iter);

}

void Gadget::PropagateAddNotifications(const HierarchyChangedDetails & details)
{
    for (auto iter = children_.begin(); iter != children_.end(); ++iter)
    {
        (*iter)->PropagateAddNotifications(details);
    }

    HierarchyChangedImpl(details);
}

void Gadget::PropagateVisibilityNotifications(ScopedGadget start, bool is_visible)
{
    for (auto iter = children_.begin(); iter != children_.end(); ++iter)
    {
        (*iter)->PropagateVisibilityNotifications(start, is_visible);
    }
    VisibilityChangedImpl(start, is_visible);
}

void Gadget::VisibilityChangedImpl(ScopedGadget starting_from, bool is_visible)
{
    OnVisibilityChanged(starting_from, is_visible);
}

void Gadget::PropagateRemoveNotifications(ScopedGadget & old_parent, ScopedGadget & new_parent)
{
    for (auto iter = children_.begin(); iter != children_.end(); ++iter)
    {
        (*iter)->PropagateRemoveNotifications(old_parent, new_parent);
    }

    HierarchyChangedDetails details(false, old_parent, ScopedGadget(this), new_parent);
    
    ScopedGadget tmp = this;
    while (tmp)
    {
        tmp->HierarchyChangedImpl(details);
        tmp = tmp->parent();
    }
}

void Gadget::HierarchyChangedImpl(const HierarchyChangedDetails & details)
{
    OnHierarchyChanged(details);
}


void Gadget::InitFocusSiblings(ScopedGadget & gadget, int index)
{
    int count = children_.size();
    if (count == 0)
    {
        gadget->next_focusable_ = nullptr;
        gadget->previous_focusable_ = nullptr;
    }
    else
    {
        if (index == count)
        {
            ScopedGadget last_focusable = nullptr;
            for (auto iter = children_.begin(); iter != children_.end(); ++iter)
            {
                if (!(*iter)->next_focusable_)
                {
                    last_focusable = *iter;
                    break;
                }
            }

            if (last_focusable == nullptr)
            {
                ScopedGadget prev = children_[index - 1];
                gadget->previous_focusable_ = prev;
                gadget->next_focusable_ = prev->next_focusable_;
                prev->next_focusable_->previous_focusable_ = gadget;
                prev->next_focusable_ = gadget;
            }
            else
            {
                last_focusable->next_focusable_ = gadget;
                gadget->next_focusable_ = nullptr;
                gadget->previous_focusable_ = last_focusable;
            }
        }
        else
        {
            ScopedGadget prev = children_[index]->GetPreviousFocusable();
            gadget->previous_focusable_ = prev;
            gadget->next_focusable_ = children_[index];
            if (prev)
                prev->next_focusable_ = gadget;
            children_[index]->previous_focusable_ = gadget;
        }
    }
}

void Gadget::Dispatch(GadgetEvent & e)
{
    if (!listerner_)
        return;

    EventHandler * handler = listerner_->Query(this, e);
    if (handler)
        handler->OnEvent(this, e);
}


void Gadget::AdvanceFocusIfNecessary()
{
    if (IsFocusable() || !HasFocus())
        return;

    auto root = GetRoot();
    FocusManager * focus_manager = root ? root->GetFocusManager() : nullptr;

    if (focus_manager)
        focus_manager->AdvanceFocusIfNecessary();
}

bool Gadget::HasFocus() const
{
    auto root = GetRoot();
    FocusManager * focus_manager = root ? root->GetFocusManager() : nullptr;
    return focus_manager && (focus_manager->GetFocusedGadget() == this);
}

bool Gadget::SkipDefaultKeyEventProcessing(const GadgetKeyEvent & key)
{
    return false;
}


}
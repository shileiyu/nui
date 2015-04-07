#ifndef NUI_GADGET_H_
#define NUI_GADGET_H_

#include <nui/nui.h>
#include <nui/base/bitwise_enum.h>
#include <nui/base/point.h>
#include <nui/base/size.h>
#include <nui/base/rect.h>
#include <nui/base/cursor_style.h>
#include "types.h"

namespace nui
{


class Painter;
class FocusTraversable;
class FocusManager;
class Layout;

class EventHandler
{
public:
    virtual void OnEvent(ScopedGadget gadget, Event & e) = 0;
};

class GadgetListener
{
public:
    virtual EventHandler * Query(ScopedGadget gadget, Event & e) = 0;
};

template <class T>
class EventAdapter : public EventHandler
{
    typedef void(T::*TF)(ScopedGadget gadget, Event & e);
public:
    void Register(T * obj, TF func)
    {
        obj_ = obj;
        func_ = func;
    }

    void OnEvent(ScopedGadget gadget, Event & e)
    {
        if (obj_ && func_)
            (obj_->*func_)(gadget, e);
    }
private:
    T * obj_;
    TF func_;
};

class Gadget : public RefCount
{
    struct _Flag
    {
        enum Enum
        {
            kVisible = 1,
            kFocusable = 2,
        };
    };

    using Flag = BitwiseEnum<_Flag>;
public:
    typedef std::vector<ScopedGadget> ScopedGadgets;

    struct HierarchyChangedDetails
    {
        HierarchyChangedDetails();

        HierarchyChangedDetails(bool, ScopedGadget & parent, ScopedGadget & child, ScopedGadget & move);

        bool is_add;
        ScopedGadget parent;
        ScopedGadget child;
        ScopedGadget move;
    };
public:
    Gadget();

    virtual ~Gadget();

    void SetLoc(const Point & pt);

    void SetSize(const Size & size);

    const Point & GetLoc() const;

    const Size & GetSize() const;

    int GetLeft() const;

    int GetTop() const;

    int GetWidth() const;

    int GetHeight() const;

    void SetVisible(bool visible);

    void SetFocusable(bool focusable);

    void SetPreferredSize(const Size & size);

    bool visible() const;

    bool focusable() const;

    bool Contains(const ScopedGadget gadget) const;

    void GetLocalBounds(Rect & rect) const;

    void GetGlobleBounds(Rect & rect) const;

    Point MapToGlobal(const Point & pt) const;

    const Size & GetPreferredSize() const;

    void Invalidate();

    ScopedGadget & parent();

    void AddChild(ScopedGadget gadget);

    void AddChildAt(ScopedGadget gadget, int index);

    void RemoveChild(ScopedGadget gadget);

    void RemoveChildren();

    void RemoveFromParent();

    void RecursiveRemoveChildren();

    size_t GetChildCount() const;

    ScopedGadget GetChild(int index);

    void SetGroup(int gid);

    int GetGroup() const;

    void GetGadgetsInGroup(int gid, ScopedGadgets & gadgets);

    virtual bool IsGroupFocusTraversable() const;

    virtual ScopedGadget GetSelectedForGroup(int gid);

    ScopedGadget & GetNextFocusable();

    ScopedGadget & GetPreviousFocusable();

    void Blink();

    virtual void Dispatch(Event & e);

    bool IsFocusable() const;

    void SetListener(GadgetListener * listener);

    void SetLayout(nui::Layout * layout);

    bool HasFocus() const;

    bool SkipDefaultKeyEventProcessing(const KeyEvent & key);

    virtual void Layout();

    virtual bool IsDrawn() const;

    virtual CursorStyles QueryCursor() const;

    virtual ScopedWorld GetRoot();

    virtual const ScopedWorld GetRoot() const;

    virtual void Invalidate(const Rect & inval);

    virtual void PreferredSizeChanged();
protected:
    void Draw(Painter & painter, const Rect & inval_rect);

    ScopedGadget HitTest(const Point & pt);
protected:
    virtual void OnChildPreferredSizeChanged(ScopedGadget child);

    virtual bool OnHitTest(const Point & pt);

    virtual void OnDraw(Painter & painter);

    virtual void OnSizeChanged();

    virtual void OnPositionChanged();

    virtual void OnHierarchyChanged(const HierarchyChangedDetails & details);

    virtual void OnVisibilityChanged(ScopedGadget starting_from, bool is_visible);
private:
    void DoRemoveChild(ScopedGadget & gadget, ScopedGadget & new_parent);

    void PropagateRemoveNotifications(ScopedGadget & old_parent, ScopedGadget & new_parent);

    void PropagateAddNotifications(const HierarchyChangedDetails & details);

    void PropagateVisibilityNotifications(ScopedGadget start, bool is_visible);

    void HierarchyChangedImpl(const HierarchyChangedDetails & details);

    void VisibilityChangedImpl(ScopedGadget starting_from, bool is_visible);

    void InitFocusSiblings(ScopedGadget & gadget, int index);

    void AdvanceFocusIfNecessary();
protected:
    Point loc_;                     //!<相对坐标
    Size size_;                     //!<尺寸
    Flag flag_;                     //!<标志,可见,启用,可接受焦点.
    ScopedGadget parent_;           //!<父|母
    ScopedGadgets children_;

    ScopedGadget next_focusable_;
    ScopedGadget previous_focusable_;

    int group_id_;

    GadgetListener * listerner_;

    nui::Layout * layout_;

    Size preferred_size_;
};

}
#endif
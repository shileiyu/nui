
#include <nui/gadget/gadget.h>
#include <nui/gadget/events.h>
#include <nui/gadget/gadget.h>
#include <nui/gadget/event_dispatch.h>
#include <nui/gadget/world.h>
#include <nui/base/rect.h>

namespace nui
{

typedef std::vector<ScopedGadget> GadgetEventPath;

static void GenerateGadgetEventPath(
    ScopedGadget target,
    GadgetEventPath & path
) 
{
    ScopedGadget node = target;
    while (node)
    {
        path.push_back(node);
        node = node->parent();
    }
}

void EventDispatch::Run(EventDispatch & dispatch, GadgetEvent & e)
{
    auto target = e.target();
    if (!target)
        return;

    GadgetEventPath path;
    path.reserve(8);
    GenerateGadgetEventPath(target, path);

    //capture阶段
    e.phase_ = kCapturing;
    for (auto item = path.rbegin(); item != path.rend(); ++item)
    {
        ScopedGadget & gadget = (*item);
        if (gadget != target)
            if(!dispatch.Pass(gadget, e))
                return;
    }

    //target
    e.phase_ = kAtTarget;
    if(!dispatch.Pass(target, e))
        return;

    //不可以冒泡
    if (!e.feature_.bubbles)
        return;

    //冒泡阶段
    e.phase_ = kBubbling;
    for (auto item = path.begin(); item != path.end(); ++item)
    {
        ScopedGadget & gadget = (*item);
        if (gadget != target)
            if(!dispatch.Pass(gadget, e))
                return;
    }
    return;
}

bool EventDispatch::Pass(ScopedGadget target, GadgetEvent & e)
{
    if (target)
        target->Dispatch(e);
    return e.status_.propagating;
}

void KeyEventDispatch::Run(GadgetKeyEvent & e)
{
    KeyEventDispatch dispatch;
    EventDispatch::Run(dispatch, e);
}

void FocusEventDispatch::Run(GadgetFocusEvent & e)
{
    FocusEventDispatch dispatch;
    EventDispatch::Run(dispatch, e);
}

void WheelEventDispatch::Run(GadgetWheelEvent & e)
{
    WheelEventDispatch dispatch;
    EventDispatch::Run(dispatch, e);
}

void MouseEventDispatch::Run(GadgetMouseEvent & e)
{
    MouseEventDispatch dispatch;
    EventDispatch::Run(dispatch, e);
}

bool MouseEventDispatch::Pass(ScopedGadget target, GadgetEvent & e)
{
    GadgetMouseEvent & globle = static_cast<GadgetMouseEvent&>(e);
    GadgetMouseEvent locale = globle;

    if (target )
    {
        Rect rect;
        target->GetGlobleBounds(rect);
        locale.x_ = globle.x_ - rect.left();
        locale.y_ = globle.y_ - rect.top();
        target->Dispatch(locale);
    }
    return locale.status_.propagating;
}

}
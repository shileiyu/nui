#include "mono.h"
#include <nui/base/size.h>
#include <nui/base/point.h>

namespace nui
{

MonoLayout::MonoLayout(){};

void MonoLayout::Attach(ScopedGadget target)
{
    target_ = target;
}

void MonoLayout::Detach()
{
    target_ = nullptr;
}

ScopedGadget MonoLayout::target()
{
    return target_;
}

AlignLayout::AlignLayout() : style_(kNoStyle){}

void AlignLayout::Set(Style style)
{
    style_ = style;
}

void AlignLayout::Arrange(ScopedGadget gadget)
{
    if (!target() || !gadget)
        return;

    Size parent_size = gadget->GetSize();

    Size target_size = target()->GetSize();

    Point target_point = target()->GetLoc();
    int left = target_point.x();
    int top = target_point.y();

    if (style_ & kHorizontal)
    {
        left = (parent_size.width() - target_size.width()) / 2;
    }
    if (style_ & kVertical)
    {
        top = (parent_size.height() - target_size.height()) / 2;
    }
    target()->SetLoc(Point(left, top));
}

}
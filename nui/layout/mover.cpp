#include "mover.h"
#include <nui/gadget/world.h>

namespace nui
{

inline static int ftoi(float number)
{
    return static_cast<int>(number);
}

Mover::Mover()
    : origin_(MoveOrigin::kDontMove)
{
};

void Mover::Arrange(ScopedGadget parent)
{
    auto & gadget = target();
    if (!parent || !gadget)
        return;

    Size size = parent->GetSize();
    Point point = parent->GetLoc();

    switch(origin_){
    case MoveOrigin::kLeftTop:
        point = LeftTop(size);
        break;
    case MoveOrigin::kTopRight:
        point = TopRight(size);
        break;
    case MoveOrigin::kRightBottom:
        point = RightBottom(size);
        break;
    case MoveOrigin::kBottomLeft:
        point = BottomLeft(size);
        break;
    }
    gadget->SetLoc(point);
}

ScaleMover::ScaleMover()
    : dx_(0.f), dy_(0.f)
{
};

void ScaleMover::Set(MoveOrigin origin, float dx, float dy)
{
    origin_ = origin;
    dx_ = dx;
    dy_ = dy;
}

Point ScaleMover::LeftTop(const Size & parent)
{
    int parent_width = parent.width(); 
    int parent_height = parent.height();
    int child_left = ftoi(parent_width * dx_);
    int child_top = ftoi(parent_height * dy_);
    return Point(child_left, child_top);
}

Point ScaleMover::TopRight(const Size & parent)
{
    auto & gadget = this->target();

    Size size = gadget->GetSize();
    int parent_width = parent.width(); 
    int parent_height = parent.height();
    int child_width = size.width();
    int child_left = ftoi((parent_width - child_width) * dx_);
    int child_top = ftoi(parent_height * dy_);
    return Point(child_left, child_top);
}

Point ScaleMover::RightBottom(const Size & parent)
{
    auto & gadget = this->target();

    int parent_width = parent.width(); 
    int parent_height = parent.height();
    Size child_size = gadget->GetSize();

    int child_width = child_size.width();
    int child_height = child_size.height();
    int child_left = ftoi((parent_width - child_width) * dx_);
    int child_top = ftoi((parent_height - child_height) * dy_);
    return Point(child_left, child_top);
}

Point ScaleMover::BottomLeft(const Size & parent)
{
    auto & gadget = this->target();
    Size size = gadget->GetSize();
    int parent_width = parent.width(); 
    int parent_height = parent.height();
    int child_height = size.height();
    int child_left = ftoi(parent_width * dx_);
    int child_top = ftoi((parent_height - child_height) * dy_);
    return Point(child_left, child_top);
}

PixelMover::PixelMover() 
    : dx_(0), dy_(0) 
{};

void PixelMover::Set(MoveOrigin origin, int dx, int dy)
{
    origin_ = origin;
    dx_ = dx;
    dy_ = dy;
}

Point PixelMover::LeftTop(const Size & parent)
{
    return Point(dx_, dy_);
}

Point PixelMover::TopRight(const Size & parent)
{
    auto & gadget = this->target();
    Size size = gadget->GetSize();
    int parent_width = parent.width();
    int child_width = size.width();
    int child_left = parent_width - child_width - dx_;
    return Point(child_left, dy_);
}

Point PixelMover::RightBottom(const Size & parent)
{
    auto & gadget = this->target();

    int parent_width = parent.width();
    int parent_height = parent.height();

    Size child_size = gadget->GetSize();

    int child_width = child_size.width();
    int child_height = child_size.height();
    int child_left = parent_width - dx_ - child_width;
    int child_top = parent_height - dy_ - child_height;

    return Point(child_left, child_top);
}

Point PixelMover::BottomLeft(const Size & parent)
{
    auto & gadget = this->target();
    Size child_size = gadget->GetSize();

    int parent_height = parent.height();
    int child_height = child_size.height();
    int child_top = parent_height - dy_ - child_height;
    return Point(dx_, child_height);
}

}
#include "sizer.h"
#include <nui/gadget/world.h>

namespace nui
{

inline static int ftoi(float number)
{
    return static_cast<int>(number);
}

Sizer::Sizer()
    : set_left_(false), set_top_(false),
      set_right_(false), set_bottom_(false)
{
};

void Sizer::Arrange(ScopedGadget parent)
{
    auto & gadget = target();
    if (!parent || !gadget)
        return;

    Size parent_size = parent->GetSize();
    Size gadget_size = gadget->GetSize();
    Point gadget_loc = gadget->GetLoc();

    int parent_width = parent_size.width();
    int parent_height = parent_size.height();
    int child_left = gadget_loc.x();
    int child_top = gadget_loc.y();
    int child_width = gadget_size.width();
    int child_height = gadget_size.height();
    int child_right = child_left + child_width;
    int child_bottom = child_top + child_height;

    
    child_left = set_left_  ? SetLeft(parent_width) : child_left;
    child_right = set_right_ ? SetRight(parent_width) : child_right;
    child_top = set_top_ ? SetTop(parent_height) : child_top ;
    child_bottom = set_bottom_ ? SetBottom(parent_height) : child_bottom;

    child_left = !set_left_  ? child_right - child_width : child_left;
    child_right = !set_right_ ? child_left + child_width : child_right;
    child_top = !set_top_ ? child_bottom - child_height : child_top ;
    child_bottom = !set_bottom_ ? child_top + child_height : child_bottom;

    child_width = child_right - child_left;
    child_height = child_bottom - child_top;
    child_width = child_width < 0 ? 0 : child_width;
    child_height = child_height < 0 ? 0 : child_height;

    gadget->SetLoc(Point(child_left, child_top));
    gadget->SetSize(Size(child_width, child_height));
}


ScaleSizer::ScaleSizer() 
    : left_(0), top_(0), right_(0), bottom_(0) 
{};


ScaleSizer & ScaleSizer::left(float left)
{
    left_ = left;
    set_left_ = true;
    return *this;
}

ScaleSizer & ScaleSizer::top(float top)
{
    top_ = top;
    set_top_ = true;
    return *this;
}

ScaleSizer & ScaleSizer::right(float right)
{
    right_ = right;
    set_right_ = true;
    return *this;
}

ScaleSizer & ScaleSizer::bottom(float bottom)
{
    bottom_ = bottom;
    set_bottom_ = true;
    return *this;
}

void ScaleSizer::Set(float left, float top, float right, float bottom)
{
    this->left(left).top(top).right(right).bottom(bottom);    
}

int ScaleSizer::SetLeft(int width)
{
    return ftoi(width * left_);
}

int ScaleSizer::SetRight(int width)
{
    return ftoi(width * (1 - right_));
}

int ScaleSizer::SetTop(int height)
{
    return ftoi(height * top_);
}

int ScaleSizer::SetBottom(int height)
{
    return ftoi(height * (1 - bottom_));
}

PixelSizer::PixelSizer()
    : left_(0), top_(0), right_(0), bottom_(0) 
{};

PixelSizer & PixelSizer::left(int left)
{
    left_ = left;
    set_left_ = true;
    return *this;
}

PixelSizer & PixelSizer::top(int top)
{
    top_ = top;
    set_top_ = true;
    return *this;
}

PixelSizer & PixelSizer::right(int right)
{
    right_ = right;
    set_right_ = true;
    return *this;
}

PixelSizer & PixelSizer::bottom(int bottom)
{
    bottom_ = bottom;
    set_bottom_ = true;
    return *this;
}

void PixelSizer::Set(int left, int top, int right, int bottom)
{
    this->left(left).top(top).right(right).bottom(bottom);
}

int PixelSizer::SetLeft(int width)
{
    return left_;
}

int PixelSizer::SetRight(int width)
{
    return width - right_;
}

int PixelSizer::SetTop(int height)
{
    return top_;
}

int PixelSizer::SetBottom(int height)
{
    return height - bottom_;
}

}
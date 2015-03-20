#include "size.h"

namespace nui
{
    

Size::Size()
    :width_(0),
    height_(0)
{
}

Size::Size(int width, int height)
    :width_(width),
    height_(height)
{
}

Size::~Size()
{
}

Size Size::Make(int width, int height)
{
    Size r(width, height);
    return r;
}

void Size::operator=(const Size & size)
{
    width_ = size.width_;
    height_ = size.height_;
}

bool Size::operator==(const Size & size) const
{
    return (width_ == size.width_) && (height_ == size.height_);
}

bool Size::operator!=(const Size & size) const
{
    return (width_ != size.width_) || (height_ != size.height_);
}

void Size::set_width(int width)
{
    width_ = width;
}

void Size::set_height(int height)
{
    height_ = height;
}

void Size::SetSize(int width, int height)
{
    width_ = width;
    height_ = height;
}

int Size::width() const
{
    return width_;
}

int Size::height() const
{
    return height_;
}


}
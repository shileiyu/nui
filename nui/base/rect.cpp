#include <algorithm>
#include "point.h"
#include "rect.h"

namespace nui
{


Rect Rect::Make(int x, int y, int width, int height)
{
    Rect r(x, y, width, height);
    return r;
}

Rect Rect::Make(const Size & size)
{
    Rect r(0, 0, size.width(), size.height());
    return r;
}

Rect Rect::Intersect(const Rect & r1, const Rect & r2)
{
    Rect r;
    r = r1;
    r.Intersect(r2);
    return r;
}

Rect Rect::Union(const Rect & r1, const Rect & r2)
{
    Rect r;
    r = r1;
    r.Union(r2);
    return r;
}

Rect Rect::Center(const Rect & r, int w, int h)
{
    int x = r.left() + (r.width() - w) / 2;
    int y = r.top() + (r.height() - h) / 2;

    Rect center;
    center.SetXYWH(x, y, w, h);
    return center;
}

Rect::Rect()
{
    left_ = 0;
    top_ = 0;
    right_ = 0;
    bottom_ = 0;
}

Rect::Rect(int x, int y,
    int width, int height)
{
    SetXYWH(x, y, width, height);
}

Rect::~Rect()
{
}

void Rect::operator=(const Rect & r)
{
    left_ = r.left();
    right_ = r.right();
    top_ = r.top();
    bottom_ = r.bottom();
}

bool Rect::operator==(const Rect & r) const
{
    return (left_ == r.left()) && (right_ == r.right()) && (top_ == r.top()) && (bottom_ == r.bottom());
}

bool Rect::operator!=(const Rect & r) const
{
    return (left_ != r.left()) || (right_ != r.right()) || (top_ != r.top()) || (bottom_ != r.bottom());
}

bool Rect::isEmpty() const
{
    bool zero_width = (right_ - left_) == 0;
    bool zero_height = (bottom_ - top_) == 0;

    return zero_width | zero_height;
}

bool Rect::isPointIn(const Point & pt) const
{
    return isPointIn(pt.x_, pt.y_);
}

bool Rect::isPointIn(int x, int y) const
{
    if( left_ <= x && 
        x < right_ &&
        top_ <= y && 
        y < bottom_ )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Rect::Intersect(const Rect & r)
{
    if( !isEmpty() && !r.isEmpty() &&
        r.left_ < r.right_ &&
        r.top_ < r.bottom_ &&
        left_ < r.right_ &&
        right_ > r.left_ &&
        top_ < r.bottom_ &&
        bottom_ > r.top_)
    {
        top_ = std::max(top_, r.top_);
        left_ = std::max(left_, r.left_);
        right_ = std::min(right_, r.right_);
        bottom_ = std::min(bottom_, r.bottom_);
    }
    else
    {
         top_ = 0;
         left_ = 0;
         right_ = 0;
         bottom_ = 0;
    }

    return;
}

void Rect::Union(const Rect & r)
{
    if(r.isEmpty())
    {
        //Do nothing because r is empty
    }
    else if(isEmpty())
    {
        top_ = r.top_;
        left_ = r.left_;
        right_ = r.right_;
        bottom_ = r.bottom_;
    }
    else
    {
        top_ = std::min(top_, r.top_);
        left_ = std::min(left_, r.left_);
        right_ = std::max(right_, r.right_);
        bottom_ = std::max(bottom_, r.bottom_);
    }

    return;
}

void Rect::SetXYWH(int x, int y, int width, int height)
{
    left_ = x;
    top_ = y;
    right_ = x + width;
    bottom_ = y + height; 
}

void Rect::SetLTRB(int left, int top, int right, int bottom)
{
    left_ = left;
    top_ = top;
    right_ = right;
    bottom_ = bottom;
}

void Rect::SetEmpty()
{
    SetLTRB(0,0,0,0);
}

int Rect::left() const
{
    return left_;
}

int Rect::top() const
{
    return top_;
}

int Rect::right() const
{
    return right_;
}

int Rect::bottom() const
{
    return bottom_;
}

int Rect::width() const
{
    return abs(right_ - left_);
}

int Rect::height() const
{
    return abs(bottom_ - top_);
}

Point Rect::LeftTop()
{
    return Point(left_, top_);
}

Size Rect::WidthHeight()
{
    return Size(width(), height());
}

void Rect::Offset(int dx, int dy)
{
    left_ += dx;
    right_ += dx;
    top_ += dy;
    bottom_ += dy;
}


}
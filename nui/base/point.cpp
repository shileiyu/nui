#include "point.h"

namespace nui
{


Point::Point()
    : x_(0),
      y_(0)
{
}

Point::Point(int x, int y)
{
    x_ = x;
    y_ = y;
}

Point::Point(const Point& pt)
{
    x_ = pt.x_;
    y_ = pt.y_;
}

Point::~Point()
{
}

Point Point::Make(int x, int y)
{
    Point r(x, y);
    return r;
}

void Point::operator=(const Point& pt)
{
    x_ = pt.x_;
    y_ = pt.y_;
}

bool Point::operator==(const Point& pt) const
{
    return (x_ == pt.x_) && (y_ == pt.y_) ;
}

bool Point::operator!=(const Point& pt) const
{
    return (x_ != pt.x_) || (y_ != pt.y_) ;
}

Point Point::operator-(const Point & pt) const
{
    Point result(*this);
    result.Offset(-pt.x(), -pt.y());
    return result;
}

Point Point::operator + (const Point & pt) const
{
    Point result(*this);
    result.Offset(pt.x(), pt.y());
    return result;
}

void Point::set_x(int x)
{
    x_ = x;
}

void Point::set_y(int y)
{
    y_ = y;
}

void Point::Set(int x, int y)
{
    x_ = x;
    y_ = y;
}

int Point::x() const
{
    return x_;
}

int Point::y() const
{
    return y_;
}

void Point::Offset(int dx, int dy)
{
    x_ += dx;
    y_ += dy;
}

}
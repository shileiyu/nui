#ifndef NUI_BASE_POINT_H_
#define NUI_BASE_POINT_H_

#include <nui/nui.h>

namespace nui
{


class Point
{
    friend class Rect;
public:
    static Point Make(int x, int y);
public:
    Point();
    Point(int x, int y);
    Point(const Point& pt);
    ~Point();
    void operator=(const Point& pt);
    bool operator==(const Point& pt) const;
    bool operator!=(const Point& pt) const;

    Point operator-(const Point & pt) const;
    Point operator + (const Point & pt) const;

    void set_x(int x);
    void set_y(int y);
    void Set(int x, int y);

    int x() const;
    int y() const;

    void Offset(int dx, int dy);
private:
    int x_;
    int y_;
};


}
#endif

#ifndef NCORE_BASE_RECT_H_
#define NCORE_BASE_RECT_H_

#include <nui/nui.h>
#include "point.h"
#include "size.h"

namespace nui
{


class Point;

class Rect
{
public:
    static Rect Make(int x, int y, int width, int height);
    static Rect Make(const Size & size);
    static Rect Intersect(const Rect & r1, const Rect & r2);
    static Rect Union(const Rect & r1, const Rect & r2);
    static Rect Center(const Rect & r, int w, int h);
public:
    Rect();
    Rect(int x, int y, int width, int height);
    ~Rect();

    void operator=(const Rect & r);
    bool operator==(const Rect & r) const;
    bool operator!=(const Rect & r) const;

    bool isEmpty() const;

    bool isPointIn(const Point & pt) const;
    bool isPointIn(int x, int y) const;
            
    void Intersect(const Rect & r);
    void Union(const Rect & r);

    void SetXYWH(int x, int y, int width, int height);
    void SetLTRB(int left, int top, int right, int bottom);
    void SetEmpty();

    int left() const;
    int top() const;
    int right() const;
    int bottom() const;
    int width() const;
    int height() const;

    Point LeftTop();
    Size WidthHeight();

    void Offset(int dx, int dy);

private:
    int left_;
    int top_;
    int right_;
    int bottom_;
};


}
#endif
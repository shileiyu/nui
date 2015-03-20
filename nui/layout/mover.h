#ifndef NUI_LAYOUT_MOVER_H_
#define NUI_LAYOUT_MOVER_H_

#include <nui/base/size.h>
#include <nui/base/point.h>
#include "mono.h"

namespace nui
{

enum class MoveOrigin
{
    kDontMove,
    kLeftTop,
    kTopRight,
    kRightBottom,
    kBottomLeft,
};

class Mover : public MonoLayout
{
public:
    Mover();

    void Arrange(ScopedGadget wrap) override;

protected:
    virtual Point LeftTop(const Size & parent) = 0;
    virtual Point TopRight(const Size & parent) = 0;
    virtual Point RightBottom(const Size & parent) = 0;
    virtual Point BottomLeft(const Size & parent) = 0;
protected:
    MoveOrigin origin_;
};

class ScaleMover : public Mover
{
public:
    ScaleMover();
    void Set(MoveOrigin origin, float dx, float dy);
    
private:
    Point LeftTop(const Size & parent);
    Point TopRight(const Size & parent);
    Point RightBottom(const Size & parent);
    Point BottomLeft(const Size & parent);
private:
    float dx_;
    float dy_;
};

class PixelMover : public Mover
{
public:
    PixelMover();
    void Set(MoveOrigin origin, int dx, int dy);
    
private:
    Point LeftTop(const Size & parent);
    Point TopRight(const Size & parent);
    Point RightBottom(const Size & parent);
    Point BottomLeft(const Size & parent);
private:
    int dx_;
    int dy_;
};

}

#endif
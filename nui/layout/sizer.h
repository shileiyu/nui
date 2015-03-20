#ifndef NUI_LAYOUT_SIZER_H_
#define NUI_LAYOUT_SIZER_H_

#include <nui/base/bitwise_enum.h>
#include "mono.h"

namespace nui
{

class Sizer : public MonoLayout
{
public:
    Sizer();

    void Arrange(ScopedGadget parent) override;
protected:
    virtual int SetLeft(int parent_width) = 0;
    virtual int SetRight(int parent_width) = 0;
    virtual int SetTop(int parent_height) = 0;
    virtual int SetBottom(int parent_height) = 0;
protected:    
    bool set_left_;
    bool set_right_;
    bool set_top_;
    bool set_bottom_;
};

class ScaleSizer : public Sizer
{
public:
    ScaleSizer();

    ScaleSizer & left(float left);

    ScaleSizer & top(float top);

    ScaleSizer & right(float right);

    ScaleSizer & bottom(float bottom);

    void Set(float left, float top, float right, float bottom);

protected:
    int SetLeft(int parent_width);
    int SetRight(int parent_width);
    int SetTop(int parent_height);
    int SetBottom(int parent_height);

private:
    float left_;
    float top_;
    float right_;
    float bottom_;

};

class PixelSizer : public Sizer
{
public:
    PixelSizer();

    PixelSizer & left(int left);

    PixelSizer & top(int top);

    PixelSizer & right(int right);

    PixelSizer & bottom(int bottom);

    void Set(int left, int top, int right, int bottom);

protected:
    int SetLeft(int parent_width);
    int SetRight(int parent_width);
    int SetTop(int parent_height);
    int SetBottom(int parent_height);

private:
    int left_;
    int top_;
    int right_;
    int bottom_;
};

}
#endif
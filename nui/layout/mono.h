#ifndef NUI_LAYOUT_MONO_H_
#define NUI_LAYOUT_MONO_H_

#include <nui/layout/layout.h>

namespace nui
{

class GadgetWorld;

class MonoLayout : public Layout
{
public:
    MonoLayout();

    void Attach(ScopedGadget target);

    void Detach();

protected:
    ScopedGadget target();
private:
    ScopedGadget target_;
};

class AlignLayout : public MonoLayout
{
public:
    enum Style
    {
        kNoStyle = 0,
        kHorizontal = 1,
        kVertical = 2,
    };

    AlignLayout();

    void Set(Style style);

    void Arrange(ScopedGadget wrap);
private:
    Style style_;
};

}

#endif
#ifndef NUI_BASE_LAYOUT_H_
#define NUI_BASE_LAYOUT_H_

#include <nui/nui.h>
#include <nui/gadget/types.h>
#include <nui/gadget/gadget.h>

namespace nui
{

class Layout
{
public:
    virtual void Arrange(ScopedGadget gadget) = 0;
};

}

#endif
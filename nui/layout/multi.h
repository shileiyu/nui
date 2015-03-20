#ifndef NUI_LAYOUT_MULTI_H_
#define NUI_LAYOUT_MULTI_H_

#include <nui/nui.h>
#include "layout.h"

namespace nui
{

class MultiLayout : public Layout
{
public:
    void Attach(Layout * layout);

    void Detach(Layout * layout);

    void Arrange(ScopedGadget gadget);

    void DetachAll();
private:
    std::set<Layout*> layouts_;
};

}

#endif
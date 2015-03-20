#include "multi.h"

namespace nui
{

void MultiLayout::Attach(Layout * layout)
{
    layouts_.insert(layout);
}

void MultiLayout::Detach(Layout * layout)
{
    layouts_.erase(layout);
}

void MultiLayout::Arrange(ScopedGadget gadget)
{
    for(auto item = layouts_.begin(); item != layouts_.end(); ++item)
    {
        (*item)->Arrange(gadget);
    }
}

void MultiLayout::DetachAll()
{
    layouts_.clear();
}

}
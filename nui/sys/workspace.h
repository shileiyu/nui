#ifndef NUI_WORKSPACE_H_
#define NUI_WORKSPACE_H_

#include <nui/nui.h>
#include <nui/base/rect.h>

namespace nui
{

class WorkSpace
{
public:
    static Rect GetPrimaryWorkArea();
};

}


#endif
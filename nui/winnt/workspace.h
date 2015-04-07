#ifndef NUI_WINNT_WORKSPACE_H_
#define NUI_WINNT_WORKSPACE_H_

#include <nui/nui.h>
#include <nui/base/rect.h>

namespace nui
{

namespace winnt
{

   
class WorkSpace
{
public:
    static Rect GetPrimaryWorkArea();
};

}

}


#endif
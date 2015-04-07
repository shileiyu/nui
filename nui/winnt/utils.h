#ifndef NUI_WINNT_UTILS_H_
#define NUI_WINNT_UTILS_H_

#include <nui/nui.h>

namespace nui
{

class Pixmap;

namespace winnt
{




class Utils
{
public:
    static HMODULE CurrentModule();

    static void Present(HDC hdc, Pixmap & pm);
};

}

}

#endif
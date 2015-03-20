#ifndef NUI_WIN_UTILS_H_
#define NUI_WIN_UTILS_H_

#include <nui/nui.h>

namespace nui
{

class Pixmap;

class Win32Utils
{
public:
    static HMODULE CurrentModule();

    static void Present(HDC hdc, Pixmap & pm);
};

}

#endif
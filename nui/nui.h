#ifndef NUI_H_
#define NUI_H_

#if defined(_MSC_VER)
#define NUI_WINDOWS
#endif

#ifdef NUI_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <dwmapi.h>
#endif


#include <assert.h>
#include <stdint.h>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <memory>
#include <map>
#include <set>
#include <stack>


namespace nui
{
#ifdef NUI_WINDOWS
typedef HICON IconHandle;
typedef HCURSOR CursorHandle;
typedef HWND WindowHandle;
typedef HDC PaintContext;
#endif
}





#endif
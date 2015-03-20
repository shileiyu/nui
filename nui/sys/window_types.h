#ifndef NUI_WIN_WINDOW_TYPES_H_
#define NUI_WIN_WINDOW_TYPES_H_

#include <nui/nui.h>

namespace nui
{

typedef const wchar_t * ConstWidePtr;

enum class ActivateParam
{
    kActive,
    kActiveByClick,
};

enum class WindowIconSize
{
    kLarge,
    kSmall,
};

enum class WindowPart
{
    kClient = HTCLIENT,
    kBorder = HTBORDER,
    kCaption = HTCAPTION,
    kSouthEast = HTBOTTOMRIGHT,
};

}

#endif
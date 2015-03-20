#ifndef NUI_MOUSE_H_
#define NUI_MOUSE_H_

#include <nui/nui.h>
#include <nui/base/bitwise_enum.h>

namespace nui
{

enum class MouseButton
{
    kNone = -1,
    kPrimary = 0,
    kSecondry = 1,
    kAuxiliary = 2,
};

struct _MouseState
{
    enum Enum
    {
        kPressedNone = 0,
        kPressedPrimary = 1,
        kPressedSecondary = 2,
        kPressedAuxiliary = 4,
        kPressedXButton1 = 8,
        kPressedXButton2 = 16,
    };
};

using MouseState = BitwiseEnum<_MouseState>;

struct _ExtendedKeyState
{
    enum Eum
    {
        kPressedNone = 0,
        kPressedCtrl = 1,
        kPressedShift = 2,
        kPressedAlt = 4,
        kPressedMeta = 8,
    };
};

using ExtendedKeyState = BitwiseEnum<_ExtendedKeyState>;

}

#endif
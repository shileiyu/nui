#include <nui/sys/utils.h>
#include <nui/sys/icon.h>

namespace nui
{

Icon::Icon()
    : icon_(0)
{

}

Icon::~Icon()
{
    Dispose();
}

bool Icon::LoadFromFile(const Size & desired, const wchar_t * file)
{
    return false;
}

bool Icon::LoadFromResource(const Size & desired, const wchar_t * name)
{
    return false;
}

bool Icon::LoadFromResource(const Size & desired, int id)
{
    auto module = Win32Utils::CurrentModule();
    auto name = MAKEINTRESOURCE(id);
    int cx = desired.width();
    int cy = desired.height();
    icon_ = (HICON)LoadImage(module, name, IMAGE_ICON, cx, cy, 0);
    return icon_ != NULL;
}

void Icon::Dispose()
{
    if (icon_) {
        DestroyIcon(icon_);
        icon_ = 0;
    }
}

IconHandle Icon::NativeHandle() const
{
    return icon_;
}

}
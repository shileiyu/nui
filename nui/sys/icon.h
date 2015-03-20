#ifndef NUI_ICON_H_
#define NUI_ICON_H_

#include <nui/nui.h>
#include <nui/base/size.h>

namespace nui
{


class Icon
{
public:
    Icon();
    ~Icon();

    bool LoadFromFile(const Size & desired, const wchar_t * file);
    bool LoadFromResource(const Size & desired, const wchar_t * name);
    bool LoadFromResource(const Size & desired, int id);

    void Dispose();

    IconHandle NativeHandle() const;
private:
    IconHandle icon_;
};

}

#endif
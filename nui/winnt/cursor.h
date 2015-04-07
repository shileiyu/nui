#ifndef NUI_WINNT_CURSOR_H_
#define NUI_WINNT_CURSOR_H_

#include <nui/nui.h>
#include <nui/base/cursor_style.h>

namespace nui
{

namespace winnt
{


class Cursor
{
public:
    static Cursor SetCursor(Cursor & cursor);
    
    Cursor();
    Cursor(const Cursor & obj);
    
    ~Cursor();
    
    bool Load(CursorStyles style);
    void Dispose();

private:
    HCURSOR cursor_;
    bool owned_;
};


}

}

#endif

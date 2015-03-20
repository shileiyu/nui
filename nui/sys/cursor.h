#ifndef NUI_CURSOR_H_
#define NUI_CURSOR_H_

#include <nui/nui.h>
#include <nui/base/cursor_style.h>

namespace nui
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
    CursorHandle cursor_;
    bool owned_;
};

}

#endif

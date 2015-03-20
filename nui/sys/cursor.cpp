#include <nui/sys/utils.h>
#include <nui/sys/cursor.h>

namespace nui
{

static LPCTSTR StyleToResource(CursorStyles style)
{
    switch (style) {
    case CursorStyles::kArrow:
        return IDC_ARROW;    
    case CursorStyles::kHand:
        return IDC_HAND;
    case CursorStyles::kIBeam:
        return IDC_IBEAM;
    case CursorStyles::kWait:
        return IDC_WAIT;
    case CursorStyles::kForbid:
        return IDC_NO;
    case CursorStyles::kSizeHorizon:
        return IDC_SIZEWE;
    case CursorStyles::kSizeVertical:
        return IDC_SIZENS;
    case CursorStyles::kSizeNWSE:
        return IDC_SIZENWSE;
    case CursorStyles::kSizeNESW:
        return IDC_SIZENESW;
    default:
        return 0;
    }
}

Cursor Cursor::SetCursor(Cursor & cursor)
{
    Cursor prev;
    prev.cursor_ = ::SetCursor(cursor.cursor_);
    return prev;
}

Cursor::Cursor()
: cursor_(0), owned_(false)
{

}

Cursor::Cursor(const Cursor & obj)
{
    cursor_ = obj.cursor_;
    owned_ = false;
}

Cursor::~Cursor()
{
    Dispose();
}

bool Cursor::Load(CursorStyles style)
{
    Dispose();
    LPCTSTR name = StyleToResource(style);
    uint32_t flags = LR_DEFAULTSIZE | LR_SHARED;
    cursor_ = (HCURSOR)LoadImage(0, name, IMAGE_CURSOR, 0, 0, flags);
    return cursor_ != 0;
}

void Cursor::Dispose()
{
    if (owned_ && cursor_) {
        DestroyCursor(cursor_);
        cursor_ = 0;
    }
}

}

#include <nui/winnt/window_events.h>

namespace nui
{

namespace winnt
{

static int16_t HiWord(uint32_t value)
{
    return static_cast<int16_t>(value >> 16);
}

static int16_t LoWord(uint32_t value)
{
    return static_cast<int16_t>(value);
}

WindowEvent::WindowEvent()
{
    this->type = 0;
    this->wparam = 0;
    this->lparam = 0;
    this->result = 0;
    this->handled = false;
}

WindowEvent::WindowEvent(uint32_t type, WPARAM wparam, LPARAM lparam)
{
    this->type = type;
    this->wparam = wparam;
    this->lparam = lparam;
    this->result = 0;
    this->handled = false;
}

void WindowEvent::PreventDefault()
{
    handled = true;
}

bool WindowEvent::DefaultPrevented() const
{
    return !handled;
}

void WindowCreatingEvent::Cancel()
{
    PreventDefault();
    result = -1;
}

static WINDOWPOS * LParamToWindowPos(LPARAM lparam)
{
    return reinterpret_cast<WINDOWPOS *>(lparam);
}

Point WindowGeometryChangedEvent::GetOrigin() const
{
    auto position = LParamToWindowPos(lparam);
    return Point(position->x, position->y);
}

Size WindowGeometryChangedEvent::GetSize() const
{
    auto position = LParamToWindowPos(lparam);
    return Size(position->cx, position->cy);
}


void WindowGeometryChangingEvent::SetOrigin(const nui::Point & point)
{
    auto position = LParamToWindowPos(lparam);
    position->x = point.x();
    position->y = point.y();
}

void WindowGeometryChangingEvent::SetSize(const nui::Size & size)
{
    auto position = LParamToWindowPos(lparam);
    position->cx = size.width();
    position->cy = size.height();
}

Point WindowHitTestEvent::Position() const
{
    int16_t x = LoWord(lparam);
    int16_t y = HiWord(lparam);
    return Point(x, y);
}

void WindowHitTestEvent::SetResult(WindowPart part)
{
    result = (LRESULT)part;
}

}

}
#ifndef NUI_WIN_WINDOW_EVENTS_H_
#define NUI_WIN_WINDOW_EVENTS_H_

#include <nui/nui.h>
#include <nui/base/point.h>
#include <nui/base/size.h>
#include <nui/sys/window_types.h>

namespace nui
{

struct WindowEvent
{
    uint32_t type;
    WPARAM wparam;
    LPARAM lparam;
    LRESULT result;
    bool handled;

    WindowEvent();

    WindowEvent(uint32_t type, WPARAM wparam, LPARAM lparam);

    void PreventDefault();

    bool DefaultPrevented() const;
};

struct WindowCreatingEvent : WindowEvent
{
    void Cancel();
};

struct WindowClosingEvent : WindowEvent
{
};

struct WindowGeometryChangedEvent : WindowEvent 
{
    Point GetOrigin() const;
    Size GetSize() const;
};

struct WindowGeometryChangingEvent : WindowEvent
{
    void SetOrigin(const nui::Point & point);
    void SetSize(const nui::Size & size);
};

struct WindowHitTestEvent : WindowEvent
{
    Point Position() const;
    void SetResult(WindowPart part);
};

}


#endif
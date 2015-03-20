#ifndef NUI_WIN_VIEW_H_
#define NUI_WIN_VIEW_H_

#include <nui/nui.h>
#include <nui/base/point.h>
#include <nui/base/size.h>
#include <nui/base/rect.h>
#include <nui/base/point.h>
#include <nui/base/mouse.h>
#include <nui/sys/keyboard.h>
#include <nui/sys/window_events.h>

namespace nui
{

class NativeWindow;
class Window;
class WindowClient;
class Icon;
class Cursor;


class NativeWindow
{
public:
    static bool DWMEnabled();

    static NativeWindow Wrap(WindowHandle handle);

    NativeWindow();

    ~NativeWindow();

    bool IsValid();

    bool Destroy();

    bool Show();

    bool Hide();

    bool ShowNormal();

    bool Restore();

    bool Maximize();

    bool Minimize();

    bool Close();

    bool GetClientRect(Rect & rect);

    bool GetWindowRect(Rect & rect);

    bool Minimized();

    bool Maximized();

    bool SetPos(int x, int y);

    bool SetPos(const Point & pos);

    bool SetSize(int cx, int cy);

    bool SetSize(const Size & size);

    bool InvalidateRect(bool erase);

    bool InvalidateRect(const Rect & rect, bool erase);

    void SetCapture();

    void ReleaseCapture();

    bool SetFocus();

    bool EnableWindow(bool enable);

    bool HasFocus() const;

    bool BringToForeground();

    bool IsVisible();

    bool IsEnabled();

    NativeWindow GetTopOwnedWindow();

    bool Post(const WindowEvent & e);

    bool Send(WindowEvent & e);

    WindowHandle NativeHandle() const;

    bool ExtendFrameIntoClientArea(int left, int right, int top, int bottom);

    void SetIcon(WindowIconSize size, const Icon & icon);

    bool ScreenToClient(Point & pt);
protected:
    WindowHandle handle_;
};

struct WindowArgs
{
    Point origin;
    Size size;
    ConstWidePtr window_name;
    uint32_t window_style;
    uint32_t window_style_ex;
    NativeWindow * parent_window;
    WindowClient * client;

    WindowArgs();
};

class Window : public NativeWindow
{
    friend class WindowStaticProc;
public:
    static bool Initialize();
   
    static void UnInitialize();
public:
    Window();

    ~Window();

    bool Create(const WindowArgs & args);

private:
    bool tracking_mouse_;
};


class WindowClient
{
    friend class StaticWndProc;
public:
    virtual void Preprocess(Window & window, WindowEvent & e) {};
    virtual void Postprocess(Window & window, WindowEvent & e) {};


    virtual void OnCreating(Window & window, WindowCreatingEvent & e) {};
    virtual void OnDestroy(Window & window) {};
    virtual void OnClose(Window & window, WindowClosingEvent & e) {};

    virtual void OnMouseMove(
        Window & window,
        MouseState state,
        Point & point
    ) {};

    virtual void OnMouseDoubleClick(
        Window & window,
        MouseButton button,
        MouseState state,
        Point & point
    ) {};

    virtual void OnMouseUp(
        Window & window,
        MouseButton button,
        MouseState state,
        Point & point
    ) {};

    virtual void OnMouseDown(
        Window & window,
        MouseButton button,
        MouseState state,
        Point & point
    ) {};

    virtual void OnMouseOver(
        Window & window,        
        MouseState state,
        Point & point
    ) {};

    virtual void OnMouseOut(
        Window & window
    ) {};

    virtual void OnMouseWheel(
        Window & window,
        MouseState state,
        Point & point,
        Point & offset
    ) {};

    virtual void OnKeyUp(
        Window & window,
        KeyCode key,
        KeyState state
    ) {};

    virtual void OnKeyDown(
        Window & window,
        KeyCode key,
        KeyState state
    ) {};

    virtual void OnPaint(
        Window & window,
        PaintContext ctx,
        Rect & dirty
    ) {};

    virtual void OnGeometryChanging(
        Window & window,
        WindowGeometryChangingEvent & e
    ) {};

    virtual void OnGeometryChanged(
        Window & window,
        WindowGeometryChangedEvent & e
    ) {};

    virtual void OnTime(Window & window, uint32_t timer) {};

    virtual void QueryBounds(
        Window & window,
        Size & max_size,
        Point & max_pos,
        Size & min_track_size,
        Size & max_track_size
    ) {};

    virtual void QueryFrameSize(
        Window & window,
        int32_t & left,
        int32_t & top,
        int32_t & right,
        int32_t & bottom
    ) {};

    virtual void OnActivated(Window & window, ActivateParam param) {};

    virtual void OnDeactivated(Window & window) {};

    virtual void OnSetFocus(Window & window, NativeWindow & from) {};

    virtual void OnKillFocus(Window & window, NativeWindow & to) {};

    virtual void OnEnabled(Window & window) {};

    virtual void OnDisabled(Window & window) {};

    virtual void QueryCursor(Window & window, NativeWindow & target ) {};

    virtual void OnHitTest(Window & window, WindowHitTestEvent & e) {};
};


}

#endif
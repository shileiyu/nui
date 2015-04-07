#include <nui/winnt/icon.h>
#include <nui/winnt/utils.h>
#include <nui/winnt/window.h>

namespace nui
{

namespace winnt
{


struct WindowInstance
{
    Window * window;
    WindowClient * client;
};

static const wchar_t * kDefaultClassName = L"__NGUI_WINDOW__";

static RECT Rect2RECT(const Rect & rect)
{
    return { rect.left(), rect.top(), rect.right(), rect.bottom() };
}

static Rect RECT2Rect(const RECT & source)
{
    Rect rect;
    rect.SetLTRB(source.left, source.top, source.right, source.bottom);
    return rect;
}

static POINT Point2POINT(const Point & pt)
{
    POINT point;
    point.x = pt.x();
    point.y = pt.y();
    return point;
}

static Point POINT2Point(const POINT & pt)
{
    return Point(pt.x, pt.y);
}

static WindowInstance GetWindowInstanceFromParam(LPARAM param)
{
    auto data = reinterpret_cast<CREATESTRUCT*>(param);
    return *reinterpret_cast<WindowInstance *>(data->lpCreateParams);
}

static std::unordered_map<HWND, WindowInstance> instances;


static int16_t HiWord(uint32_t value)
{
    return static_cast<int16_t>(value >> 16);
}

static int16_t LoWord(uint32_t value)
{
    return static_cast<int16_t>(value);
}

static bool KeyPressed(int vcode)
{
    const uint16_t kMSB = 0x8000;
    return (::GetAsyncKeyState(vcode) & kMSB) != 0;
}

static bool ShiftPressed()
{
    bool left = KeyPressed(VK_LSHIFT);
    bool right = KeyPressed(VK_RSHIFT);
    return left || right;
}

static bool CtrlPressed()
{
    bool left = KeyPressed(VK_LCONTROL);
    bool right = KeyPressed(VK_RCONTROL);
    return left || right;
}

static bool AltPressed()
{
    bool left = KeyPressed(VK_LMENU);
    bool right = KeyPressed(VK_RMENU);
    return left || right;
}

bool NativeWindow::DWMEnabled()
{
    static const HMODULE dwm_moudle = LoadLibrary(L"dwmapi.dll");

    if (dwm_moudle)
    {
        BOOL value = TRUE;
        DwmIsCompositionEnabled(&value);
        return value == TRUE;
    }
    else
    {
        return false;
    }
}

NativeWindow NativeWindow::Wrap(HWND handle)
{
    NativeWindow wrapper;
    if( ::IsWindow(handle) || 
        handle == HWND_BROADCAST ||
        handle == HWND_MESSAGE )
    {
        wrapper.handle_ = handle;
    }
    return wrapper;
}

NativeWindow::NativeWindow()
{
    handle_ = 0;
}

NativeWindow::~NativeWindow()
{
}

bool NativeWindow::IsValid()
{
    return handle_ != NULL;
}

bool NativeWindow::Show()
{
    assert(handle_);
    return !!::ShowWindow(handle_, SW_SHOW);
}

bool NativeWindow::Hide()
{   
    assert(handle_);
    return !!::ShowWindow(handle_, SW_HIDE);
}

bool NativeWindow::ShowNormal()
{
    assert(handle_);
    return !!::ShowWindow(handle_, SW_SHOWNORMAL);
}

bool NativeWindow::Restore()
{
    assert(handle_);
    return !!::ShowWindow(handle_, SW_RESTORE);
}

bool NativeWindow::Maximize()
{
    assert(handle_);
    return !!::ShowWindow(handle_, SW_SHOWMAXIMIZED);
}

bool NativeWindow::Minimize()
{
    assert(handle_);
    return !!::ShowWindow(handle_, SW_SHOWMINIMIZED);
}

bool NativeWindow::Destroy()
{
    bool result = false;
    if( handle_ != 0 && 
        handle_ != HWND_BROADCAST && 
        handle_ != HWND_MESSAGE)
    {
        result = !!::DestroyWindow(handle_);
        handle_ = 0;
    }
    return result;;
}

bool NativeWindow::Close()
{
    assert(handle_);
    return !!::CloseWindow(handle_);
}

bool NativeWindow::GetClientRect(Rect & rect)
{
    RECT client_rect;
    bool result = false;

    assert(handle_);

    result = !!::GetClientRect(handle_, &client_rect);
    rect = RECT2Rect(client_rect);
    return result;
}

bool NativeWindow::GetWindowRect(Rect & rect)
{
    RECT window_rect;
    bool result = false;

    assert(handle_);

    result = !!::GetWindowRect(handle_, &window_rect);
    rect = RECT2Rect(window_rect);
    return result;
}

bool NativeWindow::Minimized()
{
    assert(handle_);
    return !!::IsIconic(handle_);
}

bool NativeWindow::Maximized()
{
    assert(handle_);
    return !!::IsZoomed(handle_);
}

bool NativeWindow::SetPos(const Point & pos)
{
    return SetPos(pos.x(), pos.y());
}

bool NativeWindow::SetPos(int x, int y)
{
    uint32_t flag = SWP_NOSIZE | SWP_NOZORDER | 
                    SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS;
    assert(handle_);
    return !!::SetWindowPos(handle_, 0, x, y, 0, 0, flag);
}

bool NativeWindow::SetSize(const Size & size)
{
    return SetSize(size.width(), size.height());
}

bool NativeWindow::SetSize(int cx, int cy)
{
    uint32_t flag = SWP_NOMOVE | SWP_NOZORDER | 
                    SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS;

    assert(handle_);
    return !!::SetWindowPos(handle_, 0, 0, 0, cx, cy, flag);
}

bool NativeWindow::InvalidateRect(bool erase)
{
    assert(handle_);
    return !!::InvalidateRect(handle_, 0, erase);
}

bool NativeWindow::InvalidateRect(const Rect & rect, bool erase)
{
    assert(handle_);

    RECT r = Rect2RECT(rect);
    return !!::InvalidateRect(handle_, &r, erase);
}

void NativeWindow::SetCapture()
{
    assert(handle_);
    ::SetCapture(handle_);
}

void NativeWindow::ReleaseCapture()
{
    assert(handle_);
    ::ReleaseCapture();
}

bool NativeWindow::SetFocus()
{
    assert(handle_);
    return ::SetFocus(handle_) != NULL;
}

bool NativeWindow::EnableWindow(bool enable)
{
    assert(handle_);
    return !!::EnableWindow(handle_, enable);
}

bool NativeWindow::HasFocus() const
{
    assert(handle_);
    return ::GetFocus() == handle_;
}

bool NativeWindow::BringToForeground()
{
    assert(handle_);
    return !!::SetForegroundWindow(handle_);
}

bool NativeWindow::IsVisible()
{
    assert(handle_);
    return !!IsWindowVisible(handle_);
    
}

bool NativeWindow::IsEnabled()
{
    assert(handle_);
    return !!IsWindowEnabled(handle_);
}

NativeWindow NativeWindow::GetTopOwnedWindow()
{
    assert(handle_);
    HWND popup = GetWindow(handle_, GW_ENABLEDPOPUP);
    HWND first = GetWindow(popup, GW_HWNDFIRST);
    return NativeWindow::Wrap(first);
}

HWND NativeWindow::NativeHandle() const
{
    return handle_;
}

bool NativeWindow::Post(const WindowEvent & e)
{
    assert(handle_);
    return !!::PostMessage(handle_, e.type, e.wparam, e.lparam);
}

bool NativeWindow::Send(WindowEvent & e)
{
    assert(handle_);
    bool succeed = true;
    e.result = ::SendMessage(handle_, e.type, e.wparam, e.lparam);
    uint32_t error = ::GetLastError();
    switch (error) {
    case ERROR_ACCESS_DENIED:
    case ERROR_INVALID_WINDOW_HANDLE:
        succeed = false;
    }
    return succeed;
}

bool NativeWindow::ExtendFrameIntoClientArea(
    int left, int right, int top, int bottom
) {
    if (!DWMEnabled())
        return false;

    MARGINS margins = { left, right, top, bottom};
    HRESULT hr = DwmExtendFrameIntoClientArea(handle_, &margins);

    return SUCCEEDED(hr);
}

void NativeWindow::SetIcon(WindowIconSize size, const Icon & icon)
{
    WindowEvent e;
    e.type = WM_SETICON;
    e.wparam = size == WindowIconSize::kLarge ? 1 : 0;
    e.lparam = (LPARAM)icon.NativeHandle();
    Send(e);
    e.wparam = 1;
    Send(e);
}

bool NativeWindow::ScreenToClient(Point & pt)
{
    assert(handle_);
    POINT point = Point2POINT(pt);
    if(!MapWindowPoints(0, handle_, &point, 1))
        return false;
    pt = POINT2Point(point);
    return true;
}

class WindowStaticProc
{
public:

    template<typename EventType>
    static EventType & AsEvent(WindowEvent & e)
    {
        return static_cast<EventType&>(e);
    }

    static void HandleCreate(Window & w, WindowEvent & e, WindowClient & c)
    {
        c.OnCreating(w, AsEvent<WindowCreatingEvent>(e));
    }

    static void HandleDestroy(Window & w, WindowEvent & e, WindowClient & c)
    {
        c.OnDestroy(w);
    }

    static void HandleClose(Window & w, WindowEvent & e, WindowClient & c)
    {
        c.OnClose(w, AsEvent<WindowClosingEvent>(e));
    }

    static void HandleMouse(Window & w, WindowEvent & e, WindowClient & c)
    {
        Point offset;
        Point point;
        MouseButton button;
        MouseState state;

        point.set_x(LoWord(e.lparam));
        point.set_y(HiWord(e.lparam));

        e.wparam & MK_LBUTTON ? state |= MouseState::kPressedPrimary : 0;
        e.wparam & MK_RBUTTON ? state |= MouseState::kPressedSecondary : 0;
        e.wparam & MK_MBUTTON ? state |= MouseState::kPressedAuxiliary : 0;
        e.wparam & MK_XBUTTON1 ? state |= MouseState::kPressedXButton1 : 0;
        e.wparam & MK_XBUTTON2 ? state |= MouseState::kPressedXButton2 : 0;

        if (e.type != WM_MOUSELEAVE && w.tracking_mouse_ != true)
        {
            //开启鼠标追踪
            TRACKMOUSEEVENT track_event = {
                sizeof(track_event), TME_LEAVE, w.handle_, HOVER_DEFAULT
            };
            w.tracking_mouse_ = ::TrackMouseEvent(&track_event) != FALSE;
            c.OnMouseOver(w, state, point);
        }

        switch (e.type) {
        case WM_LBUTTONDBLCLK:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            button = MouseButton::kPrimary;
            break;
        case WM_RBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            button = MouseButton::kSecondry;
            break;
        case WM_MOUSEWHEEL:
            offset.set_y(static_cast<int16_t>(e.wparam >> 16));
            break;
        case WM_MOUSEHWHEEL:
            offset.set_x(static_cast<int16_t>(e.wparam >> 16));
            break;
        }

        switch (e.type) {
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
            c.OnMouseDoubleClick(w, button, state, point);
            break;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            c.OnMouseUp(w, button, state, point);
            break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            c.OnMouseDown(w, button, state, point);
            break;
        case WM_MOUSEMOVE:
            c.OnMouseMove(w, state, point);
            break;
        case WM_MOUSELEAVE:
            c.OnMouseOut(w);
            w.tracking_mouse_ = false;
            break;
        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL:
            c.OnMouseWheel(w, state, point, offset);
            break;
        }
        e.PreventDefault();        
        return;
    }

    static void HandleKeyboard(Window & w, WindowEvent & e, WindowClient & c)
    {
        KeyCode key = static_cast<KeyCode::Enum>(e.wparam);

        ShiftPressed() ? key |= KeyCode::kShift : 0;
        CtrlPressed() ? key |= KeyCode::kControl : 0;
        AltPressed() ? key |= KeyCode::kAlt : 0;

        KeyState state;
        state.repeated = static_cast<uint16_t>(e.lparam);
        state.extended = e.lparam & 0x00100000;
        state.alted = e.lparam & 0x20000000;
        state.previous = e.lparam & 0x40000000;
        state.transition = e.lparam & 0x80000000;
        switch (e.type) {
        case WM_KEYDOWN:
            c.OnKeyDown(w, key, state);
            break;
        case WM_KEYUP:
            c.OnKeyUp(w, key, state);
            break;
        }

        e.PreventDefault();
        return;
    }

    static void HandlePaint(Window & w, WindowEvent & e, WindowClient & c)
    {
        HWND hwnd = w.handle_;
        if (::GetUpdateRect(hwnd, 0, FALSE)) {
            Rect dirty;
            PAINTSTRUCT ps = { 0 };
            ::BeginPaint(hwnd, &ps);
            dirty = RECT2Rect(ps.rcPaint);
            c.OnPaint(w, ps.hdc, dirty);
            ::EndPaint(hwnd, &ps);
        }
        e.PreventDefault();
    }

    static void HandlePositionChanges(
        Window & w, WindowEvent & e, WindowClient & c
    ) {
        switch (e.type) {
        case WM_WINDOWPOSCHANGING:
            c.OnGeometryChanging(w, AsEvent<WindowGeometryChangingEvent>(e));
            break;
        case WM_WINDOWPOSCHANGED:
            c.OnGeometryChanged(w, AsEvent<WindowGeometryChangedEvent>(e));
        }
    }

    static void HandleTimer(Window & w, WindowEvent & e, WindowClient & c)
    {
        uint32_t timer = e.wparam;
        c.OnTime(w, timer);
        e.PreventDefault();
    }

    static void HandleMinMaxInfo(Window & w, WindowEvent & e, WindowClient & c)
    {
        auto mmi = reinterpret_cast<MINMAXINFO *>(e.lparam);

        Size max_size(mmi->ptMaxSize.x, mmi->ptMaxSize.y);
        Point max_pos(mmi->ptMaxPosition.x, mmi->ptMaxPosition.y);
        Size min_track_size(mmi->ptMinTrackSize.x, mmi->ptMinTrackSize.y);
        Size max_track_size(mmi->ptMaxTrackSize.x, mmi->ptMaxTrackSize.y);

        c.QueryBounds(w, max_size, max_pos, min_track_size, max_track_size);

        mmi->ptMaxSize.x = max_size.width();
        mmi->ptMaxSize.y = max_size.height();
        mmi->ptMaxPosition.x = max_pos.x();
        mmi->ptMaxPosition.y = max_pos.y();
        mmi->ptMinTrackSize.x = min_track_size.width();
        mmi->ptMinTrackSize.y = min_track_size.height();
        mmi->ptMaxTrackSize.x = max_track_size.width();
        mmi->ptMaxTrackSize.y = max_track_size.height();

        e.PreventDefault();
    }

    static void HandleNonClientSize(Window & w, WindowEvent & e, WindowClient & c)
    {
        Rect rect;
        int left = 0;
        int top = 0;
        int right = 0;
        int bottom = 0;
        RECT * nc = nullptr;

        if (e.wparam) {
            auto ncparams = reinterpret_cast<NCCALCSIZE_PARAMS *>(e.lparam);
            nc = &ncparams->rgrc[0];
        }
        else {
            nc = reinterpret_cast<RECT *>(e.lparam);
        }

        rect = RECT2Rect(*nc);

        e.result = DefWindowProc(w.handle_, e.type, e.wparam, e.lparam);

        left = nc->left - rect.left();
        top = nc->top - rect.top();
        right = rect.right() - nc->right;
        bottom = rect.bottom() - nc->bottom;

        c.QueryFrameSize(w, left, top, right, bottom);

        nc->left = rect.left() + left;
        nc->top = rect.top() + top;
        nc->right = rect.right() - right;
        nc->bottom = rect.bottom() - bottom;

        e.PreventDefault();
    }

    static void HandleActivate(Window & w, WindowEvent & e, WindowClient & c)
    {
        ActivateParam ap = ActivateParam::kActive;
        switch (e.wparam & 0xFFFF) {
        case WA_INACTIVE:
            c.OnDeactivated(w);
            break;
        case WA_ACTIVE:
            ap = ActivateParam::kActive;
            c.OnActivated(w, ap);
            break;
        case WA_CLICKACTIVE:
            ap = ActivateParam::kActiveByClick;
            c.OnActivated(w, ap);
            break;
        }
        //If the window is being activated and is not minimized,
        //the DefWindowProc function sets the keyboard focus to the window.
        //If the window is activated by a mouse click,
        //it also receives a WM_MOUSEACTIVATE message.
        //e.PreventDefault();
    }

    static void HandleFocus(Window & w, WindowEvent & e, WindowClient & c)
    {
        NativeWindow wrapper = NativeWindow::Wrap((HWND)e.wparam);

        if (e.type == WM_SETFOCUS) {
            
            c.OnSetFocus(w, wrapper);
        }
        else if (e.type == WM_KILLFOCUS) {
            c.OnKillFocus(w, wrapper);
        }
        e.PreventDefault();
    }

    static void HandleEnable(Window & w, WindowEvent & e, WindowClient & c)
    {
        if (e.wparam)
            c.OnEnabled(w);
        else
            c.OnDisabled(w);
        e.PreventDefault();
    }

    static void HandleCursor(Window & w, WindowEvent & e, WindowClient & c)
    {
        if (LoWord(e.lparam) == HTCLIENT) {
            HWND handle = (HWND)e.wparam;
            NativeWindow target = NativeWindow::Wrap(handle);
            c.QueryCursor(w, target);
            e.PreventDefault();
        }
    }

    static void HandleHitTest(Window & w, WindowEvent & e, WindowClient & c)
    {
        c.OnHitTest(w, AsEvent<WindowHitTestEvent>(e));
    }

    static void HandleEvent(Window & w, WindowEvent & e, WindowClient & c)
    {
        c.Preprocess(w, e);

        switch (e.type) {
        case WM_CREATE:
            HandleCreate(w, e, c);
            break;
        case WM_DESTROY:
            HandleDestroy(w, e, c);
            break;
        case WM_ACTIVATE:
            HandleActivate(w, e, c);
            break;
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
            HandleFocus(w, e, c);
            break;
        case WM_ENABLE:
            HandleEnable(w, e, c);
            break;
        case WM_CLOSE:
            HandleClose(w, e, c);
            break;
        case WM_SETCURSOR:
            HandleCursor(w, e, c);
            break;
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        case WM_XBUTTONDBLCLK:
        case WM_MOUSEWHEEL:
        case WM_MOUSELEAVE:
            HandleMouse(w, e, c);
            break;
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_UNICHAR:
            HandleKeyboard(w, e, c);
            break;
        case WM_PAINT:
            HandlePaint(w, e, c);
            break;
        case WM_WINDOWPOSCHANGING:
        case WM_WINDOWPOSCHANGED:
            HandlePositionChanges(w, e, c);
            break;
        case WM_TIMER:
            HandleTimer(w, e, c);
            break;
        case WM_GETMINMAXINFO:
            HandleMinMaxInfo(w, e, c);
            break;
        case WM_NCCALCSIZE:
            HandleNonClientSize(w, e, c);
            break;
        case WM_NCHITTEST:
            HandleHitTest(w, e, c);
            break;
        }

        c.Postprocess(w, e);
    }

    static LRESULT CALLBACK Routine(
        HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam
    ) {
        if (msg == WM_NCCREATE){
            auto instance = GetWindowInstanceFromParam(lparam);
            instance.window->handle_ = hwnd;
            instances[hwnd] = instance;
        }

        WindowEvent e(msg, wparam, lparam);

        auto item = instances.find(hwnd);
        if (item != instances.end())
        { 
            Window & w = *item->second.window;
            WindowClient & c = *item->second.client;
            if (msg == WM_DESTROY)
                instances.erase(hwnd);
            HandleEvent(w, e, c);
        }

        if (e.DefaultPrevented())
            e.result = DefWindowProc(hwnd, msg, wparam, lparam);
        return e.result;
    }
};

WindowArgs::WindowArgs()
{
    origin = Point::Make(100, 100);
    size = Size::Make(300, 200);
    window_name = L"DEFAULT";    
    window_style = 0;
    window_style_ex = 0;
    parent_window = nullptr;
    client = nullptr;
}

Window::Window() 
    : tracking_mouse_(false)
{
}

Window::~Window()
{
}

bool Window::Initialize()
{
    auto module = Utils::CurrentModule();

    WNDCLASSEX wndcls = { 0 };
    wndcls.cbSize = sizeof(wndcls);
    
    if (GetClassInfoEx(module, kDefaultClassName, &wndcls))
        return true;
    
    
    wndcls.cbSize = sizeof(wndcls);
    wndcls.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wndcls.lpfnWndProc = WindowStaticProc::Routine;
    wndcls.cbClsExtra = 0;
    wndcls.cbWndExtra = 0;
    wndcls.hInstance = module;
    wndcls.hIcon = 0;
    wndcls.hCursor = 0;
    wndcls.hbrBackground = 0;
    wndcls.lpszMenuName = 0;
    wndcls.lpszClassName = kDefaultClassName;
    wndcls.hIconSm = 0;

    return ::RegisterClassEx(&wndcls) != FALSE;
}

void Window::UnInitialize()
{
    auto module = Utils::CurrentModule();
    ::UnregisterClass(kDefaultClassName, module);
    return;
}

bool Window::Create(const WindowArgs & args)
{
    auto module = Utils::CurrentModule();
    NativeWindow * parent_window = args.parent_window;
    uint32_t wsex = args.window_style_ex;
    uint32_t ws = args.window_style;
    int x = args.origin.x();
    int y = args.origin.y();
    int cx = args.size.width();
    int cy = args.size.height();

    HWND parent_window_handle = 0;
    if(parent_window)
        parent_window_handle = parent_window->NativeHandle();

    WindowInstance instance;
    instance.window = this;
    instance.client = args.client;
        
    handle_ = CreateWindowEx(
        wsex, kDefaultClassName, args.window_name,
        ws, x, y, cx, cy, parent_window_handle, 0,
        module, &instance
    );

    return handle_ != NULL;
}

}

}
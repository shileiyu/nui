#include "workspace.h"

namespace nui
{

Rect WorkSpace::GetPrimaryWorkArea()
{
    HWND ref_wnd = 0;
    HMONITOR monitor = 0;
    Rect work_area_rect(0, 0, 800, 600);
    //使用开始菜单做参照
    ref_wnd = ::FindWindow(L"Shell_TrayWnd", 0);
    //如果没有找到开始菜单，使用桌面窗口做参照
    if (ref_wnd == 0)
        ref_wnd = ::GetDesktopWindow();
    //还是没有找到，直接返回默认值
    if (ref_wnd == 0)
        return work_area_rect;

    monitor = ::MonitorFromWindow(ref_wnd, MONITOR_DEFAULTTOPRIMARY);

    //如果找到监视器, 则查询监视器工作区.
    //如果没有错误发生则使用查询所得工作区.
    //否则使用默认值.
    if (monitor)
    {
        MONITORINFO mi = { 0 };
        mi.cbSize = sizeof(mi);
        if (GetMonitorInfo(monitor, &mi))
        {
            RECT & wr = mi.rcWork;
            work_area_rect.SetLTRB(wr.left, wr.top, wr.right, wr.bottom);
        }
    }
    return work_area_rect;
}

}
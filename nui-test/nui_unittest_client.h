#ifndef NUI_UINTTEST_CLIENT_H_
#define NUI_UINTTEST_CLIENT_H_

#include <nui/sys/window.h>
#include <nui/base/pixpainter.h>
#include <nui/gadget/world.h>
#include <nui/gadget/picture.h>
#include <nui/gadget/label.h>
#include <nui/base/point.h>
#include <nui/layout/multi.h>
#include <nui/layout/mover.h>
#include <nui/layout/sizer.h>
#include "list_view.h"

//#include <nui/gadget/scroller.h>
//#include <nui/gadget/text_area.h>

namespace nui
{

class UITestClient :
    public WindowClient, public GadgetWorldClient
{
public:
    UITestClient();

    void init();

    void fini();
public:
    void OnCreating(Window & window, WindowCreatingEvent & e) override;

    void OnDestroy(Window & window) override;

    void OnClose(Window & window, WindowClosingEvent & e) override;

    void OnPaint(Window & window, PaintContext ctx, Rect & dirty) override;

    void OnGeometryChanged(
        Window & window,
        WindowGeometryChangedEvent & e
        ) override;

    void OnMouseDown(
        Window & window,
        MouseButton button,
        MouseState state,
        Point & point
        ) override;

    void OnMouseUp(
        Window & window,
        MouseButton button,
        MouseState state,
        Point & point
        ) override;

    void OnMouseMove(
        Window & window,
        MouseState state,
        Point & point
        ) override;

    void OnMouseWheel(
        Window & window,
        MouseState state,
        Point & point,
        Point & offset) override;
public:
    void PenddingRedraw(ScopedWorld world, const Rect & rect) override;

    void SetCursor(ScopedWorld world, CursorStyles cursor) override;
private:
    Window wnd_;
    GadgetWorld world_;
    Picture picture1_;
    Picture picture2_;
    Label label1_;
    //TScroller scroller_;
    //TextArea text_area_;
    Pixmap pixmap_;

    MultiLayout multi_;
    PixelSizer pic2_sizer_;
    PixelMover t1_mover_;

    ListView list_view_;
    PixelSizer list_view_sizer_;
};


}
#endif
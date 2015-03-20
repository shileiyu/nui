#include "nui_unittest_client.h"
#include "usp10.h"
#include <nui/sys/utils.h>

namespace nui
{

UITestClient::UITestClient()
:world_(this)
{
}

void UITestClient::init()
{
    WindowArgs args;
    args.window_style = WS_THICKFRAME;
    args.client = this;
    args.size.SetSize(400, 400);
    wnd_.Create(args);
    wnd_.Show();
}

void UITestClient::fini()
{
    ;
}

void UITestClient::OnDestroy(Window & window)
{
    ::PostQuitMessage(0);
}

void UITestClient::OnClose(Window & window, WindowClosingEvent & e)
{
    wnd_.Destroy();
}

void UITestClient::OnGeometryChanged(Window & window,
    WindowGeometryChangedEvent & e)
{
    Rect cr;
    window.GetClientRect(cr);

    auto width = cr.width();
    auto height = cr.height();
    Size size(width, height);

    pixmap_ = Pixmap::Alloc(width, height);
    world_.SetSize(size);
    picture1_.SetSize(size);
    world_.Layout();
}


void UITestClient::OnMouseDown(
    Window & window,
    MouseButton button,
    MouseState state,
    Point & point
    )
{
    world_.SetMousePos(point);
    world_.MouseButtonDown(MouseButton::kPrimary);
    
}

void UITestClient::OnMouseUp(
    Window & window,
    MouseButton button,
    MouseState state,
    Point & point
    )
{
    world_.SetMousePos(point);
    world_.MouseButtonUp(MouseButton::kPrimary);
}

void UITestClient::OnMouseMove(
    Window & window,
    MouseState state,
    Point & point
    )
{
    world_.SetMousePos(point);
}

void UITestClient::OnMouseWheel(
    Window & window,
    MouseState state,
    Point & point,
    Point & offset)
{
    world_.Wheel(offset.x(), offset.y());
}

void UITestClient::OnPaint(Window & window, PaintContext ctx, Rect & dirty)
{
    PixPainter painter(pixmap_);
    world_.Draw(painter, dirty);

    Win32Utils::Present(ctx, pixmap_);
}

void UITestClient::OnCreating(Window & window, WindowCreatingEvent & e)
{
    //label1_.Create(&world_, L"label1\n");
    //scroller_.Create(&world_);
    //text_area_.Create(&world_);

    //auto & la1 = label1_.gadget();
    //auto & s1 = scroller_.gadget();
    //auto & t1 = text_area_.gadget();

    picture1_.AddChild(&picture2_);
    picture1_.AddChild(&label1_);
    //pc2.AttachChild(s1, -1);
    //pc2.AttachChild(t1, -1);

    picture1_.SetBackground(Color(0xff, 0xff, 0xff, 0xff));

    //s1.SetLoc(Point(20, 30));
    //s1.SetSize(Size(200, 150));
    //
    //t1.SetLoc(Point(20, 200));
    //t1.SetSize(Size(100, 50));
    //text_area_.SetText(L"我mpdj安装猩猩");
    Picture::LinearGradient linear;
    Color linear_colors[3] = 
    { 
        Color(0xff, 0, 0, 0), 
        Color(0xff, 0xff, 0, 0), 
        Color(0xff, 0, 0, 0) 
    };
    float pos[3] = { 0.f, 0.5f, 1.f };
    
    linear.begin[0] = 0.f;
    linear.begin[1] = 0.f;
    linear.end[0] = 0.f;
    linear.end[1] = 1.f;
    linear.color = linear_colors;
    linear.pos = pos;
    linear.count = 3;
    
    picture2_.SetBackground(linear);
    ////设置文本
    label1_.SetText(L"潘多拉游戏管家");
    label1_.SetTextSize(16);
    label1_.SetTextColor(Color(0xff, 0, 0xff, 0xff));
    label1_.SetLoc(Point(0, 20));
    label1_.SetSize(Size(200, 30));

    pic2_sizer_.left(10).top(20).right(30).bottom(40);
    pic2_sizer_.Attach(&picture2_);
    t1_mover_.Set(MoveOrigin::kTopRight, 0, 0);
    t1_mover_.Attach(&label1_);

    multi_.Attach(&pic2_sizer_);
    multi_.Attach(&t1_mover_);
    picture1_.SetLayout(&multi_);
    world_.AddChild(&picture1_);

    list_view_.GetScroller()->SetScrollBarAutoHide(false);

    list_view_sizer_.left(0).top(0).right(0).bottom(0);
    list_view_sizer_.Attach(list_view_.GetScroller());
    multi_.Attach(&list_view_sizer_);
    picture1_.AddChild(list_view_.GetScroller());
    list_view_.AddItem(0);
    list_view_.AddItem(1);
    list_view_.AddItem(2);
    list_view_.AddItem(3);
    list_view_.SetPreferredSize(Size::Make(400, list_view_.GetPreferredSize().height()));
}

void UITestClient::PenddingRedraw(ScopedWorld world, const Rect & rect)
{
    RECT invalid = { rect.left(), rect.top(), rect.right(), rect.bottom() };
    
    ::InvalidateRect(wnd_.NativeHandle(), NULL, FALSE);
}

void UITestClient::SetCursor(ScopedWorld world, CursorStyles cursor)
{
    ;
}

}
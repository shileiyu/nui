
#include "scroller.h"
#include <nui/base/painter.h>
#include <nui/gadget/events.h>
#include <nui/gadget/world.h>

namespace nui
{

static const int kVScrollBarWidth = 12;
static const int kHScollBarHeight = 12;


Scroller::Scroller() 
    :left_click_state_(kNone), max_scroll_y_(0), cur_scroll_y_(0),
    scale_vertical_(1.f), scale_horizontal_(1.f), 
    max_scroll_x_(0), cur_scroll_x_(0), last_mouse_(-1, -1), 
    show_scrollbar_(true)
{
    
}

Scroller::~Scroller()
{

}
bool Scroller::Create(GadgetWorld * world)
{
    if (!world)
        return false;
    if (!gadget_)
        gadget_ = world->CreateGadgetWrap(this);

    return gadget_ == true;
}

void Scroller::Destroy()
{
    if (gadget_)
    {
        gadget_.world()->DestroyGadgetWrap(gadget_);
        gadget_ = GadgetWrap();
    }
}

GadgetWrap & Scroller::gadget()
{
    return gadget_;
}

void Scroller::Draw(GadgetWrap & wrap, Painter & painter)
{
    painter.Save();
    OnClientDraw(wrap, painter);
    painter.Restore();
    //更新滚动条区域
    UpdateScrollBar();
    DrawScroller(painter);
}

bool Scroller::HitTest(GadgetWrap & wrap, const Point & pt)
{
    return OnClientHitTest(wrap, pt);
}

void Scroller::OnSizeChanged(GadgetWrap & wrap)
{
    cache_dirty_ = true;
    OnClientSizeChanged(wrap);
}

void Scroller::SetDesiredSize(const Size & size)
{
    desired_size_ = size;
}

int Scroller::GetCurrentScrollX() const
{
    return cur_scroll_x_;
}

int Scroller::MapScrollX(int x) const
{
    return  x - cur_scroll_x_;
}

int Scroller::MapScrollY(int y) const
{
    return y - cur_scroll_y_;
}

void Scroller::ShowScrollBar(bool show)
{
    show_scrollbar_ = show;
}

int Scroller::GetCurrentScrollY() const
{
    return cur_scroll_y_;
}
void Scroller::SetCurrentScrollX(int current)
{
    ScrollerBarMoveHorizontal(current - cur_scroll_x_);
}

void Scroller::SetCurrentScrollY(int current)
{
    ScrollerBarMoveVertical(current - cur_scroll_y_);
}

void Scroller::ScrollX(int dx)
{
    ScrollerBarMoveHorizontal(dx);
}

void Scroller::ScrollY(int dy)
{
    ScrollerBarMoveVertical(dy);
}

void Scroller::UpdateScrollBar()
{
    if (cache_dirty_)
    {
        UpdateScrollValue();
        UpdateVScrollBarArea();
        UpdateHScrollBarArea();
        cache_dirty_ = false;
    }
}

void Scroller::UpdateScrollValue()
{
    Size size;
    if (!gadget_.GetSize(size))
        return;

    //更新最大滚动值
    max_scroll_x_ = size.width() - desired_size_.width();

    if (max_scroll_x_ > 0)
        max_scroll_x_ = 0;

    if (cur_scroll_x_ < max_scroll_x_)
    {
        cur_scroll_x_ = max_scroll_x_;
    }
    


    max_scroll_y_ = size.height() - desired_size_.height();

    if (max_scroll_y_ > 0)
        max_scroll_y_ = 0;

    if (cur_scroll_y_ < max_scroll_y_)
    {
        cur_scroll_y_ = max_scroll_y_;
    }
}

void Scroller::UpdateHScrollBarArea()
{
    Size size;
    if (!gadget_.GetSize(size))
        return;

    auto view_width = size.width();
    auto view_height = size.height();

    //如果同时出现垂直滚动条
    auto max_slider_width = view_width;
    //if (max_scroll_y_ < 0)
    //    max_slider_width -= kVScrollBarWidth;

    max_horizontal_rect_.SetXYWH(0,
                                 view_height - kHScollBarHeight,
                                 max_slider_width,
                                 kHScollBarHeight);

    UpdateHSliderArea();
}

void Scroller::UpdateHSliderArea()
{
    auto max_width = max_horizontal_rect_.width();
    //更新水平滑块大小
    scale_horizontal_ = static_cast<float>(max_width - max_scroll_x_) / max_width;
    //得到滑块高度
    int slider_width = static_cast<int>(max_width / scale_horizontal_);
    //得到滑块TOP
    float fscale = 1.f;
    if (max_scroll_x_ < 0)
        fscale = static_cast<float>(0 - cur_scroll_x_) / (0 - max_scroll_x_);

    float fslider_left = fscale * (max_width - slider_width);
    slider_horizontal_rect_.SetXYWH(
        static_cast<int>(fslider_left),
        max_horizontal_rect_.top(),
        slider_width,
        kHScollBarHeight);
}

void Scroller::UpdateVScrollBarArea()
{
    Size size;
    if (!gadget_.GetSize(size))
        return;
    auto view_height = size.height();
    auto max_slider_height = view_height;

    //同时出现横向滚动条
    //if (max_scroll_x_ < 0)
    //    max_slider_height -= kHScollBarHeight;

    max_vertical_rect_.SetXYWH(size.width() - kVScrollBarWidth,
                           0, 
                           kVScrollBarWidth, 
                           max_slider_height);
    UpdateVSliderArea();
}

void Scroller::UpdateVSliderArea()
{
    auto max_height = max_vertical_rect_.height();
    //更新滑块大小
    scale_vertical_ = 
        static_cast<float>(max_height - max_scroll_y_) / max_height;

    //得到滑块高度
    int slider_height = static_cast<int>(max_height / scale_vertical_);
    //得到滑块TOP
    float fscale = 1.f;
    if (max_scroll_y_ < 0)
    {
        fscale = static_cast<float>(0 - cur_scroll_y_) / (0 - max_scroll_y_);
    }
    
    float fslider_top = fscale * (max_height - slider_height);
    slider_vertical_rect_.SetXYWH(max_vertical_rect_.left(),
        static_cast<int>(fslider_top),
        kVScrollBarWidth,
        slider_height);
}

void Scroller::Dispatch(GadgetWrap & wrap, GadgetEvent & e)
{
    bool handle = false;

    if (e.target() == gadget_)
    {
        GadgetMouseEvent * me = GadgetMouseEvent::FromEvent(e);
        if (me != 0)
            handle = OnScrollerMouseEvent(*me);

        GadgetWheelEvent * we = GadgetWheelEvent::FromEvent(e);
        if (we)
            handle = OnScrollerWheel(*we);
    }
    if (!handle)
        OnClientDispatch(wrap, e);
}

bool Scroller::OnScrollerMouseEvent(GadgetMouseEvent & e)
{
    switch (e.type())
    {
    case GadgetEventType::kMouseDown:
        return OnScrollerMouseDown(e);
    case GadgetEventType::kMouseUp:
        return OnScrollerMouseUp(e);
    case GadgetEventType::kMouseMove:
        return OnScrollerMouseMove(e);
    case GadgetEventType::kMouseOut:
        return OnScrollerMouseOut(e);
    default:
        return false;
        break;
    }
}

bool Scroller::OnScrollerMouseOut(GadgetMouseEvent & e)
{
    last_mouse_.Set(-1, -1);
    left_click_state_ = kNone;
    return false;
}

bool Scroller::OnScrollerWheel(GadgetWheelEvent & e)
{
    ScrollerBarMoveVertical(-e.dy());
    ScrollerBarMoveHorizontal(-e.dx());
    gadget_.Invalidate();
    return false;
}

bool Scroller::OnScrollerMouseDown(GadgetMouseEvent & e)
{
    if (e.button() == MouseButton::kPrimary)
    {//左键按下
        left_click_state_ = GetStateByPos(Point(e.x(), e.y()));
        //滚动条上按下
        if (kVSlider == left_click_state_ || kHSlider == left_click_state_)
            return true;
    }
    return false;
}

Scroller::State Scroller::GetStateByPos(const Point & pt)
{
    if (show_scrollbar_)
    {
        if (max_scroll_y_ < 0 && !slider_vertical_rect_.isEmpty())
        {//垂直方向可滚动
            if (slider_vertical_rect_.isPointIn(pt))
                return kVSlider;
        }

        if (max_scroll_x_ < 0 && !slider_horizontal_rect_.isEmpty())
        {//水平方向可滚动
            if (slider_horizontal_rect_.isPointIn(pt))
                return kHSlider;
        }
    }

    Size size;
    gadget_.GetSize(size);
    Rect bounds = Rect::Make(size);
    if (bounds.isPointIn(pt))
        return kClient;

    return kNone;
}

bool Scroller::OnScrollerMouseUp(GadgetMouseEvent & e)
{
    bool handle = false;
    if (kVSlider == left_click_state_ || kHSlider == left_click_state_)
        handle = true;


    left_click_state_ = kNone;
    return handle;
}

bool Scroller::OnScrollerMouseMove(GadgetMouseEvent & e)
{
    bool bret = false;

    if (left_click_state_ == kVSlider)
    {//鼠标按在垂直滚动条上
        int dy = 0;
        dy = e.y() - last_mouse_.y();
        ScrollerBarMoveVertical(dy);
        gadget_.Invalidate();
        bret = true;
    }
    else if (left_click_state_ == kHSlider)
    {//鼠标点在水平滚动条上
        int dx = 0;
        dx = e.x() - last_mouse_.x();
        ScrollerBarMoveHorizontal(dx);
        gadget_.Invalidate();
        bret = true;
    }
    else if (left_click_state_ == kNone)
    {//鼠标没有按下
        hover_state_ = GetStateByPos(Point(e.x(), e.y()));
        if (kHSlider == hover_state_ || kVSlider == hover_state_)
            bret = true;
    }

    last_mouse_.Set(e.x(), e.y());
    return bret;
}

void Scroller::ScrollerBarMoveHorizontal(int dx)
{
    assert(max_scroll_x_ <= 0);
    if (0 == dx || 0 <= max_scroll_x_)
        return;

    int old_left = slider_horizontal_rect_.left();
    int width = slider_horizontal_rect_.width();

    int min_left = max_horizontal_rect_.left();
    int max_left = max_horizontal_rect_.right() - width;
    int left = old_left + dx;

    if (dx > 0)
    {//右移
        if (left > max_left)
            left = max_left;
    }
    else if (dx < 0)
    {//左移

        if (left < min_left)
            left = min_left;
    }

    //修正composition_移动的步长
    if (left == min_left)
        cur_scroll_x_ = 0;
    else if (left == max_left)
        cur_scroll_x_ = max_scroll_x_;
    else
    {
        int step = left - old_left;

        float fstep = step * scale_horizontal_;
        cur_scroll_x_ = static_cast<int>(cur_scroll_x_ - fstep);
        if (cur_scroll_x_ < max_scroll_x_)
            cur_scroll_x_ = max_scroll_x_;
        if (cur_scroll_x_ > 0)
            cur_scroll_x_ = 0;
    }

    UpdateHSliderArea();
}

void Scroller::ScrollerBarMoveVertical(int dy)
{
    assert(max_scroll_y_ <= 0);
    if (0 == dy || 0 <= max_scroll_y_ )
        return;
    
    //修正滑块的位置
    int old_top = slider_vertical_rect_.top();
    int height = slider_vertical_rect_.height();
    
    int min_top = max_vertical_rect_.top();
    int max_top = max_vertical_rect_.bottom() - height;
    int top = old_top + dy;

    if (dy > 0)
    {//下移
        if (top > max_top)
            top = max_top;
    }
    else if (dy < 0)
    {//上移
        
        if (top < min_top)
            top = min_top;
    }

    //修正composition_移动的步长
    if (top == min_top)
        cur_scroll_y_ = 0;
    else if (top == max_top)
        cur_scroll_y_ = max_scroll_y_;
    else
    {
        int step = top - old_top;

        float fstep = step * scale_vertical_;
        cur_scroll_y_ = static_cast<int>(cur_scroll_y_ - fstep);
        if (cur_scroll_y_ < max_scroll_y_)
            cur_scroll_y_ = max_scroll_y_;
        if (cur_scroll_y_ > 0)
            cur_scroll_y_ = 0;
    }

    UpdateVSliderArea();
}

void Scroller::DrawScroller(Painter & painter)
{
    DrawHScroller(painter);
    DrawVScroller(painter);
}

void Scroller::DrawHScroller(Painter & painter)
{
    if (max_scroll_x_ >= 0 || !show_scrollbar_)
        return;

    assert(!slider_horizontal_rect_.isEmpty());
    if (!slider_horizontal_rect_.isEmpty())
    {
        Paint paint;
        paint.color.Set(0x88, 0xff, 0, 0);
        painter.DrawRect(slider_horizontal_rect_, paint);
    }
}

void Scroller::DrawVScroller(Painter & painter)
{
    //没有滚动条或者 滚动条不绘制
    if (max_scroll_y_ >= 0 || !show_scrollbar_)
        return;

    assert(!slider_vertical_rect_.isEmpty());

    if (!slider_vertical_rect_.isEmpty())
    {
        Paint paint;
        paint.color.Set(0x88, 0xff, 0, 0);
        painter.DrawRect(slider_vertical_rect_, paint);
    }
}

void Scroller::OnClientDispatch(GadgetWrap & wrap, GadgetEvent & e)
{
    ;
}

void Scroller::OnClientDraw(GadgetWrap & wrap, Painter & painter)
{
    ;
}

void Scroller::OnClientSizeChanged(GadgetWrap & wrap)
{
    ;
}

bool Scroller::OnClientHitTest(GadgetWrap & wrap, const Point & pt)
{
    return true;
}

}
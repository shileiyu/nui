#include "scroll_bar.h"
#include <nui/gadget/events.h>
#include <nui/base/painter.h>
#include <algorithm>

namespace nui
{

static const int kHScrollBarHeight = 12;

static const int kVScrollBarWidth = 12;

ScrollBar::ScrollBar(Style style)
:style_(style), controller_(nullptr), max_pos_(0), enable_(true),
contents_scroll_offset_(0), content_size_(0), viewport_size_(0), scale_(1.f),
left_click_state_(kNone), hover_state_(kNone), last_mouse_(-1, -1)
{
    ;
}

ScrollBar::~ScrollBar()
{
    ;
}

void ScrollBar::SetEnable(bool enable)
{
    if (enable_ != enable)
    {
        enable_ = enable;
        left_click_state_ = kNone;
        Invalidate();
    }
}

bool ScrollBar::IsEnable() const
{
    return enable_;
}

ScrollBar::Style ScrollBar::style() const
{
    return style_;
}

void ScrollBar::set_controller(ScrollBarController * controller)
{
    controller_ = controller;
}

int ScrollBar::GetContentOverlapSize() const
{
    if (style_ == kVertal)
        return kVScrollBarWidth;
    else if (style_ == kHorizontal)
        return kHScrollBarHeight;
    else
        return 0;
}

void ScrollBar::Update(int viewport_size, int content_size, int contents_scroll_offset)
{
    max_pos_ = std::max(0, content_size - viewport_size);
    //content_size_ = std::max(1, content_size);
    //viewport_size_ = std::max(1, viewport_size);
    //if (content_size < 0)
    //    content_size = 0;
    //if (contents_scroll_offset < 0)
    //    contents_scroll_offset = 0;
    //if (contents_scroll_offset > content_size)
    //    contents_scroll_offset = content_size;
    //contents_scroll_offset_ = contents_scroll_offset;

    if (contents_scroll_offset < 0)
        contents_scroll_offset = 0;
    if (contents_scroll_offset > max_pos_)
        contents_scroll_offset = max_pos_;
    contents_scroll_offset_ = contents_scroll_offset;
    //更新滑块大小
    if (kVertal == style_)
        UpdateVScrollBarArea();
    else if (kHorizontal == style_)
        UpdateHScrollBarArea();
}

void ScrollBar::UpdateHScrollBarArea()
{
    auto max_width = GetWidth();
    //更新水平滑块大小
    scale_ = static_cast<float>(max_width + max_pos_) / max_width;
    //得到滑块高度
    int slider_width = static_cast<int>(max_width / scale_);
    //得到滑块top
    float fscale = 1.f;
    if (max_pos_ > 0)
        fscale = static_cast<float>(contents_scroll_offset_) / max_pos_;
    float fslider_left = fscale * (max_width - slider_width);
    slider_rect_.SetXYWH(static_cast<int>(fslider_left), 0, slider_width, GetHeight());
}

void ScrollBar::UpdateVScrollBarArea()
{
    auto max_height = GetHeight();
    //更新滑块大小
    scale_ = static_cast<float>(max_height + max_pos_) / max_height;
    //得到滑块高度
    int slider_height = static_cast<int>(max_height / scale_);
    float fscale = 1.f;
    if (max_pos_ > 0)
        fscale = static_cast<float>(contents_scroll_offset_) / max_pos_;

    float fslider_top = fscale * (max_height - slider_height);
    slider_rect_.SetXYWH(0, static_cast<int>(fslider_top), GetWidth(), slider_height);
}

int ScrollBar::GetMaxPosition() const
{
    return max_pos_;
}

int ScrollBar::GetMinPosition() const
{
    return 0;
}

int ScrollBar::GetPosition() const
{
    return contents_scroll_offset_;
}

void ScrollBar::Dispatch(Event & e)
{
    Gadget::Dispatch(e);
    if (!e.Propagation())
        return;
    MouseEvent * me = MouseEvent::FromEvent(e);
    if (me)
    {//鼠标事件
        switch (me->type())
        {
        case EventType::kMouseMove:
            OnMouseMove(this, *me);
            break;
        case EventType::kMouseDown:
            OnMouseDown(this, *me);
            break;
        case EventType::kMouseUp:
            OnMouseUp(this, *me);
            break;
        case EventType::kMouseOut:
            OnMouseOut(this, *me);
            break;
        default:
            break;
        }

        return;
    }
    WheelEvent * we = WheelEvent::FromEvent(e);
    if (we)
    {
        OnWheel(this, *we);
        return;
    }
}

void ScrollBar::OnWheel(nui::ScopedGadget gadget, nui::WheelEvent & e)
{
    if (kVertal == style_)
        ScrollBarMoveVertical(-e.dy());
    else if (kHorizontal == style_)
        ScrollBarMoveHorizontal(-e.dx());
    else
        assert(0);
    Invalidate();
}

void ScrollBar::OnMouseOut(nui::ScopedGadget gadget, nui::MouseEvent & e)
{
    last_mouse_.Set(-1, -1);
    if (left_click_state_ != kNone || hover_state_ != kNone)
    {
        left_click_state_ = kNone;
        hover_state_ = kNone;
        Invalidate();
    }
}

void ScrollBar::OnMouseMove(nui::ScopedGadget gadget, nui::MouseEvent & e)
{
    if (e.type() != EventType::kMouseMove)
        return;
    MouseEvent * me = MouseEvent::FromEvent(e);
    State last_hover_state = hover_state_;
    hover_state_ = GetStateByPos(Point(me->x(), me->y()));
    if (left_click_state_ == kSlider)
    {
        if (kVertal == style_)
        {//垂直滚动条
            int dy = 0;
            dy = me->y() - last_mouse_.y();
            ScrollBarMoveVertical(dy);
            ScrollContentsToOffset();
            Invalidate();
        }
        else if (kHorizontal == style_)
        {//水平滚动条
            int dx = 0;
            dx = me->x() - last_mouse_.x();
            ScrollBarMoveHorizontal(dx);
            ScrollContentsToOffset();
            Invalidate();
        }
    }
    else if (left_click_state_ == kNone)
    {//左键没有按下
        if (last_hover_state != hover_state_)//鼠标在滑块上
            Invalidate();
    }
    last_mouse_.Set(me->x(), me->y());
}

void ScrollBar::ScrollBarMoveHorizontal(int dx)
{
    assert(max_pos_ >= 0);
    if (0 == dx || max_pos_ < 0)
        return;
    int old_left = slider_rect_.left();
    int width = slider_rect_.width();
    int min_left = 0;
    int max_left = GetWidth() - width;
    int left = old_left + dx;

    if (left > max_left)
        left = max_left;
    if (left < min_left)
        left = min_left;
    //计算content_offset_
    if (left == min_left)
        contents_scroll_offset_ = 0;
    else if (left == max_left)
        contents_scroll_offset_ = max_pos_;
    else
    {
        int step = left - old_left;
        float fstep = step * scale_;
        contents_scroll_offset_ = static_cast<int>(contents_scroll_offset_ + fstep);
        if (contents_scroll_offset_ < 0)
            contents_scroll_offset_ = 0;
        if (contents_scroll_offset_ > max_pos_)
            contents_scroll_offset_ = max_pos_;
    }
    UpdateHScrollBarArea();
}

void ScrollBar::ScrollBarMoveVertical(int dy)
{
    assert(max_pos_ >= 0);

    if (0 == dy || max_pos_ < 0)
        return;

    //修正滑块的位置
    int old_top = slider_rect_.top();
    int height = slider_rect_.height();

    int min_top = 0;
    int max_top = GetHeight() - height;
    int top = old_top + dy;

    if (top > max_top)
        top = max_top;

    if (top < min_top)
        top = min_top;

    //修正composition_移动的步长
    if (top == min_top)
        contents_scroll_offset_ = 0;
    else if (top == max_top)
        contents_scroll_offset_ = max_pos_;
    else
    {
        int step = top - old_top;

        float fstep = step * scale_;
        contents_scroll_offset_ = static_cast<int>(contents_scroll_offset_ + fstep);
        if (contents_scroll_offset_ < 0)
            contents_scroll_offset_ = 0;
        if (contents_scroll_offset_ > max_pos_)
            contents_scroll_offset_ = max_pos_;
    }

    UpdateVScrollBarArea();
}

void ScrollBar::OnMouseUp(nui::ScopedGadget gadget, nui::MouseEvent & e)
{
    if (e.type() != EventType::kMouseUp)
        return;
    left_click_state_ = kNone;
    Invalidate();
}

void ScrollBar::OnMouseDown(nui::ScopedGadget gadget, nui::MouseEvent & e)
{
    if (e.type() != EventType::kMouseDown)
        return;
    MouseEvent * me = MouseEvent::FromEvent(e);
    if (MouseButton::kPrimary != me->button())
        return;
    left_click_state_ = GetStateByPos(Point(me->x(), me->y()));
    Invalidate();
}

void ScrollBar::OnDraw(Painter & painter)
{
    Rect rect;
    GetLocalBounds(rect);
    
    Paint paint;
    paint.color = Color(0x88, 0xff, 0, 0);
    painter.DrawRect(rect, paint);

    paint.color = Color(0x33, 0, 0xff, 0);
    painter.DrawRect(slider_rect_, paint);
}

ScrollBar::State ScrollBar::GetStateByPos(const Point & pt)
{
    Rect bounds;
    GetLocalBounds(bounds);
    if (slider_rect_.isPointIn(pt))
        return kSlider;
    else if (bounds.isPointIn(pt))
    {
        return kTrack;
    }
    else 
        return kNone;
}

bool ScrollBar::ScrollByContentsOffset(int contents_offset)
{
    int old_offset = contents_scroll_offset_;
    contents_scroll_offset_ -= contents_offset;
    if (contents_scroll_offset_ < GetMinPosition())
        contents_scroll_offset_ = GetMinPosition();
    else if (contents_scroll_offset_ > GetMaxPosition())
        contents_scroll_offset_ = GetMaxPosition();

    if (old_offset == contents_scroll_offset_)
        return false;
    ScrollContentsToOffset();
    return true;
}

void ScrollBar::ScrollContentsToOffset()
{
    if (controller_)
        controller_->ScrollToPosition(this, contents_scroll_offset_);
}



}
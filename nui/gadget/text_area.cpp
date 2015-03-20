#include "text_area.h"

#include <nui/gadget/events.h>

#include <algorithm>

namespace nui
{

TextArea::TextArea()
    :bg_color_(0xff, 0xff, 0xff, 0xff), selected_bg_color_(0xff, 0, 0, 0xff),
    selected_text_color_(0xff, 0xff, 0xff, 0xff), normal_text_color_(0xff, 0, 0, 0),
    cache_dirty_(false), start_sel_(9), end_sel_(13), edit_(false), drag_(false),
    famliy_name_("Microsoft YaHei"), font_style_(Font::kNormal), text_size_(14)
{

}

bool TextArea::Create(GadgetWorld * world)
{
    return Scroller::Create(world);
}

void TextArea::Destroy()
{
    Scroller::Destroy();
}

void TextArea::SetText(const wchar_t * text)
{
    text_ = text;
    start_sel_ = end_sel_ = 0;
    cache_dirty_ = true;
}

void TextArea::OnClientDispatch(GadgetWrap & wrap, GadgetEvent & e)
{
    switch (e.type())
    {
    case GadgetEventType::kMouseDown:
        OnMouseDown(*GadgetMouseEvent::FromEvent(e));
        break;
    case GadgetEventType::kMouseUp:
        OnMouseUp(*GadgetMouseEvent::FromEvent(e));
        break;
    case GadgetEventType::kMouseMove:
        OnMouseMove(*GadgetMouseEvent::FromEvent(e));
        break;
    default:
        break;
    }
}

void TextArea::OnMouseDown(GadgetMouseEvent & e)
{
    if (! (MouseButton::kPrimary == e.button()) )
        return;
    //左键按下 
    drag_ = true;
    start_sel_ = end_sel_ = GetIndexByRelativePos(e.x(), e.y());
    UpdateCursorPos();
    gadget_.Invalidate();
}

void TextArea::OnMouseUp(GadgetMouseEvent & e)
{
    drag_ = false;
    gadget_.Invalidate();
}

void TextArea::OnMouseMove(GadgetMouseEvent & e)
{
    if (drag_)
    {
        end_sel_ = GetIndexByRelativePos(e.x(), e.y());
        UpdateCursorPos();
        gadget_.Invalidate();
    }
}

void TextArea::UpdateCursorPos()
{
    //确保end_sel在显示框内
    int cursor_x = 0;
    if (fragments_.empty())
        cursor_x = 0;
    else if (end_sel_ >= static_cast<int>(fragments_.size()))
        cursor_x = fragments_.rbegin()->start + fragments_.rbegin()->width;
    else
    {
        cursor_x = fragments_[end_sel_].start;
    }

    Size size;
    gadget_.GetSize(size);
    int abs_left = MapScrollX(0);
    int abs_right = MapScrollX(size.width());
    //光标在编辑框内
    if (cursor_x >= abs_left && cursor_x < abs_right)
        return;

    if (cursor_x < abs_left)
    {//在可视区左边
        ScrollX(cursor_x - abs_left);
    }
    else if (cursor_x >= abs_right)
    {
        ScrollX(cursor_x - abs_right);
    }
}

void TextArea::OnClientDraw(GadgetWrap & wrap, Painter & painter)
{
    CalcCache();

    DrawBackground(painter);
    DrawScript(painter);
}

void TextArea::DrawBackground(Painter & painter)
{
    //绘制背景
    Size size;
    gadget_.GetSize(size);
    Rect bounds = Rect::Make(size);
    Paint paint;
    paint.color = bg_color_;
    painter.DrawRect(bounds, paint);
}

void TextArea::DrawScript(Painter & painter)
{
    if (text_.empty())
        return;

    painter.Translate(GetCurrentScrollX(), GetCurrentScrollY());

    Size size;
    gadget_.GetSize(size);

    Font font;
    font.family = famliy_name_.data();
    font.size = text_size_;
    font.style = font_style_;
    FontMetrics fm;
    GetFontMetrics(font, fm);
    int top = size.height() / 2 - static_cast<int>((fm.fBottom - fm.fTop) / 2) ;

    Paint paint;

    for (size_t i = 0; i < text_.size(); ++i)
    {
        auto & fragment = fragments_[i];
        if (IsSelected(i))
        {
            Rect rect;
            rect.SetXYWH(fragment.start, 0, fragment.width, size.height());
            paint.color = selected_bg_color_;
            painter.DrawRect(rect, paint);
            paint.color = selected_text_color_;
        }
        else
            paint.color = normal_text_color_;

        Point pt(fragment.start, top);
        painter.DrawScript(text_.data() + i, 1, pt, paint, font);
    }
}

void TextArea::OnClientSizeChanged(GadgetWrap & wrap)
{
    cache_dirty_ = true;
}

bool TextArea::OnClientHitTest(GadgetWrap & wrap, const Point & pt)
{
    return true;
}

void TextArea::CalcCache()
{
    if (!cache_dirty_)
        return;

    fragments_.clear();

    if (!text_.empty())
    {
        Font font;
        font.family = famliy_name_.data();
        font.size = text_size_;
        font.style = font_style_;
        auto width = MeasureText(font, text_.data(), text_.size(), nullptr, 0);

        //计算Fragment
        int last_width = 0;
        for (size_t i = 1; i <= text_.size(); ++i)
        {
            auto width = MeasureText(font, text_.data(), i, nullptr, 0);
            Fragment fragment = { last_width, width - last_width};
            fragments_.push_back(fragment);
            last_width = width;
            if (i == text_.size())
            {
                Size desired_size;
                gadget_.GetSize(desired_size);
                desired_size.set_width(width);
                Scroller::SetDesiredSize(desired_size);
            }
        }
    }
    
    cache_dirty_ = false;
}

bool TextArea::IsSelected(int index)
{
    if (start_sel_ == end_sel_)
        return false;

    int start = (std::min)(start_sel_, end_sel_);
    int end = (std::max)(start_sel_, end_sel_);

    return (index >= start && index < end);
}

int TextArea::GetIndexByRelativePos(int x, int y)
{
    return GetIndexByPos(MapScrollX(x), MapScrollY(y));
}

int TextArea::GetIndexByPos(int x, int y)
{
    if (fragments_.empty())
        return 0;

    auto & first_fragment = *fragments_.begin();
    if (x < first_fragment.start)
        return 0;

    auto & last_fragment = *fragments_.rbegin();
    if (last_fragment.start + last_fragment.width / 2 < x)
        return fragments_.size();//末尾

    for (size_t i = 0; i < fragments_.size(); ++i)
    {
        auto & fragment = fragments_[i];
        int left = fragment.start;
        int right = fragment.start + fragment.width;

        if (x >= left && x < right)
        {
            int mid = fragment.start + fragment.width / 2;
            if (x < mid)
                return i;
            else
                return i + 1;
        }
    }

    assert(0);
    return 0;
}

}
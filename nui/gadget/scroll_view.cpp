
#include "scroll_view.h"
#include <nui/base/painter.h>
#include <nui/gadget/events.h>
#include <nui/gadget/world.h>
#include <nui/layout/layout.h>

#include <algorithm>

namespace nui
{

int GetScrollPos(int viewport_size, int content_size, int current_pos)
{
    int max = std::max(content_size - viewport_size, 0);
    if (current_pos < 0)
        return 0;
    if (current_pos > max)
        return max;
    return current_pos;
}

//确保滚动值不会超出范围
void SetScrollBounds(ScopedGadget viewport, ScopedGadget view)
{
    if (!view)
        return;

    int x = GetScrollPos(viewport->GetWidth(), view->GetWidth(), -view->GetLeft());
    int y = GetScrollPos(viewport->GetHeight(), view->GetHeight(), -view->GetTop());

    view->SetLoc(Point::Make(-x, -y));
}

int AdjustPosition(int current_position, int new_position, int content_size, int viewport_size)
{
    if (current_position == new_position)
        return new_position;
    if (new_position < 0)
        return 0;
    const int max_position = std::max(0, content_size - viewport_size);
    return (new_position > max_position) ? max_position : new_position;
}

ScrollView::ScrollView()
:contents_(nullptr),
auto_hide_scrollbar_(true)
{
    horiz_sb_ = Adopt(new ScrollBar(ScrollBar::kHorizontal));
    vert_sb_ = Adopt(new ScrollBar(ScrollBar::kVertal));

    AddChild(horiz_sb_);
    AddChild(vert_sb_);

    //默认不显示
    horiz_sb_->SetVisible(false);
    horiz_sb_->set_controller(this);
    vert_sb_->SetVisible(false);
    vert_sb_->set_controller(this);
}

ScrollView::~ScrollView()
{
    horiz_sb_->DecRef();
    vert_sb_->DecRef();
}

void ScrollView::SetContents(ScopedGadget contents)
{
    if (contents_ == contents)
        return;
    if (contents_)
        RemoveChild(contents_);
    if (contents)//滚动条在上面
        AddChildAt(contents, 0);
    contents_ = contents;
    Layout();
}

//contents 左上角为起点, 该矩形 
Rect ScrollView::GetContentVisibleRect() const
{
    if (!contents_)
        return Rect();
    return Rect(-contents_->GetLeft(),
                -contents_->GetTop(),
                GetWidth(), GetHeight());
}

void ScrollView::SetScrollBarAutoHide(bool auto_hide)
{
    auto_hide_scrollbar_ = auto_hide;
}

void ScrollView::SetHorizontalScrollBar(ScopedScrollBar horiz_sb)
{
    horiz_sb->SetVisible(horiz_sb_->visible());
    horiz_sb->set_controller(this);
    horiz_sb_ = horiz_sb;
}

void ScrollView::SetVerticalScrollbar(ScopedScrollBar vert_sb)
{
    vert_sb->SetVisible(vert_sb_->visible());
    vert_sb_->set_controller(this);
    vert_sb_ = vert_sb;
}

//移动滚动条 滚动条被移动时主动调用
void ScrollView::ScrollToPosition(ScopedScrollBar source, int position)
{
    if (!contents_)
        return;

    if (source == horiz_sb_ && horiz_sb_->visible())
    {//横向滚动条
        position = AdjustPosition(-contents_->GetLeft(), position, 
                                  contents_->GetWidth(), GetWidth());
        contents_->SetLoc(Point(-position, contents_->GetTop()));
    }
    else if (source == vert_sb_ && vert_sb_->visible())
    {//垂直滚动条
        position = AdjustPosition(-contents_->GetTop(), position, 
                                  contents_->GetHeight(), GetHeight());
        contents_->SetLoc(Point(contents_->GetLeft(), -position));
    }
    else
        assert(0);
    Invalidate();
}

int ScrollView::GetScrollIncrement(ScopedScrollBar source, bool is_page, bool is_positive)
{
    //bool is_horizontal = source->IsHorizontal();
    //int amount = 0;
    //if (contents_)
    //{
    //    if (is_page)
    //    {
    //        amount = contents_->GetPageScrollIncrement(this, is_horizontal, is_positive);
    //    }
    //    else
    //    {
    //        amount = contents_->GetLineScrollIncrement(this, is_horizontal, is_positive);
    //    }

    //    if (amount > 0)
    //        return amount;
    //}
    //if (is_page)
    //    return is_horizontal ? contents_viewport_->GetWidth() : contents_viewport_->GetHeight();
    //return is_horizontal ? contents_viewport_->GetWidth() / 5 :
    //    contents_viewport_->GetHeight() / 5;
    return 0;
}

void ScrollView::OnChildPreferredSizeChanged(ScopedGadget child)
{
    Layout();
}

void ScrollView::Layout()
{
    Rect viewport_bounds;
    GetLocalBounds(viewport_bounds);
    if (viewport_bounds.isEmpty())
        return;

    Size viewport_size = Size::Make(viewport_bounds.width(), viewport_bounds.height());
    //layout content
    contents_->SetSize(contents_->GetPreferredSize());
    if (layout_)
        layout_->Arrange(this);

    if (contents_)
        contents_->Layout();

    //控制滚动条显示还是隐藏
    ScrollbarProperty horiz_sb_required = kHide;
    ScrollbarProperty vert_sb_required = kHide;
    if (contents_)
    {
        Size content_size = Size::Make(contents_->GetWidth(), contents_->GetHeight());
        ComputeScrollBarsProperty(viewport_size, content_size, horiz_sb_required, vert_sb_required);
    }
    SetScrollbarProperty(horiz_sb_, horiz_sb_required);
    SetScrollbarProperty(vert_sb_, vert_sb_required);
    if (horiz_sb_required != kHide)
    {
        int height_offset = horiz_sb_->GetContentOverlapSize();
        horiz_sb_->SetLoc(Point::Make(0, viewport_bounds.bottom() - height_offset));
        horiz_sb_->SetSize(Size::Make(viewport_bounds.width(), height_offset));
    }
    if (vert_sb_required != kHide)
    {
        int width_offset = vert_sb_->GetContentOverlapSize();
        vert_sb_->SetLoc(Point::Make(viewport_bounds.right() - width_offset, 0));
        vert_sb_->SetSize(Size::Make(width_offset, viewport_bounds.height()));
    }
    //修正滚动的位置
    SetScrollBounds(this, contents_);
    //更新滚动条位置
    UpdateScrollbarPositions();
    Invalidate();
}

//rect 以contents为起点
void ScrollView::ScrollContentsRegionToBeVisible(const Rect & rect)
{
    //不需要显示滚动条
    if (!contents_ || (!horiz_sb_->visible() && !vert_sb_->visible()))
        return;

    Rect content_bounds;
    contents_->GetLocalBounds(content_bounds);
    content_bounds.Intersect(rect);
    if (content_bounds.isEmpty())//要显示的区域 不在contents区域内 直接返回
        return;

    Rect vis_rect = GetContentVisibleRect();
    vis_rect.Intersect(content_bounds);
    if (vis_rect == content_bounds)//已经显示的区域中已经包含了要显示的区域
        return;

    contents_->SetLoc(Point::Make(vis_rect.left(), vis_rect.top()));
    //更新滚动条位置
    UpdateScrollbarPositions();
    //const int contents_max_x = std::max(contents_viewport_->GetWidth(), contents_->GetWidth());
    //const int contents_max_y = std::max(contents_viewport_->GetHeight(), contents_->GetHeight());

    //int x = std::max(0, std::min(contents_max_x, rect.left()));
    //int y = std::max(0, std::min(contents_max_y, rect.top()));

    //const int max_x = std::min(contents_max_x,
    //    x + std::min(rect.width(), contents_viewport_->GetWidth()));
    //const int max_y = std::min(contents_max_y,
    //    y + std::min(rect.height(), contents_viewport_->GetHeight()));

    ////当前的显示区 已经包含了 要显示的区域
    //
    //if (vis_rect.Contains(Rect(x, y, max_x - x, max_y - y)))
    //    return;

    //const int new_x =
    //    (vis_rect.left() > x) ? x : std::max(0, max_x - contents_viewport_->GetWidth());
    //const int new_y =
    //    (vis_rect.top() > y) ? y : std::max(0, max_y - contents_viewport_->GetHeight());
}

void ScrollView::ComputeScrollBarsProperty(
    const Size & viewport_size, 
    const Size & content_size, 
    ScrollbarProperty & horiz_is_shown,
    ScrollbarProperty &vert_is_shown) const
{
    if (content_size.width() <= viewport_size.width() &&
        content_size.height() <= viewport_size.height())
    {
        horiz_is_shown = kHide;
        vert_is_shown = kHide;
    }
    else if (content_size.width() <= viewport_size.width())
    {
        horiz_is_shown = kHide;
        vert_is_shown = kNormal;
    }
    else if (content_size.height() <= viewport_size.height())
    {
        horiz_is_shown = kNormal;
        vert_is_shown = kHide;
    }
    else
    {
        horiz_is_shown = kNormal;
        vert_is_shown = kNormal;
    }
    //滚动条不自动隐藏
    if (!auto_hide_scrollbar_)
    {
        if (horiz_is_shown == kHide)
            horiz_is_shown = kDisable;
        if (vert_is_shown == kHide)
            vert_is_shown = kDisable;
    }
        
}


void ScrollView::SetScrollbarProperty(ScopedScrollBar bar, ScrollbarProperty property)
{
    if (!bar)
        return;
    if (kHide == property)
    {
        bar->SetVisible(false);
    }
    else
    {
        bar->SetVisible(true);
        if (kDisable == property)
            bar->SetEnable(false);
        else
            bar->SetEnable(true);
    }
}


void ScrollView::UpdateScrollbarPositions()
{
    if (!contents_)
        return;
    if (horiz_sb_->visible())
    {
        int vw = GetWidth();
        int cw = contents_->GetWidth();
        int origin = contents_->GetLeft();
        horiz_sb_->Update(vw, cw, -origin);
    }
    if (vert_sb_->visible())
    {
        int vh = GetHeight();
        int ch = contents_->GetHeight();
        int origin = contents_->GetTop();
        vert_sb_->Update(vh, ch, -origin);
    }
}


}
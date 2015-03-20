#ifndef NUI_GADGET_SCROLL_VIEW_H_
#define NUI_GADGET_SCROLL_VIEW_H_

#include <nui/gadget/gadget.h>
#include <nui/base/rect.h>
#include <nui/gadget/scroll_bar.h>
namespace nui
{

class ScrollView : public Gadget, public ScrollBarController
{
    enum ScrollbarProperty
    {
        kHide,
        kDisable,
        kNormal,
    };
public:
    ScrollView();

    ~ScrollView();

    void SetContents(ScopedGadget contents);

    Rect GetContentVisibleRect() const;

    void SetScrollBarAutoHide(bool auto_hide);

    ScopedScrollBar GetHorizontalScrollBar() const;

    ScopedScrollBar GetVerticalScrollBar() const;

    void SetHorizontalScrollBar(ScopedScrollBar horiz_sb);

    void SetVerticalScrollbar(ScopedScrollBar vert_sb);

    void ScrollToPosition(ScopedScrollBar source, int position) override;

    int GetScrollIncrement(ScopedScrollBar source, bool is_page, bool is_positive) override;

    void Layout() override;
protected:
    void OnChildPreferredSizeChanged(ScopedGadget child) override;
private:
    void ScrollContentsRegionToBeVisible(const Rect & rect);

    void ComputeScrollBarsProperty(const Size & viewport_size, const Size & content_size, 
                                   ScrollbarProperty & horiz_is_shown, ScrollbarProperty &vert_is_shown) const;

    void SetScrollbarProperty(ScopedScrollBar control, ScrollbarProperty property);

    void UpdateScrollbarPositions();

    ScopedGadget contents_;

    ScopedScrollBar horiz_sb_;
    ScopedScrollBar vert_sb_;

    bool auto_hide_scrollbar_;

};

}
#endif
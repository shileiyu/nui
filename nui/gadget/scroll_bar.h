#ifndef NUI_GADGET_SCROLL_BAR_H_
#define NUI_GADGET_SCROLL_BAR_H_


#include <nui/gadget/gadget.h>

namespace nui
{

class ScrollBar;

class ScrollBarController
{
public:
    virtual void ScrollToPosition(ScopedScrollBar source, int position) = 0;

    virtual int GetScrollIncrement(ScopedScrollBar source, bool is_page, bool is_positive){ return 0; }
};

class ScrollBar : public Gadget
{
public:
    enum Style
    {
        kHorizontal,
        kVertal,
    };
private:
    enum State
    {
        kNone,
        kSlider,
        kTrack,
    };
public:
    ScrollBar(Style style);

    ~ScrollBar();
    
    void SetEnable(bool enable);

    bool IsEnable() const;

    Style style() const;

    void set_controller(ScrollBarController * controller);

    void Update(int viewport_size, int content_size, int current_pos);

    int GetMaxPosition() const;

    int GetMinPosition() const;

    int GetPosition() const ;
    //virtual void OnMouseEnteredScrollView(const ui::MouseEvent& event);
    //virtual void OnMouseExitedScrollView(const ui::MouseEvent& event);
    bool ScrollByContentsOffset(int contents_offset);

    void Dispatch(Event & e) override;

    int GetContentOverlapSize() const;
protected:
    void OnMouseMove(nui::ScopedGadget gadget, nui::MouseEvent & e);

    void OnMouseDown(nui::ScopedGadget gadget, nui::MouseEvent & e);

    void OnMouseUp(nui::ScopedGadget gadget, nui::MouseEvent & e);

    void OnMouseOut(nui::ScopedGadget gadget, nui::MouseEvent & e);

    void OnWheel(nui::ScopedGadget gadget, nui::WheelEvent & e);

    void OnDraw(Painter & painter);
private:
    void ScrollContentsToOffset();

    void UpdateHScrollBarArea();

    void UpdateVScrollBarArea();

    State GetStateByPos(const Point & pt);

    void ScrollBarMoveVertical(int dy);

    void ScrollBarMoveHorizontal(int dx);
private:
    Style style_;
    ScrollBarController * controller_;
    int max_pos_;
    bool enable_;
    int contents_scroll_offset_;
    int content_size_;
    int viewport_size_;
    Rect slider_rect_;
    float scale_;
    State left_click_state_;
    State hover_state_;
    Point last_mouse_;
};


}
#endif
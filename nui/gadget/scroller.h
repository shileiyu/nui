#ifndef NUI_SCROLLER_H_
#define NUI_SCROLLER_H_

#include <nui/gadget/gadget.h>
#include <nui/base/rect.h>

namespace nui
{

class Scroller : public GadgetClient
{
protected:
    enum State
    {
        kNone,
        kVSlider,
        kHSlider,
        kClient,
    };
public:
    Scroller();

    virtual ~Scroller();

    bool Create(GadgetWorld * world);

    void Destroy();

    GadgetWrap & gadget();

    void Draw(GadgetWrap & wrap, Painter &) override;

    void Dispatch(GadgetWrap & wrap, GadgetEvent &) override;

    bool HitTest(GadgetWrap & wrap, const Point & pt) override;

    void OnSizeChanged(GadgetWrap & wrap) override;
protected:
    virtual void OnClientDispatch(GadgetWrap & wrap, GadgetEvent & e);

    virtual void OnClientDraw(GadgetWrap & wrap, Painter & painter);

    virtual void OnClientSizeChanged(GadgetWrap & wrap);

    virtual bool OnClientHitTest(GadgetWrap & wrap, const Point & pt);

    void SetDesiredSize(const Size & size);

    void SetCurrentScrollX(int current);

    void SetCurrentScrollY(int current);

    void ScrollX(int dx);

    void ScrollY(int dy);

    int GetCurrentScrollY() const;

    int GetCurrentScrollX() const;

    int MapScrollX(int x) const;

    int MapScrollY(int y) const;

    void ShowScrollBar(bool show);
private:
    bool OnScrollerMouseEvent(GadgetMouseEvent & e);

    bool OnScrollerMouseDown(GadgetMouseEvent & e);

    bool OnScrollerMouseUp(GadgetMouseEvent & e);

    bool OnScrollerMouseMove(GadgetMouseEvent & e);

    bool OnScrollerMouseOut(GadgetMouseEvent & e);

    bool OnScrollerWheel(GadgetWheelEvent & e);

    void DrawScroller(Painter & painter);

    void DrawHScroller(Painter & painter);

    void DrawVScroller(Painter & painter);

    void UpdateScrollBar();

    void UpdateScrollValue();

    void UpdateVScrollBarArea();

    void UpdateHScrollBarArea();

    void UpdateHSliderArea();

    void UpdateVSliderArea();

    void ScrollerBarMoveVertical(int dy);

    void ScrollerBarMoveHorizontal(int dx);

    State GetStateByPos(const Point & pt);
protected:
    GadgetWrap gadget_;
private:
    Size desired_size_;
    //垂直滚动条
    Rect max_vertical_rect_;
    Rect slider_vertical_rect_;
    float scale_vertical_;
    int max_scroll_y_;
    int cur_scroll_y_;
    //水平滚动条
    Rect max_horizontal_rect_;
    Rect slider_horizontal_rect_;
    float scale_horizontal_;
    int max_scroll_x_;
    int cur_scroll_x_;
    

    bool show_scrollbar_;
    Point last_mouse_;
    State left_click_state_;
    State hover_state_;

    bool cache_dirty_;
};

}
#endif
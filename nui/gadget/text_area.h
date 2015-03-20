#ifndef NUI_GADGET_TEXT_AREA_H
#define NUI_GADGET_TEXT_AREA_H


#include <nui/gadget/scroller.h>
#include <nui/base/painter.h>

namespace nui
{

class TextArea : public Scroller
{
private:
    struct Fragment
    {
        int start;
        int width;
    };
public:
    TextArea();

    bool Create(GadgetWorld * world);

    void Destroy();

    void SetText(const wchar_t * text);
protected:
    void OnClientDispatch(GadgetWrap & wrap, GadgetEvent & e) override;

    void OnClientDraw(GadgetWrap & wrap, Painter & painter) override;

    void OnClientSizeChanged(GadgetWrap & wrap) override;

    bool OnClientHitTest(GadgetWrap & wrap, const Point & pt) override;
private:
    void OnMouseMove(GadgetMouseEvent & e);

    void OnMouseDown(GadgetMouseEvent & e);

    void OnMouseUp(GadgetMouseEvent & e);
private:
    void DrawBackground(Painter & painter);

    void DrawScript(Painter & painter);

    void CalcCache();

    bool IsSelected(int index);

    int GetIndexByPos(int x, int y);

    int GetIndexByRelativePos(int x, int y);

    void UpdateCursorPos();
private:
    std::wstring text_;
    Color bg_color_;
    Color selected_bg_color_;
    Color selected_text_color_;
    Color normal_text_color_;
    std::string famliy_name_;
    Font::Style font_style_;
    uint32_t text_size_;
    std::vector<Fragment> fragments_;
    bool cache_dirty_;
    int start_sel_;
    int end_sel_;
    bool edit_;
    bool drag_;
};


}

#endif
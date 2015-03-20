#ifndef NUI_TEST_LABEL_BUTTON_H_
#define NUI_TEST_LABEL_BUTTON_H_

#include <nui/gadget/gadget.h>
#include <nui/gadget/label.h>
#include <nui/gadget/picture.h>
#include <nui/gadget/events.h>
#include <nui/layout/multi.h>
#include <nui/layout/sizer.h>

namespace nui
{

class Button;
class ButtonClient
{
public:
    virtual void OnClick(Button*  bt) = 0;
};

class  Button : public nui::GadgetListener
{
public:
    enum BGType
    {
        kLinear = 0,
        kImage = 1,
        kColor = 2
    };

    struct LinearColors
    {
        nui::Color color_top;
        nui::Color color_bottom;
    };
protected:
    struct State
    {
        enum Enum
        {
            kNone = 0,
            kNormal = 1,
            kHighlighted = 2,
            kValidPushed = 4,
            kInvalidPushed = 8,
            kDisabled = 16
        };
    };
    typedef nui::BitwiseEnum<State> ButtonState;
    enum Facade
    {
        kFaNormal = 0,
        kFaHighlighted,
        kFaClicked,
        kFaDisabled,

        kFaCount,
    };
public:
    Button();

    virtual ~Button();

    bool init();

    void fini();

    bool SetBGLinear(const LinearColors linear_colors[4]);

    bool SetBGImage(nui::Pixmap & pixmap);

    bool SetBGColor(const nui::Color colors[4]);

    bool IsEnabled() const;

    void SetEnabled(bool val);

    void SetClient(ButtonClient * client);

    nui::ScopedGadget GetGadget();

    nui::EventHandler * Query(nui::ScopedGadget gadget, nui::GadgetEvent & e);

    void OnMouseMove(nui::ScopedGadget gadget, nui::GadgetEvent & e);

    void OnMouseDown(nui::ScopedGadget gadget, nui::GadgetEvent & e);

    void OnMouseUp(nui::ScopedGadget gadget, nui::GadgetEvent & e);

    void OnMouseOver(nui::ScopedGadget gadget, nui::GadgetEvent & e);

    void OnMouseOut(nui::ScopedGadget gadget, nui::GadgetEvent & e);

    void OnBlur(nui::ScopedGadget gadget, nui::GadgetEvent & e);

    void OnFocus(nui::ScopedGadget gadget, nui::GadgetEvent & e);
protected:
    static bool SetPictureNineImage(nui::Picture& picture, nui::Pixmap& pix_map);

    static bool ExtractPixmap(nui::Pixmap& pixmap_big,
        nui::Pixmap* pixmap);

    void SetState(State::Enum s);

    void SwitchState(State::Enum s);

    State::Enum GetCurrencyState();

    void Trigger();

    void OnPrimaryButtonDown(const int x, const int y);

    void OnPrimaryButtonUp(const int x, const int y);

    bool IsPointIn(const int x, const int y);

    Facade GetCurrencyFacade();

    virtual void DoActionForToNormal();

    virtual void DoActionForToHighlighted();

    virtual void DoActionForToValidPushed();

    virtual void DoActionForToInvalidPushed();

    virtual void DoActionForToDisabled();

    virtual bool IsTargetGadget(nui::ScopedGadget gadget);

    virtual bool IsPhase(nui::GadgetEventPhase phase);
private:
    void SetBGLinearGradient(const LinearColors& linear_colors);

    void UpdateFacadeProperty(Facade fa);
protected:
    ButtonState  state_;
    ButtonClient *client_;
    BGType       type_;
    nui::ScopedPicture background_;
    LinearColors linear_colors_[kFaCount];
    nui::Pixmap  pixmap_[kFaCount];
    nui::Color   color_[kFaCount];
    nui::MultiLayout multi_layout_;

    nui::EventAdapter<Button> adapter_mouse_move_;
    nui::EventAdapter<Button> adapter_mouse_down_;
    nui::EventAdapter<Button> adapter_mouse_up_;
    nui::EventAdapter<Button> adapter_mouse_over_;
    nui::EventAdapter<Button> adapter_mouse_out_;
    nui::EventAdapter<Button> adapter_blur_;
    nui::EventAdapter<Button> adapter_focus_;

};


class LabelButton : public Button
{
public:
    LabelButton();

    ~LabelButton();

    bool init();

    void fini();

    void SetText(const wchar_t * text);

    void SetTextSize(uint32_t font_size);

    void SetTextColor(const nui::Color color[4]);

    void SetFont(const char * famliy_name, const nui::Font::Style& font_style);
protected:
    void UpdateFacadeProperty(Facade fa);

    void DoActionForToNormal() override;

    void DoActionForToHighlighted() override;

    void DoActionForToValidPushed() override;

    void DoActionForToInvalidPushed() override;

    void DoActionForToDisabled() override;

    bool IsTargetGadget(nui::ScopedGadget gadget) override;
protected:
    nui::ScopedLabel label_;
private:
    nui::Color font_color_[kFaCount];
    nui::PixelSizer layout_;
};

}
#endif
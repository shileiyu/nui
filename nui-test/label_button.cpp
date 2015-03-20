#include "label_button.h"

namespace nui
{


nui::Color stroke_color = { 0xff, 0, 0, 0xff };

Button::Button()
    : client_(nullptr)
{
    state_.Set(State::kNormal), state_.Clear(State::kDisabled);

    adapter_mouse_move_.Register(this, &Button::OnMouseMove);
    adapter_mouse_down_.Register(this, &Button::OnMouseDown);
    adapter_mouse_up_.Register(this, &Button::OnMouseUp);
    adapter_mouse_over_.Register(this, &Button::OnMouseOver);
    adapter_mouse_out_.Register(this, &Button::OnMouseOut);
    adapter_focus_.Register(this, &Button::OnFocus);
    adapter_blur_.Register(this, &Button::OnBlur);
}

Button:: ~Button()
{
    fini();

}

bool Button::init()
{
    background_ = Adopt(new nui::Picture);
    if (background_)
    {
        background_->SetLayout(&multi_layout_);
        background_->SetListener(this);
        return true;
    }
    return false;
}

void Button::fini()
{
    multi_layout_.DetachAll();
    background_.Reset();
}

void Button::Trigger()
{
    if (!state_.Test(State::kDisabled))
        client_ ? client_->OnClick(this) : 0;
}

void Button::SetClient(ButtonClient * client)
{
    client_ = client;
}

nui::ScopedGadget Button::GetGadget()
{
    return background_;
}

bool Button::IsEnabled() const
{
    return !state_.Test(State::kDisabled);
}

void Button::SetEnabled(bool b)
{
    bool pre_enabled = !state_.Test(State::kDisabled);
    if (pre_enabled != b)
    {
        if (b)
        {
            state_.Clear(State::kDisabled);
            State::Enum s = GetCurrencyState();
            SwitchState(s);
        }
        else
        {
            state_.Set(State::kDisabled);
            DoActionForToDisabled();
        }
        background_->Invalidate();
    }
}

void Button::OnMouseDown(nui::ScopedGadget gadget, nui::GadgetEvent & e)
{
    using namespace nui;
    if (e.type() != GadgetEventType::kMouseDown)
        return;
    if (IsPhase(e.phase()) && IsTargetGadget(e.target()))
    {
        GadgetMouseEvent * me = GadgetMouseEvent::FromEvent(e);
        MouseButton bt = me->button();
        if (MouseButton::kPrimary == bt)
        {
            OnPrimaryButtonDown(me->x(), me->y());
        }
    }
}

void Button::OnMouseOver(nui::ScopedGadget gadget, nui::GadgetEvent & e)
{
    using namespace nui;
    if (e.type() != GadgetEventType::kMouseOver)
        return;
    if (IsPhase(e.phase()) && IsTargetGadget(e.target()))
    {
        if (state_.Test(State::kNormal))
            SwitchState(State::kHighlighted);
    }
}

void Button::OnMouseOut(nui::ScopedGadget gadget, nui::GadgetEvent & e)
{
    using namespace nui;
    if (e.type() != GadgetEventType::kMouseOut)
        return;
    if (IsPhase(e.phase()) && IsTargetGadget(e.target()))
    {
        if (state_.Test(State::kHighlighted))
            SwitchState(State::kNormal);
    }
}

nui::EventHandler * Button::Query(nui::ScopedGadget gadget, nui::GadgetEvent & e)
{
    using namespace nui;
    switch (e.type())
    {
    case GadgetEventType::kMouseMove:
        return &adapter_mouse_move_;
    case GadgetEventType::kMouseDown:
        return &adapter_mouse_down_;
    case GadgetEventType::kMouseUp:
        return &adapter_mouse_up_;
    case GadgetEventType::kMouseOver:
        return &adapter_mouse_over_;
    case GadgetEventType::kMouseOut:
        return &adapter_mouse_out_;
    case GadgetEventType::kFocus:
        return &adapter_focus_;
    case GadgetEventType::kBlur:
        return &adapter_blur_;
    default:
        return nullptr;
        break;
    }
}

void  Button::OnMouseMove(nui::ScopedGadget gadget, nui::GadgetEvent & e)
{
    using namespace nui;
    if (e.type() != GadgetEventType::kMouseMove)
        return;
    if (IsPhase(e.phase()) && IsTargetGadget(e.target()))
    {
        GadgetMouseEvent * me = GadgetMouseEvent::FromEvent(e);
        if (state_.Test(State::kValidPushed))
        {
            if (!IsPointIn(me->x(), me->y()))
            {
                SwitchState(State::kInvalidPushed);
            }
        }
        else if (state_.Test(State::kInvalidPushed))
        {
            if (IsPointIn(me->x(), me->y()))
            {
                SwitchState(State::kValidPushed);
            }
        }
    }
}

void Button::OnMouseUp(nui::ScopedGadget gadget, nui::GadgetEvent & e)
{
    using namespace nui;
    if (e.type() != GadgetEventType::kMouseUp)
        return;
    if (IsPhase(e.phase()) && IsTargetGadget(e.target()))
    {
        GadgetMouseEvent * me = GadgetMouseEvent::FromEvent(e);
        MouseButton bt = me->button();
        if (MouseButton::kPrimary == bt)
        {
            OnPrimaryButtonUp(me->x(), me->y());
        }
    }
}

void Button::OnBlur(nui::ScopedGadget gadget, nui::GadgetEvent & e)
{
    using namespace nui;
    if (e.type() != GadgetEventType::kBlur)
        return;
    if (IsTargetGadget(e.target()))
    {
        nui::Picture::Stroke stroke;
        stroke.width = 1;
        stroke.color = nui::Color(0, 0, 0, 0);
        background_->SetStroke(stroke);
    }
}

void Button::OnFocus(nui::ScopedGadget gadget, nui::GadgetEvent & e)
{
    using namespace nui;
    if (e.type() != GadgetEventType::kFocus)
        return;
    GadgetFocusEvent * fe = GadgetFocusEvent::FromEvent(e);

    if (fe->IsTabTraversal() && IsTargetGadget(e.target()))
    {
        nui::Picture::Stroke stroke;
        stroke.width = 1;
        stroke.color = stroke_color;
        background_->SetStroke(stroke);
    }
}

/*处理主键按下事件*/
void Button::OnPrimaryButtonDown(const int x, const int y)
{
    using namespace nui;
    if (state_.Test(State::kHighlighted))
        SwitchState(State::kValidPushed);
}
/*处理主键弹起事件*/
void Button::OnPrimaryButtonUp(const int x, const int y)
{
    using namespace nui;
    if (state_.Test(State::kValidPushed))
    {
        SwitchState(State::kHighlighted);
        Trigger();
    }
    else if (state_.Test(State::kInvalidPushed))
    {
        SwitchState(State::kNormal);
    }
}

void Button::SetState(State::Enum s)
{
    state_.Clear(State::kNormal);
    state_.Clear(State::kHighlighted);
    state_.Clear(State::kValidPushed);
    state_.Clear(State::kInvalidPushed);
    state_.Set(s);
}

void Button::SwitchState(State::Enum s)
{
    using namespace  nui;
    SetState(s);
    if (state_.Test(State::kDisabled))
        return;
    switch (s)
    {
    case State::kNormal:
        DoActionForToNormal();
        break;
    case State::kHighlighted:
        DoActionForToHighlighted();
        break;
    case State::kValidPushed:
        DoActionForToValidPushed();
        break;
    case State::kInvalidPushed:
        DoActionForToInvalidPushed();
        break;
    default:
        break;
    }
}

bool Button::IsPointIn(const int x, const int y)
{
    using namespace nui;
    Rect rc;
    background_->GetLocalBounds(rc);
    return rc.isPointIn(x, y);
}

void Button::DoActionForToNormal()
{
    UpdateFacadeProperty(kFaNormal);
};

void Button::DoActionForToHighlighted()
{
    UpdateFacadeProperty(kFaHighlighted);
};

void Button::DoActionForToValidPushed()
{
    UpdateFacadeProperty(kFaClicked);
};

void Button::DoActionForToInvalidPushed()
{
    UpdateFacadeProperty(kFaHighlighted);
};

void Button::DoActionForToDisabled()
{
    UpdateFacadeProperty(kFaDisabled);
};

bool Button::IsTargetGadget(nui::ScopedGadget gadget)
{
    return  gadget == background_;
}

bool Button::IsPhase(nui::GadgetEventPhase phase)
{
    return phase == nui::kBubbling || phase == nui::kAtTarget;
}

Button::State::Enum  Button::GetCurrencyState()
{
    if (state_.Test(State::kNormal))
        return State::kNormal;
    else if (state_.Test(State::kHighlighted))
        return State::kHighlighted;
    else if (state_.Test(State::kValidPushed))
        return State::kValidPushed;
    else if (state_.Test(State::kInvalidPushed))
        return State::kInvalidPushed;
    return State::kNone;
}

void Button::SetBGLinearGradient(const LinearColors& linear_colors)
{
    using namespace nui;
    Picture::LinearGradient linear_gradient;
    float pos[2] = { 0.f, 1.f };
    Color colors[2] =
    {
        linear_colors.color_top,
        linear_colors.color_bottom
    };
    linear_gradient.begin[0] = 0.f;
    linear_gradient.begin[1] = 0.f;
    linear_gradient.end[0] = 0.f;
    linear_gradient.end[1] = 1.f;
    linear_gradient.color = colors;
    linear_gradient.pos = pos;
    linear_gradient.count = 2;
    background_->SetBackground(linear_gradient);
}

bool Button::SetPictureNineImage(nui::Picture & picture, nui::Pixmap& pix_map)
{
    using namespace nui;
    if (!pix_map.IsValid())
        return false;
    Picture::StrechImageNine image_nine;
    size_t width = pix_map.width();
    size_t height = pix_map.height();
    image_nine.center = Rect::Make(width / 3, height / 3, width / 3, height / 3);
    image_nine.pixmap = pix_map;
    picture.SetBackground(image_nine);
    return true;
}

bool Button::ExtractPixmap(
    nui::Pixmap& pixmap_big,
    nui::Pixmap* pixmap)
{
    using namespace nui;
    if (!pixmap_big.IsValid())
        return false;
    size_t width = pixmap_big.width();
    size_t height = pixmap_big.height() / 4;
    pixmap[kFaNormal] = pixmap_big.ExtractSubset(Rect::Make(0, 0, width, height));
    pixmap[kFaHighlighted] = pixmap_big.ExtractSubset(Rect::Make(0, height, width, height));
    pixmap[kFaClicked] = pixmap_big.ExtractSubset(Rect::Make(0, height * 2, width, height));
    pixmap[kFaDisabled] = pixmap_big.ExtractSubset(Rect::Make(0, height * 3, width, height));
    return true;
}

bool Button::SetBGLinear(const LinearColors linear_colors[4])
{
    type_ = kLinear;
    linear_colors_[kFaNormal] = linear_colors[kFaNormal];
    linear_colors_[kFaHighlighted] = linear_colors[kFaHighlighted];
    linear_colors_[kFaClicked] = linear_colors[kFaClicked];
    linear_colors_[kFaDisabled] = linear_colors[kFaDisabled];
    UpdateFacadeProperty(GetCurrencyFacade());
    return true;
}

bool Button::SetBGImage(nui::Pixmap& pixmap)
{
    type_ = kImage;
    if (!ExtractPixmap(pixmap, pixmap_))
        return false;
    UpdateFacadeProperty(GetCurrencyFacade());
    return true;
}

bool Button::SetBGColor(const nui::Color colors[4])
{
    type_ = kColor;
    color_[kFaNormal] = colors[kFaNormal];
    color_[kFaHighlighted] = colors[kFaHighlighted];
    color_[kFaClicked] = colors[kFaClicked];
    color_[kFaDisabled] = colors[kFaDisabled];
    UpdateFacadeProperty(GetCurrencyFacade());
    return true;
}

Button::Facade Button::GetCurrencyFacade()
{
    Facade fa = kFaNormal;
    if (!IsEnabled())
    {
        fa = kFaDisabled;
    }
    else
    {
        switch (GetCurrencyState())
        {
        case State::kHighlighted:
        case State::kInvalidPushed:
            fa = kFaHighlighted;
            break;
        case State::kValidPushed:
            fa = kFaClicked;
            break;
        default:
            break;
        }
    }
    return fa;
}

void Button::UpdateFacadeProperty(Facade fa)
{
    switch (type_)
    {
    case kLinear:
        SetBGLinearGradient(linear_colors_[fa]);
        break;
    case kImage:
        SetPictureNineImage(*background_.Get(), pixmap_[fa]);
        break;
    case kColor:
        background_->SetBackground(color_[fa]);
        break;
    }
}



LabelButton::LabelButton()
{
}

LabelButton::~LabelButton()
{
    fini();
}

bool LabelButton::init()
{
    if (!Button::init())
        return false;

    label_ = Adopt(new nui::Label);
    if (!label_)
        return false;

    background_->AddChild(label_);
    label_->SetFocusable(false);//文本不可接受焦点
    label_->SetHAlign(nui::Label::kCenter);
    layout_.left(0).right(0).top(0).bottom(0);
    layout_.Attach(label_);
    multi_layout_.Attach(&layout_);
    background_->Layout();
    return true;
    //nui::Picture::Stroke stroke;
    //stroke.width = 1;
    //stroke.color = nui::Color(0xff, 0, 0, 0);
    //background_->SetStroke(stroke);
}

void LabelButton::fini()
{
    layout_.Detach();
    if (label_)
    {
        label_->RemoveFromParent();
        label_.Reset();
    }

    Button::fini();
}

void LabelButton::SetText(const wchar_t * text)
{
    label_->SetText(text);
}

void LabelButton::SetTextSize(uint32_t font_size)
{
    label_->SetTextSize(font_size);
}

void LabelButton::SetTextColor(const nui::Color color[4])
{
    font_color_[kFaNormal] = color[kFaNormal];
    font_color_[kFaHighlighted] = color[kFaHighlighted];
    font_color_[kFaClicked] = color[kFaClicked];
    font_color_[kFaDisabled] = color[kFaDisabled];
    UpdateFacadeProperty(GetCurrencyFacade());
}

void LabelButton::UpdateFacadeProperty(Facade fa)
{
    label_->SetTextColor(font_color_[fa]);
}

void LabelButton::SetFont(const char * famliy_name,
    const nui::Font::Style& font_style)
{
    if (famliy_name)
        label_->SetFont(famliy_name, font_style);
}

bool LabelButton::IsTargetGadget(nui::ScopedGadget gadget)
{
    return gadget == label_;
}

void LabelButton::DoActionForToNormal()
{
    label_->SetTextColor(font_color_[kFaNormal]);
    Button::DoActionForToNormal();
}

void LabelButton::DoActionForToHighlighted()
{
    label_->SetTextColor(font_color_[kFaHighlighted]);
    Button::DoActionForToHighlighted();
}

void LabelButton::DoActionForToValidPushed()
{
    label_->SetTextColor(font_color_[kFaClicked]);
    Button::DoActionForToValidPushed();
}

void LabelButton::DoActionForToInvalidPushed()
{
    label_->SetTextColor(font_color_[kFaHighlighted]);
    Button::DoActionForToInvalidPushed();
}

void LabelButton::DoActionForToDisabled()
{
    label_->SetTextColor(font_color_[kFaDisabled]);
    Button::DoActionForToDisabled();
}

}
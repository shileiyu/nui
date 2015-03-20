#include "progress_bar.h"

namespace nui
{


static const int kFillMarginLT = 1;
static const int kFillMarginRB = 2;

ProgressBar::ProgressBar()
    :percent_(0.f)
{

}

ProgressBar::~ProgressBar()
{
    fini();
}

bool ProgressBar::init()
{

    background_ = Adopt(new nui::Picture);
    fill_ = Adopt(new nui::Picture);
    border_ = Adopt(new nui::Gadget);
    if (!background_ || !fill_ || !border_)
        return false;

    background_->AddChild(border_);
    border_->AddChild(fill_);
    fill_->SetFocusable(false);
    border_->SetFocusable(false);
    layout_border_.left(kFillMarginLT).top(kFillMarginLT).right(1).bottom(1);
    layout_border_.Attach(border_);
    background_->SetLayout(&layout_border_);

    layout_fill_.Set(0.0f, 0.0f, 1 - percent_, 0.0f);
    layout_fill_.Attach(fill_);
    border_->SetLayout(&layout_fill_);
    border_->Layout();
    return true;
}

void ProgressBar::fini()
{
    layout_fill_.Detach();
    layout_border_.Detach();

    if (fill_)
    {
        fill_->RemoveFromParent();
        fill_.Reset();
    }
    if (border_)
    {
        border_->RemoveFromParent();
        border_.Reset();
    }
    if (background_)
        background_.Reset();
}

nui::ScopedGadget ProgressBar::GetGadget()
{
    return background_;
}

void ProgressBar::SetColor(const nui::Color color[2])
{
    color_[0] = color[0];
    color_[1] = color[1];
    background_->SetBackground(color[0]);
    fill_->SetBackground(color[1]);
}

void ProgressBar::Update(float percent)
{
    if (percent < 0.f)
        percent = 0.f;
    if (percent > 1.f)
        percent = 1.f;
    percent_ = percent;

    layout_fill_.Set(0.0f, 0.0f, 1 - percent_, 0.0f);
    if (border_)
        border_->Layout();
}


}



#ifndef NUI_TEST_PROGRESS_BAR_H_
#define NUI_TEST_PROGRESS_BAR_H_

#include <nui/nui.h>
#include <nui/layout/sizer.h>
#include <nui/gadget/picture.h>

namespace nui
{

class ProgressBar
{
public:
    ProgressBar();

    ~ProgressBar();

    bool init();

    void fini();

    nui::ScopedGadget GetGadget();

    void SetColor(const nui::Color color[2]);

    void Update(float percent);
private:
    nui::ScopedPicture background_;
    nui::ScopedGadget border_;
    nui::ScopedPicture fill_;
    nui::PixelSizer layout_border_;
    nui::ScaleSizer layout_fill_;

    nui::Color color_[2];
    float percent_;
};


}




#endif
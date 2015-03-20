#ifndef NUI_GADGET_PICTURE_H_
#define NUI_GADGET_PICTURE_H_

#include <nui/nui.h>
#include <nui/base/color.h>
#include <nui/base/pixmap.h>
#include <nui/gadget/gadget.h>

namespace nui
{

class Picture : public Gadget
{
    enum BackgroundStyle
    {
        kSolid,
        kLinearGradient,
        kImage,

        kStyleCount,
    };

    enum ImageMode
    {
        kNone,
        kStrech,
        kStrechNine,
    };
    struct LinearGradientPri
    {
        float begin[2];
        float end[2];
        std::vector<nui::Color> colors;
        std::vector<float> positions;
    };


public:
    struct LinearGradient
    {
        float begin[2];
        float end[2];
        size_t count;
        nui::Color * color;
        float * pos;
    };

    struct StrechImage
    {
        nui::Pixmap pixmap;
        bool respect_ratio;//锁定比例拉伸
    };

    struct StrechImageNine
    {
        nui::Pixmap pixmap;
        nui::Rect center;
    };

    struct Stroke
    {
        int width;
        nui::Color color;
    };

public:
    Picture();

    ~Picture();

    void SetStroke(const Stroke & stroke);

    void SetBackground(const nui::Color & color);

    void SetBackground(const StrechImage & image);

    void SetBackground(const StrechImageNine & image);

    void SetBackground(const LinearGradient & gradient);
protected:
    void OnDraw(Painter & painter) override;
private:
    void DrawStroke(nui::Painter & painter);

    void DrawSolid(nui::Painter & painter);

    void DrawLinearGradient(nui::Painter & painter);

    void DrawImage(nui::Painter & painter);

    void DrawImageNine(nui::Painter & painter);

    void ResetBackground();
protected:
    Stroke stroke_;
    BackgroundStyle style_;
    nui::Color solid_;

    ImageMode image_mode_;
    nui::Rect image_center_;
    nui::Pixmap bitmap_;

    LinearGradientPri linear_;
};



}

#endif
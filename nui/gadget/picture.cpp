#include <nui/base/painter.h>
#include "picture.h"

namespace nui
{

Picture::Picture()
    :style_(kStyleCount), image_mode_(kNone),
    solid_(0)
{
    stroke_ = { 0, 0 };
}

Picture::~Picture()
{
    ResetBackground();
}

void Picture::SetStroke(const Stroke & stroke)
{
    stroke_.color = stroke.color;
    stroke_.width = stroke.width;
    Invalidate();
}

void Picture::Set(const nui::Color & color)
{
    ResetBackground();
    style_ = kSolid;
    solid_ = color;
    Invalidate();
}

void Picture::Set(const StrechImage & image)
{
    ResetBackground();
    style_ = kImage;
    image_mode_ = kStrech;
    bitmap_ = image.pixmap;
    Invalidate();
}

void Picture::Set(const StrechImageNine & image)
{
    ResetBackground();
    style_ = kImage;
    image_mode_ = kStrechNine;
    image_center_ = image.center;
    bitmap_ = image.pixmap;
    Invalidate();
}

void Picture::Set(const LinearGradient & gradient)
{
    ResetBackground();
    style_ = kLinearGradient;
    linear_.begin[0] = gradient.begin[0];
    linear_.begin[1] = gradient.begin[1];
    linear_.end[0] = gradient.end[0];
    linear_.end[1] = gradient.end[1];
    for (size_t i = 0; i < gradient.count; ++i)
    {
        linear_.colors.push_back(gradient.color[i]);
        linear_.positions.push_back(gradient.pos[i]);
    }
    Invalidate();
}

void Picture::OnDraw(Painter & painter)
{
    DrawSolid(painter);
    DrawLinearGradient(painter);
    DrawImage(painter);
    DrawImageNine(painter);

    DrawStroke(painter);

    //Paint paint;
    //paint.color.Set(0xff, 0xff, 0xff, 0xff);
    //Font font;
    //font.family = "Microsoft YaHei";
    //font.style = Font::kNormal;
    //font.size = 20;
    //Point pt(20, 20);
    //painter.DrawScript(L"xxxx", 4, pt, paint, font);
}

void Picture::DrawStroke(nui::Painter & painter)
{
    using namespace nui;
    if (stroke_.width > 0 && ColorGetA(stroke_.color) != 0)
    {
        Rect rect;

        Size size = GetSize();

        rect.SetLTRB(0, 0, size.width() - stroke_.width, size.height() - stroke_.width);
        Paint paint;
        paint.stroke_width = stroke_.width;
        paint.color = stroke_.color;
        paint.style = Paint::kStroke;

        painter.DrawRect(rect, paint);
    }
}

void Picture::DrawSolid(nui::Painter & painter)
{
    using namespace nui;

    if (style_ != kSolid)
        return;

    Rect rect;
    Size size = GetSize();
    rect.SetXYWH(0, 0, size.width(), size.height());

    if (ColorGetA(solid_))
    {//背景单色 alpha 不为空
        Paint paint;
        paint.color = solid_;
        paint.style = Paint::kFill;
        painter.DrawRect(rect, paint);
    }
}

void Picture::DrawLinearGradient(nui::Painter & painter)
{
    using namespace nui;
    if (style_ != kLinearGradient)
        return;

    nui::LinearGradient linear;

    Size size = GetSize();
    auto width = size.width();
    auto height = size.height();

    int begin_x = static_cast<int>(linear_.begin[0] * width);
    int begin_y = static_cast<int>(linear_.begin[1] * height);
    int end_x = static_cast<int>(linear_.end[0] * width);
    int end_y = static_cast<int>(linear_.end[1] * height);

    linear.begin.Set(begin_x, begin_y);
    linear.end.Set(end_x, end_y);
    linear.colors = &linear_.colors[0];
    linear.positions = &linear_.positions[0];
    linear.count = linear_.colors.size();
    linear.mode = nui::LinearGradient::kClamp;

    Rect rect;
    rect.SetXYWH(0, 0, width, height);

    Paint paint;
    painter.DrawGradient(rect, paint, linear);
}

void Picture::DrawImageNine(nui::Painter & painter)
{
    using namespace nui;
    if (kImage != style_ || kStrechNine != image_mode_)
        return;
    if (bitmap_.DrawsNothing())
        return;
    Rect dest;

    Size size = GetSize();
    auto width = size.width();
    auto height = size.height();

    dest.SetXYWH(0, 0, width, height);
    Rect src;
    src.SetXYWH(0, 0, bitmap_.width(), bitmap_.height());

    Paint paint;
    painter.DrawImageNine(bitmap_, src, image_center_, dest, paint);
}

void Picture::DrawImage(nui::Painter & device)
{
    using namespace nui;
    if (kImage != style_ || kStrech != image_mode_)
        return;

    if (bitmap_.DrawsNothing())
        return;

    Size size = GetSize();
    auto width = size.width();
    auto height = size.height();

    Rect dest;
    dest.SetXYWH(0, 0, width, height);
    Rect src;
    src.SetLTRB(0, 0, bitmap_.width(), bitmap_.height());
    Paint paint;
    device.DrawImage(bitmap_, src, dest, paint);
}

void Picture::ResetBackground()
{
    using namespace nui;
    style_ = kStyleCount;
    solid_ = 0;
    image_mode_ = kNone;

    linear_.colors.clear();
    linear_.positions.clear();
}


}

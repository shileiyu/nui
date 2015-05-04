
#include <nui/base/pixmap.h>
#include <nui/base/rect.h>
#include <nui/base/point.h>
#include <nui/base/color.h>

#include "SkCanvas.h"
#include "SkPixelRef.h"
#include "SkTypeface.h"
#include "SkDevice.h"
#include "SkGradientShader.h"

#include "pixpainter.h"
namespace nui
{

static SkRect ToSkRect(const nui::Rect & rect)
{
    SkRect srect;
    srect.setLTRB(
        SkIntToScalar(rect.left()),
        SkIntToScalar(rect.top()),
        SkIntToScalar(rect.right()),
        SkIntToScalar(rect.bottom())
        );

    return srect;
}

static Rect FromSkRect(const SkRect & skrect)
{
    Rect rect;
    rect.SetLTRB(
        SkScalarRoundToInt(skrect.left()),
        SkScalarRoundToInt(skrect.top()),
        SkScalarRoundToInt(skrect.right()),
        SkScalarRoundToInt(skrect.bottom()));
    return rect;
}

static SkIRect ToSkIRect(const nui::Rect & rect)
{
    SkIRect sire;
    sire.setXYWH(rect.left(), rect.top(), rect.width(), rect.height());
    return sire;
}


static SkColor ToSkColor(const nui::Color & color)
{
    return SkColorSetARGB(ColorGetA(color), ColorGetR(color),
        ColorGetG(color), ColorGetB(color));
}

static SkPoint ToSkPoint(const Point & pt)
{
    return SkPoint::Make(SkIntToScalar(pt.x()), SkIntToScalar(pt.y()));
}

static SkBitmap ToSkBitmap(Pixmap & pm)
{
    SkBitmap skpm;
    SkImageInfo info;
    PixStorage::Outline ol;
    Rect subset;
    pm.Lock(ol, subset);
    info.fWidth = ol.width;
    info.fHeight = ol.height;
    info.fColorType = kBGRA_8888_SkColorType;
    info.fAlphaType = kPremul_SkAlphaType;
    skpm.installPixels(info, ol.bits, ol.pitch);
    pm.Unlock();

    SkBitmap sksubsetpm;
    skpm.extractSubset(&sksubsetpm, ToSkIRect(subset));
    return sksubsetpm;
}

static SkPaint::Style ToSkPaintStyle(Paint::Style style)
{
    switch (style)
    {
    case Paint::kFill:
        return SkPaint::kFill_Style;
    case Paint::kStroke:
        return SkPaint::kStroke_Style;
    case Paint::kFillAndStroke:
        return SkPaint::kStrokeAndFill_Style;
    default:
        assert(0);
        return SkPaint::kFill_Style;
    }
}

static SkXfermode::Mode ToSkXfermode(Paint::Mode mode)
{
    switch (mode)
    {
    case Paint::kSrcOver:
        return SkXfermode::kSrcOver_Mode;
    case Paint::kSrc:
        return SkXfermode::kSrc_Mode;
    default:
        assert(0);
        return SkXfermode::kSrcOver_Mode;
    }
}
static SkTypeface * MakeSkTypeface(const char * family, Font::Style style)
{
    SkTypeface::Style face_style;

    switch (style)
    {
    case Font::kNormal:
        face_style = SkTypeface::kNormal;
        break;
    case Font::kBold:
        face_style = SkTypeface::kBold;
        break;
    case Font::kItalic:
        face_style = SkTypeface::kItalic;
        break;
    case Font::kBoldItalic:
        face_style = SkTypeface::kBoldItalic;
        break;
    default:
        assert(0);
        face_style = SkTypeface::kNormal;
    }
    return SkTypeface::CreateFromName(family, face_style);
}

static SkPaint ToSkPaint(const Paint & paint)
{
    SkPaint skpaint;
    skpaint.setColor(ToSkColor(paint.color));
    skpaint.setStrokeWidth(SkIntToScalar(paint.stroke_width));
    skpaint.setStyle(ToSkPaintStyle(paint.style));
    skpaint.setXfermodeMode(ToSkXfermode(paint.mode));
    return skpaint;
}

static SkShader * ToSkShader(const LinearGradient & linear)
{
    SkPoint pt[2] = 
    {
        ToSkPoint(linear.begin), 
        ToSkPoint(linear.end) 
    };
        
    std::vector<SkColor> colors;
    std::vector<SkScalar> positions;
    colors.resize(linear.count);
    positions.resize(linear.count);
        
    for (size_t i = 0; i < linear.count; ++i)
    {
        colors[i] = ToSkColor(linear.colors[i]);
        positions[i] = SkFloatToScalar(linear.positions[i]);
    }
        
    SkShader::TileMode mode = SkShader::kClamp_TileMode;
    switch (linear.mode)
    {
    case LinearGradient::kClamp:
        mode = SkShader::kClamp_TileMode;
        break;
    case LinearGradient::kRepeat:
        mode = SkShader::kRepeat_TileMode;
        break;
    case LinearGradient::kMirror:
        mode = SkShader::kMirror_TileMode;
        break;
    default:
        assert(0);
        return nullptr;
    }
        
    SkShader * shader = SkGradientShader::CreateLinear(pt, 
                                                       &colors[0], 
                                                       &positions[0], 
                                                       linear.count, 
                                                       mode);
    return shader;

}

void GetFontMetrics(const Font & font, FontMetrics & metrics)
{
    SkPaint paint;
    paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
    auto typeface = MakeSkTypeface(font.family, font.style);
    if (typeface)
        paint.setTypeface(typeface)->unref();
    paint.setTextSize(SkIntToScalar(font.size));
    paint.setAntiAlias(true);
    paint.setLCDRenderText(true);

    SkPaint::FontMetrics fm;
    paint.getFontMetrics(&fm);
    metrics.fFlags = fm.fFlags;
    metrics.fTop = fm.fTop;
    metrics.fAscent = fm.fAscent;
    metrics.fDescent = fm.fDescent;
    metrics.fBottom = fm.fBottom;
    metrics.fLeading = fm.fLeading;
    metrics.fAvgCharWidth = fm.fAvgCharWidth;
    metrics.fMaxCharWidth = fm.fMaxCharWidth;
    metrics.fXMin = fm.fXMin;
    metrics.fXMax = fm.fXMax;
    metrics.fXHeight = fm.fXHeight;
    metrics.fCapHeight = fm.fCapHeight;
    metrics.fUnderlineThickness = fm.fUnderlineThickness;
    metrics.fUnderlinePosition = fm.fUnderlinePosition;
}

int MeasureText(
                  const Font & font,
                  const wchar_t * text,
                  size_t length,
                  Rect * bounds,
                  float scale)
{
    SkPaint paint;
    paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
    auto typeface = MakeSkTypeface(font.family, font.style);
    if (typeface)
        paint.setTypeface(typeface)->unref();
    paint.setTextSize(SkIntToScalar(font.size));
    paint.setAntiAlias(true);
    paint.setLCDRenderText(true);

    if (bounds)
    {
        std::vector<SkRect> skbounds;
        skbounds.resize(length);

        float width = paint.measureText(text, length * 2, &skbounds[0], scale);

        for (size_t i = 0; i < length; ++i)
        {
            bounds[i] = FromSkRect(skbounds[i]);
        }
        return SkScalarRoundToInt(width);
    }
    else
        return SkScalarRoundToInt(paint.measureText(text, length * 2));
}

int GetTextWidths(const Font & font,
                  const wchar_t * text,
                  uint32_t length,
                  uint32_t width[],
                  uint32_t& str_length)
{
    SkPaint paint;
    paint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
    auto typeface = MakeSkTypeface(font.family, font.style);
    if (typeface)
        paint.setTypeface(typeface)->unref();
    paint.setTextSize(SkIntToScalar(font.size));
    paint.setAntiAlias(true);
    paint.setLCDRenderText(true);
    std::vector<SkScalar> skwidth;
    skwidth.resize(length);

    auto count = paint.getTextWidths(text, length * 2, &skwidth[0], 0);
    str_length = 0;
    for (size_t i = 0; i < length; ++i)
    {
        width[i] = SkScalarRoundToInt(skwidth[i]);
        str_length += width[i];
    }

    return count;
}

PixPainter::~PixPainter()
{
    ;
}

PixPainter::PixPainter(Pixmap & target)
{
    canvas_.reset(new SkCanvas(ToSkBitmap(target)));
    //paint_.reset(new SkPaint);
    //paint_->setTextEncoding(SkPaint::kUTF16_TextEncoding);
    //paint_->setAntiAlias(true);
    //paint_->setLCDRenderText(true);
    //paint_stack_.reset( new std::stack<SkPaint>);
}

void PixPainter::Save()
{
    canvas_->save();
}

void PixPainter::Restore()
{
    canvas_->restore();
}

void PixPainter::ClipRect(const Rect & rect)
{
    canvas_->clipRect(ToSkRect(rect));
}

void PixPainter::Translate(int x, int y)
{
    canvas_->translate(SkIntToScalar(x), SkIntToScalar(y));
}

void PixPainter::DrawLine(const Point & pt1, const Point & pt2, const Paint & paint)
{
    assert(canvas_);
    SkPoint spt1 = ToSkPoint(pt1);
    SkPoint spt2 = ToSkPoint(pt2);
    canvas_->drawLine(spt1.x(), spt1.y(), spt2.x(), spt2.y(), ToSkPaint(paint));
}

void PixPainter::DrawRect(const Rect & rect, const Paint & paint)
{
    assert(canvas_);
    canvas_->drawRect(ToSkRect(rect), ToSkPaint(paint));
}

void PixPainter::DrawImage(
    Pixmap & pixmap, 
    const Rect & src, 
    const Rect & dest,
    const Paint & paint)
{
    assert(canvas_);
    canvas_->drawBitmapRect(
        ToSkBitmap(pixmap), 
        &ToSkIRect(src), 
        ToSkRect(dest), 
        &ToSkPaint(paint));
}

void PixPainter::DrawImageNine(Pixmap & pixmap, 
                               const Rect & src,
                               const Rect & center, 
                               const Rect & dest, 
                               const Paint & paint)
{
    assert(canvas_);
    SkBitmap bm = ToSkBitmap(pixmap);
    SkBitmap sub;
    bm.extractSubset(&sub, ToSkIRect(src));
    canvas_->drawBitmapNine(sub, 
                            ToSkIRect(center), 
                            ToSkRect(dest), 
                            &ToSkPaint(paint));
}

void PixPainter::DrawScript(const wchar_t * text, 
                            size_t length,
                            const Point & pt,
                            const Paint & paint,
                            const Font & font)
{
    assert(canvas_);
    SkPaint skpaint = ToSkPaint(paint);
    skpaint.setTextSize(SkIntToScalar(font.size));
    auto typeface = MakeSkTypeface(font.family, font.style);
    if (typeface)
        skpaint.setTypeface(typeface)->unref();
    skpaint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
    skpaint.setAntiAlias(true);
    skpaint.setLCDRenderText(true);
    skpaint.setHinting(SkPaint::kFull_Hinting);

    SkPaint::FontMetrics fm;
    skpaint.getFontMetrics(&fm);
    SkScalar baseline = pt.y() - fm.fTop;
    canvas_->drawText(text, length * sizeof(wchar_t),
        SkIntToScalar(pt.x()), baseline, skpaint);
}


void PixPainter::DrawGradient(const Rect & rect,
    const Paint & paint,
    const LinearGradient & linear)
{
    SkPaint skpaint = ToSkPaint(paint);
    auto shader = ToSkShader(linear);
    if (shader)
        skpaint.setShader(shader)->unref();

    canvas_->drawRect(ToSkRect(rect), skpaint);
}


}
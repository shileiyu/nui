#ifndef NUI_PAINTER_H_
#define NUI_PAINTER_H_

#include <nui/nui.h>
#include "color.h"
#include "point.h"

namespace nui
{

class Rect;
class Pixmap;

struct Font
{
    enum Style
    {
        kNormal = 0,
        kBold = 0x01,
        kItalic = 0x02,

        // helpers
        kBoldItalic = 0x03
    };
    const char * family;
    Style style;
    int size;
};

struct FontMetrics 
{
    uint32_t    fFlags;       //!< Bit field to identify which values are unknown
    float       fTop;       //!< The greatest distance above the baseline for any glyph (will be <= 0)
    float       fAscent;    //!< The recommended distance above the baseline (will be <= 0)
    float       fDescent;   //!< The recommended distance below the baseline (will be >= 0)
    float       fBottom;    //!< The greatest distance below the baseline for any glyph (will be >= 0)
    float       fLeading;   //!< The recommended distance to add between lines of text (will be >= 0)
    float       fAvgCharWidth;  //!< the average character width (>= 0)
    float       fMaxCharWidth;  //!< the max character width (>= 0)
    float       fXMin;      //!< The minimum bounding box x value for all glyphs
    float       fXMax;      //!< The maximum bounding box x value for all glyphs
    float       fXHeight;   //!< The height of an 'x' in px, or 0 if no 'x' in face
    float       fCapHeight;  //!< The cap height (> 0), or 0 if cannot be determined.
    float       fUnderlineThickness; //!< underline thickness, or 0 if cannot be determined
    float       fUnderlinePosition; //!< underline position, or 0 if cannot be determined
};

void GetFontMetrics(const Font & font, FontMetrics & metrics);

int MeasureText(const Font & font, 
                const wchar_t * text, 
                size_t length, 
                Rect * bounds, 
                float scale);


int GetTextWidths(const Font & font,
                            const wchar_t * text,
                            uint32_t length,
                            uint32_t width[],
                            uint32_t& str_length);

struct Paint
{
    enum Style
    {
        kFill,
        kStroke,
        kFillAndStroke,
    };

    enum Mode
    {
        kSrcOver,
        kSrc,
    };
    Color color;
    Style style;
    int stroke_width;
    Mode mode;

    Paint()
    {
        color = Color(0xff, 0, 0, 0);
        style = kFill;
        stroke_width = 0;
        mode = kSrcOver;
    }
};


struct LinearGradient
{
    enum TileMode
    {
        kClamp,
        kRepeat,
        kMirror,
    };
    Point begin;
    Point end;
    Color * colors;
    float * positions;//{0, ..., 1}
    size_t count;
    TileMode mode;

};

class Painter
{
public:
    virtual void Save() = 0;

    virtual void Restore() = 0;

    virtual void ClipRect(const Rect & rect) = 0;

    virtual void Translate(int x, int y) = 0;

    virtual void DrawLine(const Point & pt1, const Point & pt2, const Paint & paint) = 0;

    virtual void DrawRect(const Rect & rect, const Paint & paint) = 0;

    virtual void DrawImage(Pixmap & pixmap, const Rect & src, const Rect & dest,
                           const Paint & paint) = 0;

    virtual void DrawImageNine(Pixmap & pixmap, const Rect & src,
                               const Rect & center, const Rect & dest, 
                               const Paint & paint) = 0;

    virtual void DrawScript(const wchar_t * text, 
                             size_t length, 
                             const Point & pt, 
                             const Paint & paint,
                             const Font & font) = 0;

    virtual void DrawGradient(
        const Rect & rect, 
        const Paint & paint, 
        const LinearGradient & linear) = 0;
};


}


#endif
#ifndef NUI_PIXPAINTER_H_
#define NUI_PIXPAINTER_H_

#include <nui/nui.h>
#include "painter.h"
#include "pixmap.h"

class SkCanvas;
class SkPaint;

namespace nui
{

class Pixmap;

class PixPainter : public Painter
{
    typedef std::unique_ptr<SkCanvas> SkCanvasProxy;
public:
    PixPainter(Pixmap & pm);

    ~PixPainter();

    void Save() override;

    void Restore() override;

    void ClipRect(const Rect & rect) override;

    void Translate(int x, int y) override;

    void DrawLine(const Point & pt1, 
                  const Point & pt2, 
                  const Paint & paint) override;

    void DrawRect(const Rect & rect, const Paint & paint) override;

    void DrawImage(Pixmap & pixmap, const Rect & src, const Rect & dest, 
                   const Paint & paint) override;

    void DrawImageNine(Pixmap & pixmap, const Rect & src,
                       const Rect & center, const Rect & dest, 
                       const Paint & paint) override;

    void DrawScript(const wchar_t * text, 
                    size_t length, 
                    const Point & pt, 
                    const Paint & paint,
                    const Font & font) override;

    void DrawGradient(const Rect & rect,
                      const Paint & paint,
                      const LinearGradient & linear) override;
private:
    SkCanvasProxy canvas_;
};

}

#endif
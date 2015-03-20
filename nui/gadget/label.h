#ifndef NUI_GADGET_LABEL_H_
#define NUI_GADGET_LABEL_H_

#include <nui/base/color.h>
#include <nui/base/painter.h>
#include <nui/gadget/world.h>

namespace nui
{

class Label : public Gadget
{
public:
    enum HAlign
    {
        kLeft,
        kRight,
        kCenter,
    };
    struct LineInfo
    {
        LineInfo();
        int left;
        int top;
        size_t width;
        std::wstring text;

    };
public:
    Label();

    ~Label();

    void SetText(const wchar_t * text);

    void SetLineAttribute(bool ellipsis, uint32_t spacing);

    void SetFont(const char * famliy_name, nui::Font::Style style);

    void SetTextSize(uint32_t text_size);

    void SetHAlign(HAlign align);

    void SetTextColor(const nui::Color & color);
protected:
    void OnDraw(Painter &) override;

    void OnSizeChanged() override;
private:
    void CalcCache();

    int MeasureText(const wchar_t * text, size_t length);

    int GetTextHeight();

    void GenerateDispalyTextInfos();

    void ParseSingleLineToMulti(std::wstring& str_line);

    void ParseSingleLine(std::wstring& str_line);

    void ParseLineInfoVector();

    void DrawLineVector(nui::Painter & painter);
protected:
    bool bellipsis;
    bool cache_dirty_;
    std::string famliy_name_;
    std::wstring text_;
    nui::Font::Style font_style_;
    uint32_t text_size_;
    HAlign alignment_;
    nui::Color color_;
    std::wstring display_text_;
    int left_;
    int top_;
    uint32_t spacing_;
    uint32_t text_height_;
    std::vector<LineInfo> display_text_vector;
};

}



#endif
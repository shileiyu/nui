#include "label.h"
#include <nui/base/painter.h>
#include<math.h>

namespace nui
{

static const wchar_t * kEllipsis = L"...";

Label::LineInfo::LineInfo()
    :left(0), top(0), width(0)
{}

Label::Label()
    : cache_dirty_(false), text_size_(12), alignment_(kLeft),
    famliy_name_("Microsoft YaHei"), font_style_(nui::Font::kNormal),
    left_(0), top_(0), color_(0xff000000), spacing_(0), text_height_(0),
    bellipsis(false)
{
    ;
}

Label::~Label()
{
    cache_dirty_ = true;
}

void Label::SetFont(const char * famliy_name, nui::Font::Style style)
{
    if (famliy_name)
        famliy_name_ = famliy_name;
    else
        famliy_name_.clear();

    font_style_ = style;
    cache_dirty_ = true;
    Invalidate();
}

void Label::SetText(const wchar_t * text)
{
    text_.clear();
    if (text)
        text_ = text;

    cache_dirty_ = true;
    Invalidate();
}

void Label::SetLineAttribute(bool ellipsis, uint32_t spacing)
{
    bellipsis = ellipsis;
    spacing_ = spacing;
    cache_dirty_ = true;
    Invalidate();
}

void Label::SetTextSize(uint32_t text_size)
{
    text_size_ = text_size;
    cache_dirty_ = true;
    Invalidate();
}

void Label::SetHAlign(HAlign align)
{
    alignment_ = align;
    cache_dirty_ = true;
    Invalidate();
}

void Label::SetTextColor(const nui::Color & color)
{
    color_ = color;
    Invalidate();
}

void Label::OnDraw(Painter & painter)
{
    if (text_.empty())
        return;

    CalcCache();
    DrawLineVector(painter);
}

void Label::CalcCache()
{
    if (!cache_dirty_)
        return;
    GenerateDispalyTextInfos();
    ParseLineInfoVector();
    cache_dirty_ = false;
}

/*解析超长字符串，并生成用于Draw的vector<LineInfo>数据结构*/
void Label::GenerateDispalyTextInfos()
{
    text_height_ = GetTextHeight();
    display_text_vector.clear();
    std::wstring str_line;
    auto iter = text_.begin();
    for (; iter != text_.end(); iter++)
    {
        if (*iter != '\n')
            str_line.push_back(*iter);
        if (*iter == '\n' || iter == text_.end() - 1)
        {
            if (!bellipsis)
                ParseSingleLineToMulti(str_line);
            else
                ParseSingleLine(str_line);
            str_line.clear();
        }
    }
}
/*不换行*/
void Label::ParseSingleLine(std::wstring& str_line)
{
    using namespace nui;
    LineInfo line_info;
    if (0 == str_line.size())
    {
        line_info.text = str_line;
        line_info.width = 0;
        display_text_vector.push_back(line_info);
        return;
    }
    Size size = GetSize();
    int width = size.width();
    int height = size.height();

    std::vector<uint32_t> widths;
    widths.resize(str_line.size());
    Font font;
    font.family = famliy_name_.data();
    font.style = font_style_;
    font.size = text_size_;
    uint32_t text_width = 0;
    GetTextWidths(font, str_line.data(), str_line.size(),
        &widths[0], text_width);

    uint32_t max_width = width;
    int display_width = 0;

    int ellipsis_width = MeasureText(kEllipsis, wcslen(kEllipsis));

    if (text_width > max_width)
    {
        max_width -= ellipsis_width;

        int i = 0;
        uint32_t str_width = 0;
        while (str_width <= max_width)
        {
            str_width += widths[i];
            i++;
        }
        display_text_ = str_line.substr(0, i - 1) + kEllipsis;
        display_width = str_width - widths[i - 1];
    }
    else
    {
        display_width = text_width;
        display_text_ = str_line;
    }

    line_info.text = display_text_;
    line_info.width = display_width;
    display_text_vector.push_back(line_info);
}

/*多行处理*/
void Label::ParseSingleLineToMulti(std::wstring& str_line)
{
    using namespace nui;
    if (0 == str_line.size())
    {
        LineInfo line_info;
        line_info.text = str_line;
        line_info.width = 0;
        display_text_vector.push_back(line_info);
        return;
    }
    Size size = GetSize();
    uint32_t width_label = size.width();
    std::vector<uint32_t> widths;
    widths.resize(str_line.size());
    Font font;
    font.family = famliy_name_.data();
    font.style = font_style_;
    font.size = text_size_;
    uint32_t width_text = 0;
    GetTextWidths(font, str_line.data(), str_line.size(),
        &widths[0], width_text);

    if (width_text <= width_label) //递归到了最后尾巴的一段，退出结束
    {
        LineInfo line_info;
        line_info.text = str_line;
        line_info.width = width_text;
        display_text_vector.push_back(line_info);
        return;
    }
    else    //获取一行可以显示的字符串，并对超出部分递归分析
    {
        uint32_t i, width;
        for (i = 0, width = 0; width <= width_label; i++)
            width += widths[i];

        int str_space_sum = i - 1;
        std::wstring str_space = str_line.substr(0, str_space_sum);
        LineInfo line_info;
        line_info.text = str_space;
        line_info.width = width - widths[i - 1];
        display_text_vector.push_back(line_info);
        size_t length_remian = str_line.size() - str_space_sum;
        if (0 == length_remian)
            return;
        std::wstring str_remain = str_line.substr(str_space_sum, length_remian);
        return ParseSingleLineToMulti(str_remain);
    }
}


/*计算出每一行的左上角坐标*/
void Label::ParseLineInfoVector()
{
    using namespace nui;
    Size size = GetSize();
    int width_label = size.width();
    int height_label = size.height();
    size_t line_sum = display_text_vector.size();
    size_t text_height_sum = text_height_ * line_sum + spacing_ * (line_sum - 1);
    int top = (height_label - static_cast<int>(text_height_sum)) / 2;
    auto iter = display_text_vector.begin();
    for (; iter != display_text_vector.end(); iter++)
    {
        iter->top = top;
        top = top + text_height_ + spacing_;
        if (kCenter == alignment_)
            left_ = width_label / 2 - iter->width / 2;
        else if (kRight == alignment_)
            left_ = width_label - iter->width;
        else
            left_ = 0;
        iter->left = left_;
    }
}

/*画出所有行字符串*/
void Label::DrawLineVector(nui::Painter & painter)
{
    using namespace nui;
    Paint paint;
    paint.color = color_;
    Font font;
    font.family = famliy_name_.data();
    font.style = font_style_;
    font.size = text_size_;
    Point pt;
    auto iter = display_text_vector.begin();
    for (; iter != display_text_vector.end(); iter++)
    {
        pt.Set(iter->left, iter->top);
        painter.DrawScript(iter->text.data(),
            iter->text.size(), pt, paint, font);
    }
}

int Label::MeasureText(const wchar_t * text, size_t length)
{
    using namespace nui;
    Font font;
    font.family = famliy_name_.data();
    font.style = font_style_;
    font.size = text_size_;
    return nui::MeasureText(font, text, length, nullptr, 0);
}

int Label::GetTextHeight()
{
    using namespace nui;
    Font font;
    font.family = famliy_name_.data();
    font.style = font_style_;
    font.size = text_size_;
    FontMetrics font_metrics;
    nui::GetFontMetrics(font, font_metrics);
    return static_cast<int>(round(font_metrics.fBottom - font_metrics.fTop));
}

void Label::OnSizeChanged()
{
    cache_dirty_ = true;
}


}
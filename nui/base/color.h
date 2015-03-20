#ifndef NUI_COLOR_H_
#define NUI_COLOR_H_

#include <nui/nui.h>

namespace nui
{

class Color
{
public:
    Color();

    Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

    void Set(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

    uint8_t alpha() const;

    uint8_t red() const;

    uint8_t green() const;

    uint8_t blue() const;

    uint32_t Premul() const;
private:
    uint32_t color_;
};

}
#endif
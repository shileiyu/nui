#include "color.h"

namespace nui
{
Color::Color() :color_(0)
{
    ;
}
Color::Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    Set(a, r, g, b);
}

void Color::Set(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    color_ = (a << 24) | (r << 16) | (g << 8) | (b << 0);
}
uint8_t Color::alpha() const
{
    return (((color_) >> 24) & 0xFF);
}

uint8_t Color::red() const
{
    return (((color_) >> 16) & 0xFF);
}

uint8_t Color::green() const
{
    return (((color_) >> 8) & 0xFF);
}

uint8_t Color::blue() const
{
    return (((color_) >> 0) & 0xFF);
}

uint32_t Color::Premul() const
{
    uint32_t premul;
    uint8_t a = this->alpha();
    uint8_t r = (this->red() * a) >> 8;
    uint8_t g = (this->green() * a) >> 8;
    uint8_t b = (this->blue() * a) >> 8;
    premul = (a << 24) | (r << 16) | (g << 8) | (b << 0);
    return premul;
}


}
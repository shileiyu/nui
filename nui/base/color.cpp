#include "color.h"

namespace nui
{

Color ColorSetARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

uint8_t ColorGetA(Color c)
{
    return (((c) >> 24) & 0xFF);
}

uint8_t ColorGetR(Color c)
{
    return (((c) >> 16) & 0xFF);
}

uint8_t ColorGetG(Color c)
{
    return (((c) >> 8) & 0xFF);
}

uint8_t ColorGetB(Color c)
{
    return (((c) >> 0) & 0xFF);
}

PMColor ColorPremul(Color c)
{
    PMColor premul;
    uint8_t a = ColorGetA(c);
    uint8_t r = (ColorGetR(c) * a) >> 8;
    uint8_t g = (ColorGetG(c) * a) >> 8;
    uint8_t b = (ColorGetB(c) * a) >> 8;
    premul = (a << 24) | (r << 16) | (g << 8) | (b << 0);
    return premul;
}


}
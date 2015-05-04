#ifndef NUI_BASE_COLOR_H_
#define NUI_BASE_COLOR_H_

#include <nui/nui.h>

namespace nui
{

typedef uint32_t Color;

typedef uint32_t PMColor;//预乘

Color ColorSetARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

uint8_t ColorGetA(Color c);

uint8_t ColorGetR(Color c);

uint8_t ColorGetG(Color c);

uint8_t ColorGetB(Color c);

PMColor ColorPremul(Color c);

}
#endif
#ifndef NCORE_BASE_SIZE_H_
#define NCORE_BASE_SIZE_H_

#include <nui/nui.h>

namespace nui
{


class Size
{
public:
    static Size Make(int width, int height);
public:
    Size();
    Size(int width, int height);
    ~Size();

    void operator=(const Size & size);
    bool operator==(const Size & size) const;
    bool operator!=(const Size & size) const;

    void set_width(int width);
    void set_height(int height);
    void SetSize(int width, int height);

    int width() const;
    int height() const;
private:
    int width_;
    int height_;
};


}
#endif
#ifndef NUI_PIXMAP_H_
#define NUI_PIXMAP_H_

#include <nui/nui.h>
#include "ref.h"
#include "rect.h"

namespace nui
{

class PixStorage;
typedef ScopedRef<PixStorage> PixRef;

class PixStorage : public ThreadSafeRefCount
{
public:
    struct Outline
    {
        void * bits;
        size_t width;
        size_t height;
        size_t pitch;
    };

    static PixRef Create(size_t width, size_t height, size_t pitch);

    ~PixStorage();

    bool Lock(Outline & outline);

    void Unlock();
protected:
    PixStorage(size_t width, size_t height, size_t pitch);
private:
    void * bits_;
    size_t width_;
    size_t height_;
    size_t pitch_;
};

class Color;

class Pixmap
{
public:
    static Pixmap Decode(const void * data, size_t len);

    static Pixmap Alloc(size_t width, size_t height);

    static Pixmap Alloc(const Size & size);
public:
    Pixmap();

    ~Pixmap();

    Pixmap & operator = (const Pixmap &);

    Pixmap ExtractSubset(const Rect & subset);

    size_t width() const;

    size_t height() const;

    Size GetSize() const;

    bool Lock(PixStorage::Outline & outline, Rect & subset);

    void Unlock();

    bool IsValid() const;//pixels_

    bool IsEmpty() const;// width_, height_

    bool DrawsNothing() const;

    void Erase(const Color & color);
private:
    Rect subset_;
    PixRef pixels_;
};

}

#endif
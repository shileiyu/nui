
#include "SkImageDecoder.h"
#include "SkStream.h"
#include "SkUtils.h"
#include "pixmap.h"
#include "color.h"

namespace nui
{

template<typename T>
static T * AdvanceByBytes(T * ptr, size_t offset)
{
    char * bytes = reinterpret_cast<char*>(ptr);
    return reinterpret_cast<T *>(bytes + offset);
}

PixStorage::PixStorage(size_t width, size_t height, size_t pitch)
{
    size_t bytes = pitch * height;
    bits_ = malloc(bytes);
    width_ = width;
    height_ = height;
    pitch_ = pitch;
    memset(bits_, 0, bytes);
}

PixStorage::~PixStorage()
{
    if (bits_)
        free(bits_);
}

bool PixStorage::Lock(Outline & outline)
{
    outline.bits = bits_;
    outline.width = width_;
    outline.height = height_;
    outline.pitch = pitch_;
    return true;
}

void PixStorage::Unlock()
{
    ;
}

PixRef PixStorage::Create(size_t width, size_t height, size_t pitch)
{
    PixRef pixel(new PixStorage(width, height, pitch));
    pixel->DecRef();
    return pixel;
}


Pixmap Pixmap::Decode(const void * data, size_t len)
{
    assert(data && len);

    Pixmap pm;
    if (nullptr == data || 0 == len)
        return pm;

    SkBitmap argb8888;
    SkMemoryStream ms(data, len);

    if (!SkImageDecoder::DecodeStream(
        &ms, &argb8888, SkBitmap::kARGB_8888_Config, 
        SkImageDecoder::kDecodePixels_Mode, NULL
    )) {
        return pm;
    }

    size_t width = argb8888.width();
    size_t height = argb8888.height();

    Pixmap tmp_pm = Pixmap::Alloc(width, height);
    PixStorage::Outline outline;
    Rect subset;
    if (!tmp_pm.Lock(outline, subset))
        return pm;
    size_t pitch = outline.pitch;

    if (argb8888.copyPixelsTo(outline.bits, pitch * outline.height, pitch))
        pm = tmp_pm;

    tmp_pm.Unlock();
    return pm;
}

Pixmap Pixmap::Alloc(size_t width, size_t height)
{
    return Alloc(Size(width, height));
}

Pixmap Pixmap::Alloc(const Size & size)
{
    Pixmap pm;
    auto width = size.width();
    auto height = size.height();
    pm.subset_.SetXYWH(0, 0, width, height);
    pm.pixels_ = PixStorage::Create(width, height, width * 4);
    return pm;
}

Pixmap::Pixmap()
{
    ;
}

Pixmap::~Pixmap()
{
}



Pixmap Pixmap::ExtractSubset(const Rect& subset)
{
    Pixmap sub;

    Rect real_subset = subset_;
    real_subset.Intersect(subset);
    if (!real_subset.isEmpty())
    {
        sub.subset_ = real_subset;
        sub.pixels_ = pixels_;
    }

    return sub;
}

size_t Pixmap::width() const
{
    return subset_.width();
}

size_t Pixmap::height() const
{
    return subset_.height();
}

Size Pixmap::GetSize() const
{
    return Size(width(), height());
}

Pixmap & Pixmap::operator = (const Pixmap & pm)
{
    subset_ = pm.subset_;
    pixels_ = pm.pixels_;
    return *this;
}

bool Pixmap::Lock(PixStorage::Outline & outline, Rect & subset)
{
    if(!pixels_)
        return false;

    if (!pixels_->Lock(outline))
        return false;

    subset = subset_;
    return true;    
}

void Pixmap::Unlock()
{
    if(pixels_)
        pixels_->Unlock();
}

bool Pixmap::IsValid() const
{
    if(!pixels_)
        return false;
    return pixels_.IsValid();
}

bool Pixmap::IsEmpty() const
{
    return width() == 0 || height() == 0;
}

bool Pixmap::DrawsNothing() const
{
    return !IsValid() || IsEmpty();
}

void Pixmap::Erase(const Color & color)
{
    if(!pixels_)
        return;

    PixStorage::Outline ol;

    pixels_->Lock(ol);
    uint32_t precolor = color.Premul();
    auto width = subset_.width();
    auto height = subset_.height();
    auto pitch = ol.pitch;
    size_t offset = subset_.left() * 4 + subset_.top() * pitch;
    uint32_t * bits = static_cast<uint32_t*>(ol.bits);
    bits = AdvanceByBytes(bits, offset);
    while(height)
    {
        sk_memset32(bits, precolor, width);
        bits = AdvanceByBytes(bits, pitch);
        height -= 1;
    }
    pixels_->Unlock();
}




}
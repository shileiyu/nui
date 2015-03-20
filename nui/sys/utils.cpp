#include "utils.h"
#include <nui/base/pixmap.h>

namespace nui
{

HMODULE Win32Utils::CurrentModule()
{
    static const uint32_t kQueryByAddress =
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT;

    static HINSTANCE instance = 0;

    if (!instance){
        ::GetModuleHandleEx(kQueryByAddress, (LPCTSTR)&instance, &instance);
    }    
    return instance;
}

void Win32Utils::Present(HDC hdc, Pixmap & pm)
{
    PixStorage::Outline ol;
    Rect subset;
    pm.Lock(ol, subset);

    int bm_width = ol.width;
    int bm_height = ol.height;

    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = bm_width;
    bmi.bmiHeader.biHeight = -bm_height; // top-down image
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;

    //
    // Do the SetDIBitsToDevice.
    //
    // TODO(wjmaclean):
    //       Fix this call to handle SkBitmaps that have rowBytes != width,
    //       i.e. may have padding at the end of lines. The SkASSERT below
    //       may be ignored by builds, and the only obviously safe option
    //       seems to be to copy the bitmap to a temporary (contiguous)
    //       buffer before passing to SetDIBitsToDevice().
    //SkASSERT(bitmap.width() * bitmap.bytesPerPixel() == bitmap.rowBytes());
    int iRet = SetDIBitsToDevice(
        hdc,
        0, 0,
        subset.width(), 
        subset.height(),
        subset.left(), 
        bm_height - subset.bottom(),
        0, bm_height,
        ol.bits,
        &bmi,
        DIB_RGB_COLORS);

    pm.Unlock();
}

}
#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <windows.h>

class ImageLoader {
public:
    ImageLoader() : hBitmap(NULL) {}

    ~ImageLoader() {
        DeleteObject(hBitmap);
    }

    bool load(const std::wstring& filename) {
        hBitmap = (HBITMAP)LoadImage(NULL, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        return hBitmap != NULL;
    }

    COLORREF getPixel(HDC hdc, int x, int y) const {
        return GetPixel(hdc, x, y);
    }

private:
    HBITMAP hBitmap;
};

#endif // IMAGELOADER_H

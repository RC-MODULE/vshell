//------------------------------------------------------------------------
#include <windows.h>
#include "Image.h"
//------------------------------------------------------------------------
#ifdef __DEMO__
static BYTE pbDemo[48] = 
{
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x1E, 0x1C, 0x92, 0x70,
    0x11, 0x22, 0x92, 0x88,
    0x10, 0xA0, 0x92, 0x88,
    0x10, 0xBE, 0x92, 0x88,
    0x10, 0xA2, 0xD2, 0x88,
    0x10, 0x9C, 0xBC, 0x70,
    0x11, 0x00, 0x00, 0x00,
    0x1E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
static BYTE pbBI[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2] = 
{
    sizeof(BITMAPINFOHEADER), 0, 0, 0,  // DWORD  biSize;
    32, 0, 0, 0,                        // LONG   biWidth; 
    12, 0, 0, 0,                        // LONG   biHeight;
    1, 0,                               // WORD   biPlanes; 
    1, 0,                               // WORD   biBitCount; 
    0, 0, 0, 0,                         // DWORD  biCompression;
    48, 0, 0, 0,                        // DWORD  biSizeImage; 
    0, 0, 0, 0,                         // LONG   biXPelsPerMeter; 
    0, 0, 0, 0,                         // LONG   biYPelsPerMeter; 
    0, 0, 0, 0,                         // DWORD  biClrUsed; 
    0, 0, 0, 0,                         // DWORD  biClrImportant;
    255, 128, 128, 0,                   // BGR0
    255, 255, 255, 0                    // BGR1
};
#endif
//------------------------------------------------------------------------
void C_Image::Draw(HDC hDC, HBITMAP hBmp, 
        int *pnLowOverflows, POINT *pptLowOverflows, 
        int *pnHiOverflows, POINT *pptHiOverflows)
{
    BYTE *pbData = Adjust(pnLowOverflows, pptLowOverflows, pnHiOverflows, pptHiOverflows);

    if(pbData)
    {
        SetDIBits(hDC, hBmp, 0, m_nHeight, pbData, (LPBITMAPINFO)m_pBIH, DIB_RGB_COLORS);
#ifdef __DEMO__
        SetDIBits(hDC, hBmp, 0, 12, pbDemo, (LPBITMAPINFO)pbBI, DIB_RGB_COLORS);
#endif
    }
}
//------------------------------------------------------------------------
void C_Image::StretchDraw(HDC hDC, RECT *prcDst, RECT *prcSrc, 
        int *pnLowOverflows, POINT *pptLowOverflows, 
        int *pnHiOverflows, POINT *pptHiOverflows)
{
    BYTE *pbData = Adjust(pnLowOverflows, pptLowOverflows, pnHiOverflows, pptHiOverflows);

    if(pbData)
    {
        StretchDIBits(hDC, prcDst->left, prcDst->top, prcDst->right, prcDst->bottom, 
            prcSrc->left, prcSrc->top, prcSrc->right, prcSrc->bottom, m_pbData, 
            (LPBITMAPINFO)m_pBIH, DIB_RGB_COLORS, SRCCOPY);
#ifdef __DEMO__
        StretchDIBits(hDC, 0, 0, 32, 12, 0, 0, 32, 12, pbDemo, (LPBITMAPINFO)pbBI, DIB_RGB_COLORS, SRCCOPY);
#endif
    }
}
//------------------------------------------------------------------------

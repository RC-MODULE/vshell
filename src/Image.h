//------------------------------------------------------------------------
#ifndef _IMAGE_H_
#define _IMAGE_H_ 
//------------------------------------------------------------------------
struct S_Adjust
{
    BOOL fAdjust;
    float fltMinMult;
    float fltMaxMult;
    float fltStepMult;
    float fltMult;
    int nContrast;
    int nBrightness;
};
//------------------------------------------------------------------------
class C_Image
{
protected:
    BYTE *m_pbData;
    BYTE *m_pbAdjustData;
    PBITMAPINFOHEADER m_pBIH;
    int m_nWidth;
    int m_nLineWidth;
    int m_nHeight;
    int m_nBitCount;
    int m_nType;
    int m_nBufSize;
    S_Adjust m_Adjust;
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows) = 0;

public:
    virtual ~C_Image() {}
    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal) = 0;
    virtual BOOL SetData(void *pvData) = 0;
    virtual BOOL GetData(void *pvData) = 0;
    virtual BOOL GetGrayData(void *pvData) = 0;
    virtual BOOL GetBGRData(void *pvData) = 0;
    virtual DWORD GetPixelValue(int nX, int nY) = 0;
    virtual BOOL SetPalette(RGBQUAD *prgbPal) = 0;
    virtual BOOL GetPalette(RGBQUAD *prgbPal) = 0;
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight) = 0;
    virtual BYTE *Data() = 0;

    int Width(){return m_nWidth;}
	int Height(){return m_nHeight;}
	int BitCount(){return m_nBitCount;}
    int Type(){return m_nType;}
    int BufSize() {return m_nBufSize;}
    BOOL GetAdjust(S_Adjust *pAdjust) {if(pAdjust) {CopyMemory(pAdjust, &m_Adjust, sizeof(S_Adjust)); return TRUE;} return FALSE;}
    BOOL SetAdjust(S_Adjust *pAdjust) {if(pAdjust) {CopyMemory(&m_Adjust, pAdjust, sizeof(S_Adjust)); return TRUE;} return FALSE;}
    PBITMAPINFOHEADER BitmapInfo() {return m_pBIH;}
    LPRGBQUAD Palette()
    {
        if(m_nBitCount <= 8) 
            return(LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));
        else
            return NULL;
    }
    void Draw(HDC hDC, HBITMAP hBmp, int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
    void StretchDraw(HDC hDC, RECT *prcDst, RECT *prcSrc,
            int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
    BYTE RGB2Y(COLORREF clrRGB) 
    {
        return (BYTE)(0.299 * (double)GetRValue(clrRGB) + 0.587 * (double)GetGValue(clrRGB) + 0.114 * (double)GetBValue(clrRGB) + 0.5);
    }
    BYTE RGB2Y(BYTE bR, BYTE bG, BYTE bB) 
    {
        return (BYTE)(0.299 * (double)bR + 0.587 * (double)bG + 0.114 * (double)bB + 0.5);
    }
};
//------------------------------------------------------------------------
class C_ImageRGB1 : public C_Image
{
private:
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
public:
    C_ImageRGB1();
    ~C_ImageRGB1();

    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal);
    virtual BOOL SetData(void *pvData);
    virtual BOOL GetData(void *pvData);
    virtual BOOL GetGrayData(void *pvData);
    virtual BOOL GetBGRData(void *pvData);
    virtual DWORD GetPixelValue(int nX, int nY);
    virtual BOOL SetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight);
    virtual BYTE *Data(){return m_pbData;}
};
//------------------------------------------------------------------------
class C_ImageRGB4 : public C_Image
{
private:    
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
public:
    C_ImageRGB4();
    ~C_ImageRGB4();

    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal);
    virtual BOOL SetData(void *pvData);
    virtual BOOL GetData(void *pvData);
    virtual BOOL GetGrayData(void *pvData);
    virtual BOOL GetBGRData(void *pvData);
    virtual DWORD GetPixelValue(int nX, int nY);
    virtual BOOL SetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight);
    virtual BYTE *Data(){return m_pbData;}
};
//------------------------------------------------------------------------
class C_ImageRGB8 : public C_Image
{
private:
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
public:
    C_ImageRGB8();
    ~C_ImageRGB8();

    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal);
    virtual BOOL SetData(void *pvData);
    virtual BOOL GetData(void *pvData);
    virtual BOOL GetGrayData(void *pvData);
    virtual BOOL GetBGRData(void *pvData);
    virtual DWORD GetPixelValue(int nX, int nY);
    virtual BOOL SetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight);
    virtual BYTE *Data(){return m_pbData;}
};
//------------------------------------------------------------------------
class C_ImageRGB16 : public C_Image
{
private:
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
public:
    C_ImageRGB16();
    ~C_ImageRGB16();

    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal);
    virtual BOOL SetData(void *pvData);
    virtual BOOL GetData(void *pvData);
    virtual BOOL GetGrayData(void *pvData);
    virtual BOOL GetBGRData(void *pvData);
    virtual DWORD GetPixelValue(int nX, int nY);
    virtual BOOL SetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight);
    virtual BYTE *Data(){return m_pbData;}
};
//------------------------------------------------------------------------
class C_ImageRGB24 : public C_Image
{
private:
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
public:
    C_ImageRGB24();
    ~C_ImageRGB24();

    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal);
    virtual BOOL SetData(void *pvData);
    virtual BOOL GetData(void *pvData);
    virtual BOOL GetGrayData(void *pvData);
    virtual BOOL GetBGRData(void *pvData);
    virtual DWORD GetPixelValue(int nX, int nY);
    virtual BOOL SetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight);
    virtual BYTE *Data(){return m_pbData;}
};
//------------------------------------------------------------------------
class C_ImageRGB32 : public C_Image
{
private:
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
public:
    C_ImageRGB32();
    ~C_ImageRGB32();

    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal);
    virtual BOOL SetData(void *pvData);
    virtual BOOL GetData(void *pvData);
    virtual BOOL GetGrayData(void *pvData);
    virtual BOOL GetBGRData(void *pvData);
    virtual DWORD GetPixelValue(int nX, int nY);
    virtual BOOL SetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight);
    virtual BYTE *Data(){return m_pbData;}
};
//------------------------------------------------------------------------
class C_ImageRGB8_8 : public C_Image
{
private:
    char *m_pcData;
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
public:
    C_ImageRGB8_8();
    ~C_ImageRGB8_8();

    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal);
    virtual BOOL SetData(void *pvData);
    virtual BOOL GetData(void *pvData);
    virtual BOOL GetGrayData(void *pvData);
    virtual BOOL GetBGRData(void *pvData);
    virtual DWORD GetPixelValue(int nX, int nY);
    virtual BOOL SetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight);
    virtual BYTE *Data(){return (BYTE*)m_pcData;}
};
//------------------------------------------------------------------------
class C_ImageRGB8_16 : public C_Image
{
private:
    short *m_pshData;
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
public:
    C_ImageRGB8_16();
    ~C_ImageRGB8_16();

    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal);
    virtual BOOL SetData(void *pvData);
    virtual BOOL GetData(void *pvData);
    virtual BOOL GetGrayData(void *pvData);
    virtual BOOL GetBGRData(void *pvData);
    virtual DWORD GetPixelValue(int nX, int nY);
    virtual BOOL SetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight);
    virtual BYTE *Data(){return (BYTE*)m_pshData;}
};
//------------------------------------------------------------------------
class C_ImageRGB8_32 : public C_Image
{
private:
    int *m_pnData;
    virtual BYTE *Adjust(int *pnLowOverflows, POINT *pptLowOverflows, 
            int *pnHiOverflows, POINT *pptHiOverflows);
public:
    C_ImageRGB8_32();
    ~C_ImageRGB8_32();

    virtual BOOL Init(int nWidth, int nHeight, LPRGBQUAD prgbPal);
    virtual BOOL SetData(void *pvData);
    virtual BOOL GetData(void *pvData);
    virtual BOOL GetGrayData(void *pvData);
    virtual BOOL GetBGRData(void *pvData);
    virtual DWORD GetPixelValue(int nX, int nY);
    virtual BOOL SetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetPalette(RGBQUAD *prgbPal);
    virtual BOOL GetHistogram(int *pnHistogram, int nHistoHeight);
    virtual BYTE *Data(){return (BYTE*)m_pnData;}
};
//------------------------------------------------------------------------
#endif  // _IMAGE_H_





















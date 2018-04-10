//------------------------------------------------------------------------
#ifndef _BITMAPEX_H_
#define _BITMAPEX_H_ 
//------------------------------------------------------------------------
#include "Image.h"
#include "VecGraph.h"
#include "AVIWriter.h"
//------------------------------------------------------------------------
struct S_Overflows
{
    int nOverflows;
    POINT *pptOverflows;
};
//------------------------------------------------------------------------
class C_BitmapEx
{
private:
    HWND m_hWnd;
    C_Image *m_pImage;
    
    int m_nScale;
    int m_nXScale;
    int m_nYScale;
    C_AVIWriter *m_pAVIW;
    BITMAPFILEHEADER m_BFH;
    BITMAPINFOHEADER m_BIH;
    
public:
    C_VecGraph m_VG;
    BOOL m_fShowOverflows;
    BOOL m_fRaster;
    BOOL m_fShowUserGraphic;
    S_Overflows m_LowOverflows;
    S_Overflows m_HiOverflows;

    C_BitmapEx();
    ~C_BitmapEx();

    BOOL Init(HWND hWnd, int nWidth, int nHeight, int nType, LPRGBQUAD prgbPal);
	BOOL Draw();
	int Width(){if(m_pImage) return m_pImage->Width(); return 0;}
	int Height(){if(m_pImage) return m_pImage->Height(); return 0;}
	int BitCount(){if(m_pImage) return m_pImage->BitCount(); return 0;}
    int Type(){if(m_pImage) return m_pImage->Type(); return 0;}
    int BufSize(){if(m_pImage) return m_pImage->BufSize(); return 0;}
    int Scale(){return m_nScale;}
    BOOL SetPalette(RGBQUAD *prgbPal){if(m_pImage) return m_pImage->SetPalette(prgbPal); return FALSE;}
    BOOL GetPalette(RGBQUAD *prgbPal){if(m_pImage) return m_pImage->GetPalette(prgbPal); return FALSE;}
    BOOL SetData(void *pvData){if(m_pImage) return m_pImage->SetData(pvData); return FALSE;}
    BOOL GetData(void *pvData){if(m_pImage) return m_pImage->GetData(pvData); return FALSE;}
    BOOL GetGrayData(void *pvData){if(m_pImage) return m_pImage->GetGrayData(pvData); return FALSE;}
    BOOL GetBGRData(void *pvData){if(m_pImage) return m_pImage->GetBGRData(pvData); return FALSE;}
    DWORD GetPixelValue(int nX, int nY) {if(m_pImage) return m_pImage->GetPixelValue(nX, nY); return 0;}
    BOOL GetHistogram(int *pnHistogram, int nHistoHeight)
        {if(m_pImage) return m_pImage->GetHistogram(pnHistogram, nHistoHeight); return FALSE;}
    BOOL SetVectors(int nBlockSize, int *pnVectors, COLORREF VectorsColor);
    BOOL SetVectors(int nBlockSize, float *pfltVectors, COLORREF VectorsColor);
    BOOL SaveImage(char *szFileName);
    BOOL CreateAVIWriter(char *szFileName);
    BOOL DeleteAVIWriter();
    BYTE *Data(){if(m_pImage) return m_pImage->Data(); return NULL;}
    BOOL SetAdjust(S_Adjust *pAdjust) {if(m_pImage) return m_pImage->SetAdjust(pAdjust); return FALSE;}
    BOOL GetAdjust(S_Adjust *pAdjust) {if(m_pImage) return m_pImage->GetAdjust(pAdjust); return FALSE;}

    void SetScale(int nXMouse, int nYMouse, BOOL fMin);
    void MoveImage(int nXDelta, int nYDelta);
    void MoveImage();
    void GetScaleMouseFromMouse(int nXMouse, int nYMouse, int *pnXScaleMouse, int *pnYScaleMouse);
    void GetMouseFromScaleMouse(int nXScaleMouse, int nYScaleMouse, int *pnXMouse, int *pnYMouse);
    void GetGridCoordinates(int *pnXGrid, int *pnYGrid);
    void GetVectorsInBlock(float *pfltXVector, float *pfltYVector);
	
    BOOL SetPixel(int nX, int nY, COLORREF crColor)
    {
        return m_VG.Pixel(nX, nY, crColor);
    }
    BOOL Line(int nX1, int nY1, int nX2, int nY2, COLORREF crColor, int nDashDot)
    {
        return m_VG.Line(nX1, nY1, nX2, nY2, crColor, nDashDot);
    }
    BOOL Polyline(POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot)
    {
        return m_VG.Polyline(pPoints, nPoints, crColor, nDashDot);
    }
    BOOL PolyBezier(POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot)
    {
        return m_VG.PolyBezier(pPoints, nPoints, crColor, nDashDot);
    }
    BOOL Arc(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
            COLORREF crColor, int nDashDot)
    {
        return m_VG.Arc(nLeftRect, nTopRect, nRightRect, nBottomRect,
                    nXStartArc, nYStartArc, nXEndArc, nYEndArc, crColor, nDashDot);
    }
    BOOL Rectangle(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Rectangle(nLeftRect, nTopRect, nRightRect, nBottomRect,
                    crBoundColor, crFillColor, nDashDot);
    }
    BOOL Polygon(POINT *pptPoints, int nPoints, 
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Polygon(pptPoints, nPoints, crBoundColor, crFillColor, nDashDot);
    }
    BOOL Ellipse(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Ellipse(nLeftRect, nTopRect, nRightRect, nBottomRect,
                    crBoundColor, crFillColor, nDashDot);
    }
    BOOL Chord(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXRadial1, int nYRadial1,
            int nXRadial2, int nYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Chord(nLeftRect, nTopRect, nRightRect, nBottomRect,
                    nXRadial1, nYRadial1, nXRadial2, nYRadial2, crBoundColor, crFillColor, nDashDot);
    }
    BOOL Pie(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXRadial1, int nYRadial1,
            int nXRadial2, int nYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Pie(nLeftRect, nTopRect, nRightRect, nBottomRect,
                    nXRadial1, nYRadial1, nXRadial2, nYRadial2, crBoundColor, crFillColor, nDashDot);
    }
    BOOL SetPixel(float fltX, float fltY, COLORREF crColor)
    {
        return m_VG.Pixel(fltX, fltY, crColor);
    }
	BOOL Line(float fltX1, float fltY1, float fltX2, float fltY2, COLORREF crColor, int nDashDot)
    {
        return m_VG.Line(fltX1, fltY1, fltX2, fltY2, crColor, nDashDot);
    }
    BOOL Polyline(float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot)
    {
        return m_VG.Polyline(pfltPoints, nPoints, crColor, nDashDot);
    }
    BOOL PolyBezier(float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot)
    {
        return m_VG.PolyBezier(pfltPoints, nPoints, crColor, nDashDot);
    }
    BOOL Arc(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            float fltXStartArc, float fltYStartArc, float fltXEndArc, float fltYEndArc,
            COLORREF crColor, int nDashDot)
    {
        return m_VG.Arc(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
                fltXStartArc, fltYStartArc, fltXEndArc, fltYEndArc, crColor, nDashDot);
    }
    BOOL Rectangle(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Rectangle(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
                crBoundColor, crFillColor, nDashDot);
    }
    BOOL Polygon(float *pfltPoints, int nPoints, COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Polygon(pfltPoints, nPoints, crBoundColor, crFillColor, nDashDot);
    }
    BOOL Ellipse(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Ellipse(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
                crBoundColor, crFillColor, nDashDot);
    }
    BOOL Chord(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            float fltXRadial1, float fltYRadial1,
            float fltXRadial2, float fltYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Chord(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            fltXRadial1, fltYRadial1, fltXRadial2, fltYRadial2, crBoundColor, crFillColor, nDashDot);
    }
    BOOL Pie(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            float fltXRadial1, float fltYRadial1,
            float fltXRadial2, float fltYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
    {
        return m_VG.Pie(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            fltXRadial1, fltYRadial1, fltXRadial2, fltYRadial2, crBoundColor, crFillColor, nDashDot);
    }
    BOOL TextOut(char *szText, int nX, int nY, int nSize, COLORREF clrColor)
    {
        return m_VG.TextOut(szText, nX, nY, nSize, clrColor);
    }

    BOOL CorrectSignedImage();
};
//------------------------------------------------------------------------
#endif  // _BITMAPEX_H_
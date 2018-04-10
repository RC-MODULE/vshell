//-------------------------------------------------------------------
#ifndef _ENHMETAFILE_H_
#define _ENHMETAFILE_H_
//-------------------------------------------------------------------
#include "VecGraph.h"
//-------------------------------------------------------------------
class C_EnhMetaFile
{
private:
    HDC m_hDC;
    RECT m_rcView;
    RECT m_rcMF;
    double m_dPixInMmX; 
    double m_dPixInMmY;
    CRITICAL_SECTION m_CS;
    HENHMETAFILE m_hMF;
    C_VecGraph m_VG;
public:
    C_EnhMetaFile();
    ~C_EnhMetaFile();

    BOOL Init(int nWidth, int nHeight);
    void Flush();
    //void Draw(HDC hDC);
    void StretchDraw(HDC hDC, RECT *prcDst, RECT *prcSrc);
    void Draw(HDC hDC);
    
    void Grid(int nStepX, int nStepY, COLORREF crColor);
    void Vectors(int nBlockSize, int *pnVectors, COLORREF crColor);
    void Vectors(int nBlockSize, int *pnVectors, COLORREF *pcrColor);
    
	BOOL SetPixel(int nX, int nY, COLORREF crColor);
    BOOL Line(int nX1, int nY1, int nX2, int nY2, COLORREF crColor);
    BOOL PolyLine(POINT *pPoints, int nPoints, COLORREF crColor);
    BOOL PolyBezier(POINT *pPoints, int nPoints, COLORREF crColor);
    BOOL Arc(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
            COLORREF crColor);
    
    BOOL Rectangle(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor);
    BOOL Polygon(POINT *pptPoints, int nPoints, 
            COLORREF crBoundColor, COLORREF crFillColor);
    BOOL Ellipse(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor);
    BOOL Chord(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXRadial1, int nYRadial1,
            int nXRadial2, int nYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor);
    BOOL Pie(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXRadial1, int nYRadial1,
            int nXRadial2, int nYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor);
};
//-------------------------------------------------------------------
#endif  // _ENHMETAFILE_H_
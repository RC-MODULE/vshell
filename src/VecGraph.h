    //--------------------------------------------------------------------
#ifndef _VECGRAPH_H_
#define _VECGRAPH_H_
    //--------------------------------------------------------------------
#include "GraphElem.h"
    //--------------------------------------------------------------------
struct S_Grid
{
    BOOL fShow;
    int nX;
    int nY;
    COLORREF crColor;
};
    //--------------------------------------------------------------------
struct S_Vectors
{
    BOOL fShow;
    int nBlockSize;
    COLORREF crColor;
    int *pnVectors;
    float *pfltVectors;
};
    //--------------------------------------------------------------------
struct S_VGElem
{
    C_GraphElem *pElem;
    S_VGElem *pNext;
};
    //--------------------------------------------------------------------
class C_VecGraph
{
private:
    S_VGElem *m_pFirst[2];
    S_VGElem *m_pLast[2];
    int m_nCurList;
    int m_nWidth;
    int m_nHeight;
    CRITICAL_SECTION m_csList;
    CRITICAL_SECTION m_csVectors;

    void Add(int nType, int *pnParams);
    void Clear(int nCurList);
public:
    S_Grid m_Grid;
    S_Vectors m_Vectors;

    C_VecGraph();
    ~C_VecGraph();

    BOOL Init(int nWidth, int nHeight);
    void DrawGrid(HDC hDC, int nX = 0, int nY = 0, int nW = 0, int nH = 0, int nK = 0);
    void DrawVectors(HDC hDC, int nX = 0, int nY = 0, int nW = 0, int nH = 0, int nK = 0);
    void DrawList(HDC hDC, int nX = 0, int nY = 0, int nW = 0, int nH = 0, int nK = 0);

    void Grid(int nStepX, int nStepY, COLORREF crColor);
    void Vectors(int nBlockSize, int *pnVectors, COLORREF crColor);
    void Vectors(int nBlockSize, float *pfltVectors, COLORREF crColor);
 
    BOOL Pixel(int nX, int nY, COLORREF crColor);
    BOOL Line(int nX1, int nY1, int nX2, int nY2, COLORREF crColor, int nDashDot);
    BOOL Polyline(POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot);
    BOOL PolyBezier(POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot);
    BOOL Arc(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
            COLORREF crColor, int nDashDot);
    
    BOOL Rectangle(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Polygon(POINT *pPoints, int nPoints, 
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Ellipse(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Chord(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXRadial1, int nYRadial1,
            int nXRadial2, int nYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Pie(int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXRadial1, int nYRadial1,
            int nXRadial2, int nYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);

    BOOL Pixel(float fltX, float fltY, COLORREF crColor);
	BOOL Line(float fltX1, float fltY1, float fltX2, float fltY2, COLORREF crColor, int nDashDot);
    BOOL Polyline(float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot);
    BOOL PolyBezier(float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot);
    BOOL Arc(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            float fltXStartArc, float fltYStartArc, float fltXEndArc, float fltYEndArc,
            COLORREF crColor, int nDashDot);
    BOOL Rectangle(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Polygon(float *pfltPoints, int nPoints, COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Ellipse(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Chord(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            float fltXRadial1, float fltYRadial1,
            float fltXRadial2, float fltYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Pie(float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            float fltXRadial1, float fltYRadial1,
            float fltXRadial2, float fltYRadial2,
            COLORREF crBoundColor, COLORREF crFillColo, int nDashDotr);

    BOOL TextOut(char *szText, int nX, int nY, int nSize, COLORREF clrColor);

    void Swap();
};
    //--------------------------------------------------------------------
#endif  // _VECGRAPH_H_
    //--------------------------------------------------------------------
#include <windows.h>
#include "VecGraph.h"
    //--------------------------------------------------------------------
static const int GRID_X = 8;
static const int GRID_Y = 8;
static const COLORREF GRID_COLOR = RGB(128, 128, 128);
static const COLORREF VECT_COLOR = RGB(0, 255, 0);
    //--------------------------------------------------------------------
C_VecGraph::C_VecGraph()
{
    m_pFirst[0] = NULL;
    m_pFirst[1] = NULL;
    m_pLast[0] = NULL;
    m_pLast[1] = NULL;
    m_nCurList = 0;
    m_nWidth = 0;
    m_nHeight = 0;
    m_Grid.nX = GRID_X;
    m_Grid.nY = GRID_Y;
    m_Grid.crColor = GRID_COLOR;
    m_Grid.fShow = FALSE;
    m_Vectors.nBlockSize = 0;
    m_Vectors.pnVectors = NULL;
    m_Vectors.pfltVectors = NULL;
    m_Vectors.fShow = FALSE;
    m_Vectors.crColor = VECT_COLOR;
    InitializeCriticalSection(&m_csList);
    InitializeCriticalSection(&m_csVectors);
}
    //--------------------------------------------------------------------
C_VecGraph::~C_VecGraph()
{
    Clear(0);
    Clear(1);
    delete [] m_Vectors.pnVectors;
    delete [] m_Vectors.pfltVectors;
    DeleteCriticalSection(&m_csList);
    DeleteCriticalSection(&m_csVectors);
}
    //--------------------------------------------------------------------
void C_VecGraph::Add(int nType, int *pnParams)
{
    S_VGElem *pVGElem = new S_VGElem;
    if(NULL == pVGElem)
    {
        return;
    }
    pVGElem->pElem = CreateGraphElem(nType);
    if(NULL == pVGElem->pElem)
    {
        return;
    }
    memcpy(pVGElem->pElem->pnParams, pnParams, GRAPH_MAX_PARAMS * sizeof(int));
    if(nType == GRAPH_POLYGON || nType == GRAPH_POLYLINE || nType == GRAPH_POLYBEZIER)
    {
        pVGElem->pElem->pnParams[0] = (int)(new int[pnParams[1] * 2]);
        memcpy((void*)(pVGElem->pElem->pnParams[0]), (void*)pnParams[0], 
                pnParams[1] * 2 * sizeof(int));
    }
    else if(nType == GRAPH_POLYGON_F || nType == GRAPH_POLYLINE_F || nType == GRAPH_POLYBEZIER_F)
    {
        pVGElem->pElem->pnParams[0] = (int)(new float[pnParams[1] * 2]);
        memcpy((void*)(pVGElem->pElem->pnParams[0]), (void*)pnParams[0], 
                pnParams[1] * 2 * sizeof(float));
    }
    else if(nType == GRAPH_TEXTOUT)
    {
        pVGElem->pElem->pnParams[0] = (int)(new char[lstrlen((char*)pnParams[0]) + 1]);
        lstrcpy((char*)(pVGElem->pElem->pnParams[0]), (char*)pnParams[0]);
    }

    pVGElem->pNext = NULL;

    EnterCriticalSection(&m_csList);
    if(!m_pFirst[m_nCurList])
    {
        m_pFirst[m_nCurList] = pVGElem;
    }
    else
    {
        m_pLast[m_nCurList]->pNext = pVGElem;
    }
    m_pLast[m_nCurList] = pVGElem;
    LeaveCriticalSection(&m_csList);
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Init(int nWidth, int nHeight)
{
    Clear(0);
    Clear(1);
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    return TRUE;
}
    //--------------------------------------------------------------------
void C_VecGraph::Grid(int nStepX, int nStepY, COLORREF crColor)
{
    m_Grid.nX = nStepX;
    m_Grid.nY = nStepY;
    m_Grid.crColor = crColor;
}
    //--------------------------------------------------------------------
void C_VecGraph::Vectors(int nBlockSize, int *pnVectors, COLORREF crColor)
{
    if(nBlockSize == 0 || !pnVectors)
    {
        return;
    }
    int nVectors = (m_nWidth * m_nHeight) / (nBlockSize * nBlockSize);
    EnterCriticalSection(&m_csVectors);
    m_Vectors.nBlockSize = nBlockSize;
    m_Vectors.crColor = crColor;
    if(!m_Vectors.pnVectors || ((sizeof(m_Vectors.pnVectors) >> 3) != nVectors))
    {
        delete [] m_Vectors.pfltVectors;
        m_Vectors.pfltVectors = NULL;
        delete [] m_Vectors.pnVectors;
        m_Vectors.pnVectors = new int[nVectors * 2];        
    }
    memcpy(m_Vectors.pnVectors, pnVectors, nVectors << 3);
    LeaveCriticalSection(&m_csVectors);
}   
    //--------------------------------------------------------------------
void C_VecGraph::Vectors(int nBlockSize, float *pfltVectors, COLORREF crColor)
{
    if(nBlockSize == 0 || !pfltVectors)
    {
        return;
    }
    int nVectors = (m_nWidth * m_nHeight) / (nBlockSize * nBlockSize);
    EnterCriticalSection(&m_csVectors);
    m_Vectors.nBlockSize = nBlockSize;
    m_Vectors.crColor = crColor;
    if(!m_Vectors.pnVectors || ((sizeof(m_Vectors.pnVectors) >> 3) != nVectors))
    {
        delete [] m_Vectors.pnVectors;
        m_Vectors.pnVectors = NULL;
        delete [] m_Vectors.pfltVectors;
        m_Vectors.pfltVectors = new float[nVectors * 2];        
    }
    memcpy(m_Vectors.pfltVectors, pfltVectors, nVectors << 3);
    LeaveCriticalSection(&m_csVectors);
}   
    //--------------------------------------------------------------------
BOOL C_VecGraph::Pixel(int nX, int nY, COLORREF crColor)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = nX;
    pnParams[1] = nY;
    pnParams[2] = crColor;
    Add(GRAPH_PIXEL, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Line(int nX1, int nY1, int nX2, int nY2, COLORREF crColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = nX1;
    pnParams[1] = nY1;
    pnParams[2] = nX2;
    pnParams[3] = nY2;
    pnParams[4] = crColor;
	pnParams[5] = nDashDot;
    Add(GRAPH_LINE, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Polyline(POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = (int)pPoints;
    pnParams[1] = nPoints;
    pnParams[2] = crColor;
	pnParams[3] = nDashDot;
    Add(GRAPH_POLYLINE, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::PolyBezier(POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = (int)pPoints;
    pnParams[1] = nPoints;
    pnParams[2] = crColor;
	pnParams[3] = nDashDot;
    Add(GRAPH_POLYBEZIER, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Arc(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect,
            int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
            COLORREF crColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = nLeftRect;
    pnParams[1] = nTopRect;
    pnParams[2] = nRightRect;
    pnParams[3] = nBottomRect;
    pnParams[4] = nXStartArc;
    pnParams[5] = nYStartArc;
    pnParams[6] = nXEndArc;
    pnParams[7] = nYEndArc;
    pnParams[8] = crColor;
	pnParams[9] = nDashDot;
    Add(GRAPH_ARC, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Rectangle(int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = nLeftRect;
    pnParams[1] = nTopRect;
    pnParams[2] = nRightRect;
    pnParams[3] = nBottomRect;
    pnParams[4] = crBoundColor;
    pnParams[5] = crFillColor;
	pnParams[6] = nDashDot;
    Add(GRAPH_RECTANGLE, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Polygon(POINT *pPoints, int nPoints, 
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = (int)pPoints;
    pnParams[1] = nPoints;
    pnParams[2] = crBoundColor;
    pnParams[3] = crFillColor;
	pnParams[4] = nDashDot;
    Add(GRAPH_POLYGON, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Ellipse(int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = nLeftRect;
    pnParams[1] = nTopRect;
    pnParams[2] = nRightRect;
    pnParams[3] = nBottomRect;
    pnParams[4] = crBoundColor;
    pnParams[5] = crFillColor;
	pnParams[6] = nDashDot;
    Add(GRAPH_ELLIPSE, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Chord(int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = nLeftRect;
    pnParams[1] = nTopRect;
    pnParams[2] = nRightRect;
    pnParams[3] = nBottomRect;
    pnParams[4] = nXRadial1;
    pnParams[5] = nYRadial1;
    pnParams[6] = nXRadial2;
    pnParams[7] = nYRadial2;
    pnParams[8] = crBoundColor;
    pnParams[9] = crFillColor;
	pnParams[10] = nDashDot;
    Add(GRAPH_CHORD, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Pie(int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = nLeftRect;
    pnParams[1] = nTopRect;
    pnParams[2] = nRightRect;
    pnParams[3] = nBottomRect;
    pnParams[4] = nXRadial1;
    pnParams[5] = nYRadial1;
    pnParams[6] = nXRadial2;
    pnParams[7] = nYRadial2;
    pnParams[8] = crBoundColor;
    pnParams[9] = crFillColor;
	pnParams[10] = nDashDot;
    Add(GRAPH_PIE, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Pixel(float fltX, float fltY, COLORREF crColor)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = *(int*)&fltX;
    pnParams[1] = *(int*)&fltY;
    pnParams[2] = crColor;
    Add(GRAPH_PIXEL_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Line(float fltX1, float fltY1, float fltX2, float fltY2, COLORREF crColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = *(int*)&fltX1;
    pnParams[1] = *(int*)&fltY1;
    pnParams[2] = *(int*)&fltX2;
    pnParams[3] = *(int*)&fltY2;
    pnParams[4] = crColor;
	pnParams[5] = nDashDot;
    Add(GRAPH_LINE_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Polyline(float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = (int)pfltPoints;
    pnParams[1] = nPoints;
    pnParams[2] = crColor;
	pnParams[3] = nDashDot;
    Add(GRAPH_POLYLINE_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::PolyBezier(float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = (int)pfltPoints;
    pnParams[1] = nPoints;
    pnParams[2] = crColor;
	pnParams[3] = nDashDot;
    Add(GRAPH_POLYBEZIER_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Arc(float fltLeftRect, float fltTopRect, float fltRightRect, float fltBottomRect,
            float fltXStartArc, float fltYStartArc, float fltXEndArc, float fltYEndArc,
            COLORREF crColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = *(int*)&fltLeftRect;
    pnParams[1] = *(int*)&fltTopRect;
    pnParams[2] = *(int*)&fltRightRect;
    pnParams[3] = *(int*)&fltBottomRect;
    pnParams[4] = *(int*)&fltXStartArc;
    pnParams[5] = *(int*)&fltYStartArc;
    pnParams[6] = *(int*)&fltXEndArc;
    pnParams[7] = *(int*)&fltYEndArc;
    pnParams[8] = crColor;
	pnParams[9] = nDashDot;
    Add(GRAPH_ARC_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Rectangle(float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = *(int*)&fltLeftRect;
    pnParams[1] = *(int*)&fltTopRect;
    pnParams[2] = *(int*)&fltRightRect;
    pnParams[3] = *(int*)&fltBottomRect;
    pnParams[4] = crBoundColor;
    pnParams[5] = crFillColor;
	pnParams[6] = nDashDot;
    Add(GRAPH_RECTANGLE_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Polygon(float *pfltPoints, int nPoints, COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = (int)pfltPoints;
    pnParams[1] = nPoints;
    pnParams[2] = crBoundColor;
    pnParams[3] = crFillColor;
	pnParams[4] = nDashDot;
    Add(GRAPH_POLYGON_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Ellipse(float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = *(int*)&fltLeftRect;
    pnParams[1] = *(int*)&fltTopRect;
    pnParams[2] = *(int*)&fltRightRect;
    pnParams[3] = *(int*)&fltBottomRect;
    pnParams[4] = crBoundColor;
    pnParams[5] = crFillColor;
	pnParams[6] = nDashDot;
    Add(GRAPH_ELLIPSE_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Chord(float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXRadial1, float fltYRadial1,
        float fltXRadial2, float fltYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = *(int*)&fltLeftRect;
    pnParams[1] = *(int*)&fltTopRect;
    pnParams[2] = *(int*)&fltRightRect;
    pnParams[3] = *(int*)&fltBottomRect;
    pnParams[4] = *(int*)&fltXRadial1;
    pnParams[5] = *(int*)&fltYRadial1;
    pnParams[6] = *(int*)&fltXRadial2;
    pnParams[7] = *(int*)&fltYRadial2;
    pnParams[8] = crBoundColor;
    pnParams[9] = crFillColor;
	pnParams[10] = nDashDot;
    Add(GRAPH_CHORD_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::Pie(float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXRadial1, float fltYRadial1,
        float fltXRadial2, float fltYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = *(int*)&fltLeftRect;
    pnParams[1] = *(int*)&fltTopRect;
    pnParams[2] = *(int*)&fltRightRect;
    pnParams[3] = *(int*)&fltBottomRect;
    pnParams[4] = *(int*)&fltXRadial1;
    pnParams[5] = *(int*)&fltYRadial1;
    pnParams[6] = *(int*)&fltXRadial2;
    pnParams[7] = *(int*)&fltYRadial2;
    pnParams[8] = crBoundColor;
    pnParams[9] = crFillColor;
	pnParams[10] = nDashDot;
    Add(GRAPH_PIE_F, pnParams);
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_VecGraph::TextOut(char *szText, int nX, int nY, int nSize, COLORREF clrColor)
{
    int pnParams[GRAPH_MAX_PARAMS];
    pnParams[0] = (int)szText;
    pnParams[1] = nX;
    pnParams[2] = nY;
    pnParams[3] = nSize;
    pnParams[4] = clrColor;
    Add(GRAPH_TEXTOUT, pnParams);
    return TRUE;    
}
    //--------------------------------------------------------------------
void C_VecGraph::Clear(int nCurList)
{
    EnterCriticalSection(&m_csList);
    while(m_pFirst[nCurList])
    {
        S_VGElem *pTmp = m_pFirst[nCurList];
        m_pFirst[nCurList] = m_pFirst[nCurList]->pNext;
        delete pTmp->pElem;
        delete pTmp;
    }
    m_pFirst[nCurList] = NULL;
    m_pLast[nCurList] = NULL;
    LeaveCriticalSection(&m_csList);
}
    //--------------------------------------------------------------------
void C_VecGraph::Swap()
{
    Clear(m_nCurList = 1 - m_nCurList);
}
    //--------------------------------------------------------------------
void C_VecGraph::DrawGrid(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!m_Grid.fShow || !m_Grid.nX || !m_Grid.nY)
    {
        return;
    }

    if(0 == nK)
    {
        nX = 0;
        nY = 0;
        nW = m_nWidth;
        nH = m_nHeight;
    }

    int nStepX = m_Grid.nX << nK;
    int nStepY = m_Grid.nY << nK;
    int nStartX = (m_Grid.nX - (nX % m_Grid.nX)) << nK;
    int nStartY = (m_Grid.nY - (nY % m_Grid.nY)) << nK;
    int nEndX = nW << nK;
    int nEndY = nH << nK;

    HPEN hPen, hOldPen;
    int i;

    hPen = CreatePen(PS_SOLID, 1, m_Grid.crColor);
    hOldPen = (HPEN)SelectObject(hDC, hPen);

    for(i=nStartX; i<nEndX; i+=nStepX)
    {
        MoveToEx(hDC, i, 0, NULL);
	    LineTo(hDC, i, nEndY);
    }
    for(i=nStartY; i<nEndY; i+=nStepY)
    {
        MoveToEx(hDC, 0, i, NULL);
	    LineTo(hDC, nEndX, i);
    }
    
    SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}
    //--------------------------------------------------------------------
void C_VecGraph::DrawVectors(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!m_Vectors.fShow || (!m_Vectors.pnVectors && !m_Vectors.pfltVectors) || !m_Vectors.nBlockSize)
    {
        return;
    }

    EnterCriticalSection(&m_csVectors);
    if(0 == nK)
    {
        nX = 0;
        nY = 0;
        nW = m_nWidth;
        nH = m_nHeight;
    }

    int nStep = m_Vectors.nBlockSize << nK;
    int nStartX = (m_Vectors.nBlockSize - (nX % m_Vectors.nBlockSize) - 
            (m_Vectors.nBlockSize >> 1)) << nK;
    int nStartY = (m_Vectors.nBlockSize - (nY % m_Vectors.nBlockSize) - 
            (m_Vectors.nBlockSize >> 1)) << nK;
    int nBW = m_nWidth / m_Vectors.nBlockSize;
   // int nBH = m_nHeight / m_Vectors.nBlockSize;
    int nVecInd = ((int)(nX / m_Vectors.nBlockSize) + 
                (int)(nY / m_Vectors.nBlockSize) * nBW) << 1;
    if(nStartX < 0)
    {
        nStartX += nStep;
        nVecInd += 2;
    }
    if(nStartY < 0)
    {
        nStartY += nStep;
        nVecInd += (nBW << 1);
    }
    int nEndX = nW << nK;
    int nEndY = nH << nK;

    HPEN hPen, hOldPen;
    int i, j, k;
    //int nEHS = 2 << nK;

    hPen = CreatePen(PS_SOLID, 1, m_Vectors.crColor);
    hOldPen = (HPEN)SelectObject(hDC, hPen);
   
    if(m_Vectors.pnVectors)
    {
        for(i=nStartY; i<nEndY; i+=nStep)
        {
            for(k=0, j=nStartX; j<nEndX; j+=nStep, k+=2)
            {
                if(m_Vectors.pnVectors[nVecInd+k] == 0 && m_Vectors.pnVectors[nVecInd+k+1] == 0)
                {
                    SetPixel(hDC, j, i, (~m_Vectors.crColor) & (0xFFFFFF));
                }
                else if(m_Vectors.pnVectors[nVecInd+k] != 0x80000000)
                {
                    MoveToEx(hDC, j, i, NULL);
	                LineTo(hDC, j + (m_Vectors.pnVectors[nVecInd+k] << nK), 
                            i + (m_Vectors.pnVectors[nVecInd+k+1] << nK));
                }
            }
            nVecInd += (nBW << 1);
        }   
    }
    else if(m_Vectors.pfltVectors)
    {
        float fltK = (float)(1 << nK);
        for(i=nStartY; i<nEndY; i+=nStep)
        {
            for(k=0, j=nStartX; j<nEndX; j+=nStep, k+=2)
            {
                if(m_Vectors.pfltVectors[nVecInd+k] == 0.0f && m_Vectors.pfltVectors[nVecInd+k+1] == 0.0f)
                {
                    SetPixel(hDC, j, i, (~m_Vectors.crColor) & (0xFFFFFF));
                }
                else if(m_Vectors.pfltVectors[nVecInd+k] != (float)0x80000000)
                {
                    MoveToEx(hDC, j, i, NULL);
	                LineTo(hDC, j + (int)(m_Vectors.pfltVectors[nVecInd+k] * fltK), 
                            i + (int)(m_Vectors.pfltVectors[nVecInd+k+1] * fltK));
                }
            }
            nVecInd += (nBW << 1);
        }   
    }
    LeaveCriticalSection(&m_csVectors);

    SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}
    //--------------------------------------------------------------------
void C_VecGraph::DrawList(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!m_pFirst[1 - m_nCurList])
    {
        return;
    }
    if(0 == nK)
    {
        nX = 0;
        nY = 0;
        nW = m_nWidth;
        nH = m_nHeight;
    }

    EnterCriticalSection(&m_csList);
    S_VGElem *pCurElem = m_pFirst[1 - m_nCurList];
    while(pCurElem)
    {
        pCurElem->pElem->Draw(hDC, nX, nY, nW, nH, nK);
        pCurElem = pCurElem->pNext;
    }
    LeaveCriticalSection(&m_csList);
}
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
#include <windows.h>
#include "GraphElem.h"
    //--------------------------------------------------------------------
static BOOL IsRectInRect(int nLeft1, int nTop1, int nRight1, int nBottom1, 
            int nLeft2, int nTop2, int nRight2, int nBottom2)
{
    if(nLeft1 > nRight1)
    {
        nLeft1 = nLeft1 ^ nRight1;
        nRight1 = nLeft1 ^ nRight1;
        nLeft1 = nLeft1 ^ nRight1;
    }
    if(nTop1 > nBottom1)
    {
        nTop1 = nTop1 ^ nBottom1;
        nBottom1 = nTop1 ^ nBottom1;
        nTop1 = nTop1 ^ nBottom1;
    }

    if((nRight1 < nLeft2) || (nLeft1 > nRight2) || 
            (nBottom1 < nTop2) || (nTop1 > nBottom2))
    {
        return FALSE;
    }

    return TRUE;
}
    //--------------------------------------------------------------------
static BOOL IsBoundInRect(int *pnPoints, int nPoints, 
        int nLeft, int nTop, int nRight, int nBottom)
{
    if(!pnPoints || !nPoints)
    {
        return FALSE;
    }

    int nX1 = pnPoints[0];
    int nY1 = pnPoints[1];
    int nX2 = nX1;
    int nY2 = nY1;
    int i, j;
    for(i=1, j=2; i<nPoints; i++, j+=2)
    {
        if(nX1 > pnPoints[j])
        {
            nX1 = pnPoints[j];
        }
        else if(nX2 < pnPoints[j])
        {
            nX2 = pnPoints[j];
        }
        if(nY1 > pnPoints[j+1])
        {
            nY1 = pnPoints[j+1];
        }
        else if(nY2 < pnPoints[j+1])
        {
            nY2 = pnPoints[j+1];
        }
    }
    return IsRectInRect(nX1, nY1, nX2, nY2, nLeft, nTop, nRight, nBottom);
}
    //--------------------------------------------------------------------
C_GraphElem *CreateGraphElem(int nType)
{
    C_GraphElem *pGE = NULL;

    switch(nType)
    {
    case GRAPH_PIXEL:
        pGE = (C_GraphElem*)new C_Pixel; 
        break;
    case GRAPH_LINE:
        pGE = (C_GraphElem*)new C_Line;
        break;     
    case GRAPH_POLYLINE:
        pGE = (C_GraphElem*)new C_Polyline;
        break; 
    case GRAPH_POLYBEZIER:
        pGE = (C_GraphElem*)new C_PolyBezier;
        break; 
    case GRAPH_ARC:
        pGE = (C_GraphElem*)new C_Arc;
        break;      
    case GRAPH_RECTANGLE:
        pGE = (C_GraphElem*)new C_Rectangle;
        break;
    case GRAPH_POLYGON:
        pGE = (C_GraphElem*)new C_Polygon;
        break;  
    case GRAPH_ELLIPSE:
        pGE = (C_GraphElem*)new C_Ellipse;
        break;  
    case GRAPH_CHORD:
        pGE = (C_GraphElem*)new C_Chord;
        break;    
    case GRAPH_PIE:
        pGE = (C_GraphElem*)new C_Pie;
        break;      
    case GRAPH_PIXEL_F:
        pGE = (C_GraphElem*)new C_PixelF; 
        break;
    case GRAPH_LINE_F:
        pGE = (C_GraphElem*)new C_LineF;
        break;     
    case GRAPH_POLYLINE_F:
        pGE = (C_GraphElem*)new C_PolylineF;
        break; 
    case GRAPH_POLYBEZIER_F:
        pGE = (C_GraphElem*)new C_PolyBezierF;
        break; 
    case GRAPH_ARC_F:
        pGE = (C_GraphElem*)new C_ArcF;
        break;      
    case GRAPH_RECTANGLE_F:
        pGE = (C_GraphElem*)new C_RectangleF;
        break;
    case GRAPH_POLYGON_F:
        pGE = (C_GraphElem*)new C_PolygonF;
        break;  
    case GRAPH_ELLIPSE_F:
        pGE = (C_GraphElem*)new C_EllipseF;
        break;  
    case GRAPH_CHORD_F:
        pGE = (C_GraphElem*)new C_ChordF;
        break;    
    case GRAPH_PIE_F:
        pGE = (C_GraphElem*)new C_PieF;
        break;      
    case GRAPH_TEXTOUT:
        pGE = (C_GraphElem*)new C_TextOut;
        break;
    }
    return pGE;
}
    //--------------------------------------------------------------------
C_GraphElem::C_GraphElem()
{
    ZeroMemory(pnParams, GRAPH_MAX_PARAMS * sizeof(int));
}
    //--------------------------------------------------------------------
void C_Pixel::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    POINT pt = {pnParams[0], pnParams[1]};
    RECT rc = {nX, nY, nX+nW, nY+nH};

    if(PtInRect(&rc, pt))
    {
        SetPixel(hDC, (pt.x - nX) << nK, (pt.y - nY) << nK, pnParams[2]);
    }
}
    //--------------------------------------------------------------------
void C_Line::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!IsRectInRect(pnParams[0], pnParams[1], pnParams[2], pnParams[3], 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    HPEN hPen, hOldPen;
    hPen = CreatePen(pnParams[5], 1, pnParams[4]);
    hOldPen = (HPEN)SelectObject(hDC, hPen);

	MoveToEx(hDC, (pnParams[0] - nX) << nK, (pnParams[1] - nY) << nK, NULL);
    LineTo(hDC, (pnParams[2] - nX) << nK, (pnParams[3] - nY) << nK);    
	
    SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}
    //--------------------------------------------------------------------
void C_Polyline::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!IsBoundInRect((int*)pnParams[0], pnParams[1], nX, nY, nW+nX, nH+nY))
    {
        return;
    }

    LPPOINT pptPoints = new POINT[pnParams[1]];
    if(pptPoints == NULL)
    {
        return;
    }
    CopyMemory(pptPoints, (LPVOID)pnParams[0], pnParams[1] * sizeof(POINT));

    for(int i=0; i<pnParams[1]; i++)
    {
        (pptPoints[i].x -= nX) <<= nK;
        (pptPoints[i].y -= nY) <<= nK;
    }

    HPEN hPen, hOldPen;
    hPen = CreatePen(pnParams[3], 1, pnParams[2]);
    hOldPen = (HPEN)SelectObject(hDC, hPen);

    Polyline(hDC, pptPoints, pnParams[1]);

    SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
    delete [] pptPoints;
}
    //--------------------------------------------------------------------
void C_PolyBezier::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!IsBoundInRect((int*)pnParams[0], pnParams[1], nX, nY, nW+nX, nH+nY))
    {
        return;
    }

    LPPOINT pptPoints = new POINT[pnParams[1]];
    if(pptPoints == NULL)
    {
        return;
    }
    CopyMemory(pptPoints, (LPVOID)pnParams[0], pnParams[1] * sizeof(POINT));

    for(int i=0; i<pnParams[1]; i++)
    {
        (pptPoints[i].x -= nX) <<= nK;
        (pptPoints[i].y -= nY) <<= nK;
    }

    HPEN hPen, hOldPen;
    hPen = CreatePen(pnParams[3], 1, pnParams[2]);
    hOldPen = (HPEN)SelectObject(hDC, hPen);

    PolyBezier(hDC, pptPoints, pnParams[1]);

    SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
    delete [] pptPoints;
}
    //--------------------------------------------------------------------
void C_Arc::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!IsRectInRect(pnParams[0], pnParams[1], pnParams[2], pnParams[3], 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    HPEN hPen = CreatePen(pnParams[9], 1, pnParams[8]);
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
    Arc(hDC, (pnParams[0] - nX) << nK, (pnParams[1] - nY) << nK, 
            (pnParams[2] - nX) << nK, (pnParams[3] - nY) << nK,
            (pnParams[4] - nX) << nK, (pnParams[5] - nY) << nK,
            (pnParams[6] - nX) << nK, (pnParams[7] - nY) << nK);
    SelectObject(hDC, hOldPen);
    DeleteObject(hPen);
}
    //--------------------------------------------------------------------
void C_Rectangle::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!IsRectInRect(pnParams[0], pnParams[1], pnParams[2], pnParams[3], 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    HPEN hPen = CreatePen(pnParams[6], 0, pnParams[4]);
    HBRUSH hBrush;

    if(pnParams[5] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[5]);
    }
   	
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Rectangle(hDC, (pnParams[0] - nX) << nK, (pnParams[1] - nY) << nK, 
            (pnParams[2] - nX) << nK, (pnParams[3] - nY) << nK);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
    //--------------------------------------------------------------------
void C_Polygon::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!IsBoundInRect((int*)pnParams[0], pnParams[1], nX, nY, nW+nX, nH+nY))
    {
        return;
    }

    LPPOINT pptPoints = new POINT[pnParams[1]];
    if(pptPoints == NULL)
    {
        return;
    }
    CopyMemory(pptPoints, (LPVOID)pnParams[0], pnParams[1] * sizeof(POINT));

    for(int i=0; i<pnParams[1]; i++)
    {
        pptPoints[i].x = (pptPoints[i].x - nX) << nK;
        pptPoints[i].y = (pptPoints[i].y - nY) << nK;
    }

    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;

    if(pnParams[2] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[2]);
    }
    hPen = CreatePen(pnParams[4], 1, pnParams[2]);
    hOldPen = (HPEN)SelectObject(hDC, hPen);
    hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Polygon(hDC, pptPoints, pnParams[1]);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);
	DeleteObject(hBrush);
    
    delete [] pptPoints;
}
    //--------------------------------------------------------------------
void C_Ellipse::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!IsRectInRect(pnParams[0], pnParams[1], pnParams[2], pnParams[3], 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    HPEN hPen = CreatePen(pnParams[6], 1, pnParams[4]);
    HBRUSH hBrush;

    if(pnParams[5] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[5]);
    }
   	
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Ellipse(hDC, (pnParams[0] - nX) << nK, (pnParams[1] - nY) << nK, 
            (pnParams[2] - nX) << nK, (pnParams[3] - nY) << nK);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
    //--------------------------------------------------------------------
void C_Chord::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!IsRectInRect(pnParams[0], pnParams[1], pnParams[2], pnParams[3], 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }
    
    HPEN hPen = CreatePen(pnParams[10], 1, pnParams[8]);
    HBRUSH hBrush;
    if(pnParams[9] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[9]);
    }
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Chord(hDC, (pnParams[0] - nX) << nK, (pnParams[1] - nY) << nK, 
            (pnParams[2] - nX) << nK, (pnParams[3] - nY) << nK,
            (pnParams[4] - nX) << nK, (pnParams[5] - nY) << nK,
            (pnParams[6] - nX) << nK, (pnParams[7] - nY) << nK);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
    //--------------------------------------------------------------------
void C_Pie::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    if(!IsRectInRect(pnParams[0], pnParams[1], pnParams[2], pnParams[3], 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    HPEN hPen = CreatePen(pnParams[10], 1, pnParams[8]);
    HBRUSH hBrush;
    if(pnParams[9] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[9]);
    }
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Pie(hDC, (pnParams[0] - nX) << nK, (pnParams[1] - nY) << nK, 
            (pnParams[2] - nX) << nK, (pnParams[3] - nY) << nK,
            (pnParams[4] - nX) << nK, (pnParams[5] - nY) << nK,
            (pnParams[6] - nX) << nK, (pnParams[7] - nY) << nK);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
    //--------------------------------------------------------------------
void C_PixelF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    POINT pt;
    RECT rc = {nX, nY, nX+nW, nY+nH};
    float fltX, fltY, fltK;

    fltK = (float)(1 << nK);
    fltX = *(float*)&pnParams[0];
    fltY = *(float*)&pnParams[1];
    pt.x = (int)fltX;
    pt.y = (int)fltY;
    
    if(PtInRect(&rc, pt))
    {
        fltX = (fltX - (float)nX) * fltK;
        fltY = (fltY - (float)nY) * fltK;
        SetPixel(hDC, (int)fltX, (int)fltY, pnParams[2]);
    }
}
    //--------------------------------------------------------------------
void C_LineF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    float fltLeft, fltTop, fltRight, fltBottom, fltK;
    HPEN hPen, hOldPen;

    fltLeft = *(float*)&pnParams[0];
    fltTop = *(float*)&pnParams[1];
    fltRight = *(float*)&pnParams[2];
    fltBottom = *(float*)&pnParams[3];

    if(!IsRectInRect((int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom, 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }
    
    fltK = (float)(1 << nK);
    fltLeft = (fltLeft - (float)nX) * fltK;
    fltTop = (fltTop - (float)nY) * fltK;
    fltRight = (fltRight - (float)nX) * fltK;
    fltBottom = (fltBottom - (float)nY) * fltK;

    
    hPen = CreatePen(pnParams[5], 1, pnParams[4]);
    hOldPen = (HPEN)SelectObject(hDC, hPen);

	MoveToEx(hDC, (int)fltLeft, (int)fltTop, NULL);
    LineTo(hDC, (int)(fltRight + 0.5f), (int)(fltBottom + 0.5f));    

    SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}
    //--------------------------------------------------------------------
void C_PolylineF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    HPEN hPen, hOldPen;
    LPPOINT pptPoints = new POINT[pnParams[1]];
    float *pfltPoints = (float*)pnParams[0];
    float fltK;
    int i, j;

    if(!pptPoints || !pfltPoints)
    {
        return;
    }

    for(i=0, j=0; i<pnParams[1]; i++, j+=2)
    {
        pptPoints[i].x = (int)pfltPoints[j];
        pptPoints[i].y = (int)pfltPoints[j + 1];
    }
    if(!IsBoundInRect((int*)pptPoints, pnParams[1], nX, nY, nW+nX, nH+nY))
    {
        return;
    }

    fltK = (float)(1 << nK);
    for(i=0, j=0; i<pnParams[1]; i++, j+=2)
    {
        pptPoints[i].x = (int)((pfltPoints[j] - (float)nX) * fltK);
        pptPoints[i].y = (int)((pfltPoints[j + 1] - (float)nY) * fltK);
    }
    
    hPen = CreatePen(pnParams[3], 1, pnParams[2]);
    hOldPen = (HPEN)SelectObject(hDC, hPen);

    Polyline(hDC, pptPoints, pnParams[1]);

    SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
    delete [] pptPoints;
}
    //--------------------------------------------------------------------
void C_PolyBezierF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    HPEN hPen, hOldPen;
    LPPOINT pptPoints = new POINT[pnParams[1]];
    float *pfltPoints = (float*)pnParams[0];
    float fltK;
    int i, j;

    if(!pptPoints || !pfltPoints)
    {
        return;
    }

    for(i=0, j=0; i<pnParams[1]; i++, j+=2)
    {
        pptPoints[i].x = (int)pfltPoints[j];
        pptPoints[i].y = (int)pfltPoints[j + 1];
    }
    if(!IsBoundInRect((int*)pptPoints, pnParams[1], nX, nY, nW+nX, nH+nY))
    {
        return;
    }

    fltK = (float)(1 << nK);
    for(i=0, j=0; i<pnParams[1]; i++, j+=2)
    {
        pptPoints[i].x = (int)((pfltPoints[j] - (float)nX) * fltK);
        pptPoints[i].y = (int)((pfltPoints[j + 1] - (float)nY) * fltK);
    }
    
    hPen = CreatePen(pnParams[3], 1, pnParams[2]);
    hOldPen = (HPEN)SelectObject(hDC, hPen);

    PolyBezier(hDC, pptPoints, pnParams[1]);

    SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
    delete [] pptPoints;
}
    //--------------------------------------------------------------------
void C_ArcF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    HPEN hPen, hOldPen;
    float fltLeft, fltTop, fltRight, fltBottom, fltXStart, fltYStart, fltXEnd, fltYEnd, fltK;

    fltLeft = *(float*)&pnParams[0];
    fltTop = *(float*)&pnParams[1];
    fltRight = *(float*)&pnParams[2];
    fltBottom = *(float*)&pnParams[3];
    fltXStart = *(float*)&pnParams[4];
    fltYStart = *(float*)&pnParams[5];
    fltXEnd = *(float*)&pnParams[6];
    fltYEnd = *(float*)&pnParams[7];
    if(!IsRectInRect((int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom, 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    fltK = (float)(1 << nK);
    fltLeft = (fltLeft - (float)nX) * fltK;
    fltTop = (fltTop - (float)nY) * fltK;
    fltRight = (fltRight - (float)nX) * fltK;
    fltBottom = (fltBottom - (float)nY) * fltK;
    fltXStart = (fltXStart - (float)nX) * fltK;
    fltYStart = (fltYStart - (float)nY) * fltK;
    fltXEnd = (fltXEnd - (float)nX) * fltK;
    fltYEnd = (fltYEnd - (float)nY) * fltK;

    hPen = CreatePen(pnParams[9], 1, pnParams[8]);
    hOldPen = (HPEN)SelectObject(hDC, hPen);
    Arc(hDC, (int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom,
            (int)fltXStart, (int)fltYStart, (int)fltXEnd, (int)(fltYEnd + 0.5f));
    SelectObject(hDC, hOldPen);
    DeleteObject(hPen);
}
    //--------------------------------------------------------------------
void C_RectangleF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    float fltLeft, fltTop, fltRight, fltBottom, fltK;

    fltLeft = *(float*)&pnParams[0];
    fltTop = *(float*)&pnParams[1];
    fltRight = *(float*)&pnParams[2];
    fltBottom = *(float*)&pnParams[3];
    if(!IsRectInRect((int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom, 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    fltK = (float)(1 << nK);
    fltLeft = (fltLeft - (float)nX) * fltK;
    fltTop = (fltTop - (float)nY) * fltK;
    fltRight = (fltRight - (float)nX) * fltK;
    fltBottom = (fltBottom - (float)nY) * fltK;

    hPen = CreatePen(pnParams[6], 1, pnParams[4]);
    if(pnParams[5] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[5]);
    }
   	
    hOldPen = (HPEN)SelectObject(hDC, hPen);
    hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Rectangle(hDC, (int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
    //--------------------------------------------------------------------
void C_PolygonF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    LPPOINT pptPoints = new POINT[pnParams[1]];
    float *pfltPoints = (float*)pnParams[0];
    float fltK;
    int i, j;

    if(!pptPoints || !pfltPoints)
    {
        return;
    }

    for(i=0, j=0; i<pnParams[1]; i++, j+=2)
    {
        pptPoints[i].x = (int)pfltPoints[j];
        pptPoints[i].y = (int)pfltPoints[j + 1];
    }
    if(!IsBoundInRect((int*)pptPoints, pnParams[1], nX, nY, nW+nX, nH+nY))
    {
        return;
    }

    fltK = (float)(1 << nK);
    for(i=0, j=0; i<pnParams[1]; i++, j+=2)
    {
        pptPoints[i].x = (int)((pfltPoints[j] - (float)nX) * fltK);
        pptPoints[i].y = (int)((pfltPoints[j + 1] - (float)nY) * fltK);
    }
    
    hPen = CreatePen(pnParams[4], 1, pnParams[2]);
    if(pnParams[3] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[3]);
    }
    hOldPen = (HPEN)SelectObject(hDC, hPen);
    hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Polygon(hDC, pptPoints, pnParams[1]);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
	DeleteObject(hPen);
    DeleteObject(hBrush);
    delete [] pptPoints;
}
    //--------------------------------------------------------------------
void C_EllipseF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    float fltLeft, fltTop, fltRight, fltBottom, fltK;

    fltLeft = *(float*)&pnParams[0];
    fltTop = *(float*)&pnParams[1];
    fltRight = *(float*)&pnParams[2];
    fltBottom = *(float*)&pnParams[3];
    if(!IsRectInRect((int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom, 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    fltK = (float)(1 << nK);
    fltLeft = (fltLeft - (float)nX) * fltK;
    fltTop = (fltTop - (float)nY) * fltK;
    fltRight = (fltRight - (float)nX) * fltK;
    fltBottom = (fltBottom - (float)nY) * fltK;

    hPen = CreatePen(pnParams[6], 1, pnParams[4]);
    if(pnParams[5] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[5]);
    }
   	
    hOldPen = (HPEN)SelectObject(hDC, hPen);
    hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Ellipse(hDC, (int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
    //--------------------------------------------------------------------
void C_ChordF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    float fltLeft, fltTop, fltRight, fltBottom, fltXRad1, fltYRad1, fltXRad2, fltYRad2, fltK;

    fltLeft = *(float*)&pnParams[0];
    fltTop = *(float*)&pnParams[1];
    fltRight = *(float*)&pnParams[2];
    fltBottom = *(float*)&pnParams[3];
    fltXRad1 = *(float*)&pnParams[4];
    fltYRad1 = *(float*)&pnParams[5];
    fltXRad2 = *(float*)&pnParams[6];
    fltYRad2 = *(float*)&pnParams[7];
    if(!IsRectInRect((int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom, 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    fltK = (float)(1 << nK);
    fltLeft = (fltLeft - (float)nX) * fltK;
    fltTop = (fltTop - (float)nY) * fltK;
    fltRight = (fltRight - (float)nX) * fltK;
    fltBottom = (fltBottom - (float)nY) * fltK;
    fltXRad1 = (fltXRad1 - (float)nX) * fltK;
    fltYRad1 = (fltYRad1 - (float)nY) * fltK;
    fltXRad2 = (fltXRad2 - (float)nX) * fltK;
    fltYRad2 = (fltYRad2 - (float)nY) * fltK;

    hPen = CreatePen(pnParams[10], 1, pnParams[8]);
    if(pnParams[9] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[9]);
    }
    hOldPen = (HPEN)SelectObject(hDC, hPen);
    hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Chord(hDC, (int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom,
            (int)fltXRad1, (int)fltYRad1, (int)fltXRad2, (int)fltYRad2);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
    //--------------------------------------------------------------------
void C_PieF::Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK)
{
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    float fltLeft, fltTop, fltRight, fltBottom, fltXRad1, fltYRad1, fltXRad2, fltYRad2, fltK;

    fltLeft = *(float*)&pnParams[0];
    fltTop = *(float*)&pnParams[1];
    fltRight = *(float*)&pnParams[2];
    fltBottom = *(float*)&pnParams[3];
    fltXRad1 = *(float*)&pnParams[4];
    fltYRad1 = *(float*)&pnParams[5];
    fltXRad2 = *(float*)&pnParams[6];
    fltYRad2 = *(float*)&pnParams[7];
    if(!IsRectInRect((int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom, 
            nX, nY, nW + nX, nH + nY))
    {
        return;
    }

    fltK = (float)(1 << nK);
    fltLeft = (fltLeft - (float)nX) * fltK;
    fltTop = (fltTop - (float)nY) * fltK;
    fltRight = (fltRight - (float)nX) * fltK;
    fltBottom = (fltBottom - (float)nY) * fltK;
    fltXRad1 = (fltXRad1 - (float)nX) * fltK;
    fltYRad1 = (fltYRad1 - (float)nY) * fltK;
    fltXRad2 = (fltXRad2 - (float)nX) * fltK;
    fltYRad2 = (fltYRad2 - (float)nY) * fltK;

    hPen = CreatePen(pnParams[10], 1, pnParams[8]);
    if(pnParams[9] == 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(pnParams[9]);
    }
    hOldPen = (HPEN)SelectObject(hDC, hPen);
    hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Pie(hDC, (int)fltLeft, (int)fltTop, (int)fltRight, (int)fltBottom,
            (int)fltXRad1, (int)fltYRad1, (int)fltXRad2, (int)fltYRad2);
    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
    //--------------------------------------------------------------------
void C_TextOut::Draw(HDC hDC, int nX, int nY, int, int, int nK)
{
    HFONT hFont, hOldFont;
    COLORREF clrOldColor;

    hFont = CreateFont(-MulDiv(pnParams[3] << nK, GetDeviceCaps(hDC, LOGPIXELSY), 72),
            0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
            DEFAULT_PITCH, "Arial");
    clrOldColor = SetTextColor(hDC, (COLORREF)pnParams[4]);
    hOldFont = (HFONT)SelectObject(hDC, hFont);
    TextOut(hDC, (pnParams[1] - nX) << nK, (pnParams[2] - nY) << nK, 
            (char*)pnParams[0], lstrlen((char*)pnParams[0]));
    SelectObject(hDC, hOldFont);
    DeleteObject(hFont);
    SetTextColor(hDC, clrOldColor);
}
    //--------------------------------------------------------------------

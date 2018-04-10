//-------------------------------------------------------------------
#include <windows.h>
#include "EnhMetaFile.h"
#include "vShell.h"
//-------------------------------------------------------------------
C_EnhMetaFile::C_EnhMetaFile()
{
    m_hDC = NULL;
    m_hMF = NULL;
    InitializeCriticalSection(&m_CS);
}
//-------------------------------------------------------------------
C_EnhMetaFile::~C_EnhMetaFile()
{
    if(m_hDC)
    {
        DeleteDC(m_hDC);
    }
    if(m_hMF)
    {
        DeleteEnhMetaFile(m_hMF);
    }
    DeleteCriticalSection(&m_CS);
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::Init(int nWidth, int nHeight)
{
    HDC hDC = GetDC(NULL);
    m_dPixInMmX = (double)GetDeviceCaps(hDC, HORZSIZE) / 
            (double)GetDeviceCaps(hDC, HORZRES) * 100.0;
    m_dPixInMmY = (double)GetDeviceCaps(hDC, VERTSIZE) / 
            (double)GetDeviceCaps(hDC, VERTRES) * 100.0;
    ReleaseDC(NULL, hDC);

    m_rcView.left = m_rcView.top = 0;
    m_rcView.right = nWidth;
    m_rcView.bottom = nHeight;

    m_rcMF.left = m_rcMF.top = 0;
    m_rcMF.right = (int)(nWidth * m_dPixInMmX);
    m_rcMF.bottom = (int)(nHeight * m_dPixInMmY);
    m_hDC = CreateEnhMetaFile(NULL, NULL, &m_rcMF, NULL);

    return TRUE;
}
//-------------------------------------------------------------------
void C_EnhMetaFile::Flush()
{
    if(m_hMF)
    {
        DeleteEnhMetaFile(m_hMF);
    }
    m_hMF = CloseEnhMetaFile(m_hDC);
    m_hDC = CreateEnhMetaFile(NULL, NULL, &m_rcMF, NULL);
}
//-------------------------------------------------------------------
void C_EnhMetaFile::Draw(HDC hDC)
{
    EnterCriticalSection(&m_CS);
    if(m_hMF)
    {
        PlayEnhMetaFile(hDC, m_hMF, &m_rcView);
    }
    LeaveCriticalSection(&m_CS);
}
//-------------------------------------------------------------------
void C_EnhMetaFile::StretchDraw(HDC hDC, RECT *prcDst, RECT *prcSrc)
{
    EnterCriticalSection(&m_CS);
    if(m_hMF)
    {
        HENHMETAFILE hMFStretch;
        ENHMETAHEADER *pMFHeader;
        int nMFSize;
        
        if((nMFSize = GetEnhMetaFileBits(m_hMF, 0, NULL)) == 0)
        {
            LeaveCriticalSection(&m_CS);
            MessageBox(NULL, "GetEnhMetaFileBits()", "Error", MB_OK);
            return;
        }
        BYTE *pbMFData = new BYTE[nMFSize];
        if(pbMFData == NULL)
        {
            LeaveCriticalSection(&m_CS);
            MessageBox(NULL, "new", "Error", MB_OK);
            return;
        }
        pMFHeader = (ENHMETAHEADER*)pbMFData;
        if(!GetEnhMetaFileBits(m_hMF, nMFSize, pbMFData))
        {
            LeaveCriticalSection(&m_CS);
            MessageBox(NULL, "GetEnhMetaFileBits()", "Error", MB_OK);
            return;
        }
        pMFHeader->rclBounds.left = prcSrc->left;
        pMFHeader->rclBounds.top = prcSrc->top;
        pMFHeader->rclBounds.right = prcSrc->right + prcSrc->left;
        pMFHeader->rclBounds.bottom = prcSrc->bottom + prcSrc->top;
        pMFHeader->rclFrame.left = (long)(prcSrc->left * m_dPixInMmX);
        pMFHeader->rclFrame.top = (long)(prcSrc->top * m_dPixInMmY);
        pMFHeader->rclFrame.right = (long)((prcSrc->right + prcSrc->left) * m_dPixInMmX);
        pMFHeader->rclFrame.bottom = (long)((prcSrc->bottom + prcSrc->top) * m_dPixInMmY);
        if((hMFStretch = SetEnhMetaFileBits(nMFSize, pbMFData)) == NULL)
        {
            LeaveCriticalSection(&m_CS);
            MessageBox(NULL, "SetEnhMetaFileBits()", "Error", MB_OK);
            return;
        }
        delete [] pbMFData;
        if((PlayEnhMetaFile(hDC, hMFStretch, prcDst)) == NULL)
        {
            LeaveCriticalSection(&m_CS);
            MessageBox(NULL, "PlayEnhMetaFile()", "Error", MB_OK);
        }
        if(!DeleteEnhMetaFile(hMFStretch))
        {
            MessageBox(NULL, "DeleteEnhMetaFile()", "Error", MB_OK);
        }
        if((m_hDC = CreateEnhMetaFile(NULL, NULL, &m_rcMF, NULL)) == NULL)
        {
            MessageBox(NULL, "CreateEnhMetaFile()", "Error", MB_OK);
        }
    }
    LeaveCriticalSection(&m_CS);
}
//-------------------------------------------------------------------
void C_EnhMetaFile::Grid(int nStepX, int nStepY, COLORREF crColor)
{
    EnterCriticalSection(&m_CS);
    HPEN hPen = CreatePen(0, PS_SOLID, crColor);
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    int i;
    for(i=nStepX; i<m_rcView.right; i+=nStepX)
    {
        MoveToEx(m_hDC, i, 0, NULL);
        LineTo(m_hDC, i, m_rcView.bottom);
    }
    for(i=nStepY; i<m_rcView.bottom; i+=nStepY)
    {
        MoveToEx(m_hDC, 0, i, NULL);
        LineTo(m_hDC, m_rcView.right, i);
    }
    SelectObject(m_hDC, hOldPen);
    DeleteObject(hPen);
    LeaveCriticalSection(&m_CS);
}
//-------------------------------------------------------------------
void C_EnhMetaFile::Vectors(int nBlockSize, int *pnVectors, COLORREF crColor)
{
    EnterCriticalSection(&m_CS);
    HPEN hPen = CreatePen(0, PS_SOLID, crColor);
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);

    int i, nX, nY;
    int nHalf = nBlockSize / 2;

	int nVecX;
	int nVecY;

    for(i=0, nY=nHalf; nY<m_rcView.bottom; nY+=nBlockSize)
    {
        for(nX=nHalf; nX<m_rcView.right; nX+=nBlockSize, i+=2)
        {
			nVecX=pnVectors[i];
			nVecY=pnVectors[i+1];
			if (nVecX!=VS_NULL_VEC)
			{
				MoveToEx(m_hDC, nX, nY, NULL);
				LineTo(m_hDC, nX + nVecX, nY + nVecY);
			}
			else
				::Ellipse(m_hDC, nX + nVecX-1, nY + nVecY-1, nX + nVecX+1, nY + nVecY+1);
        }
    }
    SelectObject(m_hDC, hOldPen);
    DeleteObject(hPen);
    LeaveCriticalSection(&m_CS);
}
//-------------------------------------------------------------------
void C_EnhMetaFile::Vectors(int nBlockSize, int *pnVectors, COLORREF *pcrColor)
{
    EnterCriticalSection(&m_CS);
    HPEN hPen = NULL;
    HPEN hOldPen = NULL;

    int i = 0, j = 0, nX, nY;
    int nHalf = nBlockSize / 2;

	int nVecX;
	int nVecY;

    for(nY=nHalf; nY<m_rcView.bottom; nY+=nBlockSize)
    {
        for(nX=nHalf; nX<m_rcView.right; nX+=nBlockSize, i+=2)
        {
            hPen = CreatePen(0, PS_SOLID, pcrColor[j++]);
            if(hOldPen == NULL)
            {
                hOldPen = (HPEN)SelectObject(m_hDC, hPen);
            }
            SelectObject(m_hDC, hPen);

			nVecX=pnVectors[i];
			nVecY=pnVectors[i+1];
			if (nVecX!=VS_NULL_VEC)
			{
				MoveToEx(m_hDC, nX, nY, NULL);
				LineTo(m_hDC, nX + nVecX, nY + nVecY);
			}
			else
				::Ellipse(m_hDC, nX + nVecX-1, nY + nVecY-1, nX + nVecX+1, nY + nVecY+1);

            DeleteObject(SelectObject(m_hDC, hOldPen));
        }
    }
    LeaveCriticalSection(&m_CS);
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::SetPixel(int nX, int nY, COLORREF crColor)
{
/*    EnterCriticalSection(&m_CS);
    BOOL fRes;
    fRes = ::SetPixelV(m_hDC, nX, nY, crColor);
    LeaveCriticalSection(&m_CS);
    return fRes;
*/
    return Ellipse(nX, nY, nX+2, nY+2, crColor, crColor);
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::Line(int nX1, int nY1, int nX2, int nY2, COLORREF crColor)
{
    EnterCriticalSection(&m_CS);
    BOOL fRes;
    HPEN hPen = CreatePen(0, PS_SOLID, crColor);
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    ::MoveToEx(m_hDC, nX1, nY1, NULL);
    fRes = ::LineTo(m_hDC, nX2, nY2);
    SelectObject(m_hDC, hOldPen);
    DeleteObject(hPen);
    LeaveCriticalSection(&m_CS);
    return fRes;
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::PolyLine(POINT *pPoints, int nPoints, COLORREF crColor)
{
    EnterCriticalSection(&m_CS);
    BOOL fRes;
    HPEN hPen = CreatePen(0, PS_SOLID, crColor);
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    fRes = ::Polyline(m_hDC, pPoints, nPoints);
    SelectObject(m_hDC, hOldPen);
    DeleteObject(hPen);
    LeaveCriticalSection(&m_CS);
    return fRes;
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::PolyBezier(POINT *pPoints, int nPoints, COLORREF crColor)
{
    EnterCriticalSection(&m_CS);
    BOOL fRes;
    HPEN hPen = CreatePen(0, PS_SOLID, crColor);
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    fRes = ::PolyBezier(m_hDC, pPoints, nPoints);
    SelectObject(m_hDC, hOldPen);
    DeleteObject(hPen);
    LeaveCriticalSection(&m_CS);
    return fRes;
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::Arc(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect,
            int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
            COLORREF crColor)
{
    EnterCriticalSection(&m_CS);
    BOOL fRes;
    HPEN hPen = CreatePen(0, PS_SOLID, crColor);
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    fRes = ::Arc(m_hDC, nLeftRect, nTopRect, nRightRect, nBottomRect,
            nXStartArc, nYStartArc, nXEndArc, nYEndArc);
    SelectObject(m_hDC, hOldPen);
    DeleteObject(hPen);
    LeaveCriticalSection(&m_CS);
    return fRes;
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::Rectangle(int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor)
{
    EnterCriticalSection(&m_CS);
    BOOL fRes;
    HPEN hPen = CreatePen(0, PS_SOLID, crBoundColor);
    HBRUSH hBrush;
    if(crFillColor & 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(crFillColor);
    }
    
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hDC, hBrush);
    fRes = ::Rectangle(m_hDC, nLeftRect, nTopRect, 
            nRightRect, nBottomRect);
    SelectObject(m_hDC, hOldPen);
    SelectObject(m_hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
    LeaveCriticalSection(&m_CS);
    return fRes;
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::Polygon(POINT *pptPoints, int nPoints, 
        COLORREF crBoundColor, COLORREF crFillColor)
{
    EnterCriticalSection(&m_CS);
    BOOL fRes;
    HPEN hPen = CreatePen(0, PS_SOLID, crBoundColor);
    HBRUSH hBrush;
    if(crFillColor & 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(crFillColor);
    }
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hDC, hBrush);
    fRes = ::Polygon(m_hDC, pptPoints, nPoints);
    SelectObject(m_hDC, hOldPen);
    SelectObject(m_hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
    LeaveCriticalSection(&m_CS);
    return fRes;
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::Ellipse(int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor)
{
    EnterCriticalSection(&m_CS);
    BOOL fRes;
    HPEN hPen = CreatePen(0, PS_SOLID, crBoundColor);
    HBRUSH hBrush;
    if(crFillColor & 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(crFillColor);
    }
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hDC, hBrush);
    fRes = ::Ellipse(m_hDC, nLeftRect, nTopRect, 
            nRightRect, nBottomRect);
    SelectObject(m_hDC, hOldPen);
    SelectObject(m_hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
    LeaveCriticalSection(&m_CS);
    return fRes;
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::Chord(int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor)
{
    EnterCriticalSection(&m_CS);
    BOOL fRes;
    HPEN hPen = CreatePen(0, PS_SOLID, crBoundColor);
    HBRUSH hBrush;
    if(crFillColor & 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(crFillColor);
    }
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hDC, hBrush);
    fRes = ::Chord(m_hDC, nLeftRect, nTopRect, 
            nRightRect, nBottomRect,
            nXRadial1, nYRadial1,
            nXRadial2, nYRadial2);
    SelectObject(m_hDC, hOldPen);
    SelectObject(m_hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
    LeaveCriticalSection(&m_CS);
    return fRes;
}
//-------------------------------------------------------------------
BOOL C_EnhMetaFile::Pie(int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor)
{
    EnterCriticalSection(&m_CS);
    BOOL fRes;
    HPEN hPen = CreatePen(0, PS_SOLID, crBoundColor);
    HBRUSH hBrush;
    if(crFillColor & 0x80000000)
    {
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        hBrush = CreateSolidBrush(crFillColor);
    }
    HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hDC, hBrush);
    fRes = ::Pie(m_hDC, nLeftRect, nTopRect, 
            nRightRect, nBottomRect,
            nXRadial1, nYRadial1,
            nXRadial2, nYRadial2);
    SelectObject(m_hDC, hOldPen);
    SelectObject(m_hDC, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
    LeaveCriticalSection(&m_CS);
    return fRes;
}
//-------------------------------------------------------------------
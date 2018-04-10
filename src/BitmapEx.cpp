//------------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include "VShell.h"
#include "BitmapEx.h"
//------------------------------------------------------------------------
static const COLORREF LOW_OVERFLOW_CLR = RGB(0, 0, 255);
static const COLORREF HI_OVERFLOW_CLR = RGB(255, 0, 0);
static const COLORREF GRID_CLR = RGB(127, 127, 127);
static const int GRID_SIZE = 8;
static const COLORREF VECTOR_CLR = RGB(0, 255, 0);
static const int MIN_BLOCK_SIZE = 1;
static const int MAX_BLOCK_SIZE = 64;
static const int MAX_SCALE = 5;
//------------------------------------------------------------------------
C_BitmapEx::C_BitmapEx()
{
    m_fShowOverflows = FALSE;
    m_LowOverflows.nOverflows = 0;
    m_HiOverflows.nOverflows = 0;
    m_fShowOverflows = FALSE;
    m_fRaster = FALSE;
    m_fShowUserGraphic = TRUE;
    m_pImage = NULL;
    m_hWnd = NULL;
    m_nScale = 0;
    m_nXScale = 0;
    m_nYScale = 0;
    m_pAVIW = NULL;
    ZeroMemory(&m_BFH, sizeof(BITMAPFILEHEADER));
    ZeroMemory(&m_BIH, sizeof(BITMAPINFOHEADER));
}
//------------------------------------------------------------------------
C_BitmapEx::~C_BitmapEx()
{
    delete m_pImage;
    m_pImage = NULL;
    delete [] m_LowOverflows.pptOverflows;
    m_LowOverflows.pptOverflows = NULL;
    delete [] m_HiOverflows.pptOverflows;
    m_HiOverflows.pptOverflows = NULL;
    DeleteAVIWriter();
}
//------------------------------------------------------------------------
BOOL C_BitmapEx::Init(HWND hWnd, int nWidth, int nHeight, int nType, LPRGBQUAD prgbPal)
{
    m_hWnd = hWnd;
    
    switch(nType)
    {
    case VS_RGB1:
        m_pImage = (C_Image*)new C_ImageRGB1;
        break;
    case VS_RGB4:
        m_pImage = (C_Image*)new C_ImageRGB4;
        break;
    case VS_RGB8:
        m_pImage = (C_Image*)new C_ImageRGB8;
        break;
    case VS_RGB16:
        m_pImage = (C_Image*)new C_ImageRGB16;
        break;
    case VS_RGB24:
        m_pImage = (C_Image*)new C_ImageRGB24;
        break;
    case VS_RGB32:
        m_pImage = (C_Image*)new C_ImageRGB32;
        break;
    case VS_RGB8_8:
        m_pImage = (C_Image*)new C_ImageRGB8_8;
        break;    
    case VS_RGB8_16:
        m_pImage = (C_Image*)new C_ImageRGB8_16;
        break;
    case VS_RGB8_32:
        m_pImage = (C_Image*)new C_ImageRGB8_32;
        break;
    default:
        return FALSE;
    }
    if(m_pImage == NULL)
    {
        return FALSE;
    }

    m_nScale = 0;
    m_nXScale = 0;
    m_nYScale = 0;
    m_pImage->Init(nWidth, nHeight, prgbPal);
    m_VG.Init(nWidth, nHeight);
    m_LowOverflows.pptOverflows = new POINT[nWidth * nHeight];
    m_HiOverflows.pptOverflows = new POINT[nWidth * nHeight];

    ZeroMemory(&m_BIH, sizeof(BITMAPINFOHEADER));
    if(Width() & 0x3)
    {
        m_BIH.biWidth = Width() + (4 - (Width() & 0x3));
    }
    else
    {
        m_BIH.biWidth = Width();
    }
    m_BIH.biSize = sizeof(BITMAPINFOHEADER);
    m_BIH.biPlanes = 1;
    m_BIH.biHeight = Height();
	m_BIH.biBitCount = 24;
    m_BIH.biSizeImage = m_BIH.biHeight * m_BIH.biWidth * 3;

    m_BFH.bfType = 'MB';
    m_BFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    m_BFH.bfSize = m_BFH.bfOffBits + m_BIH.biSizeImage;
    m_BFH.bfReserved1 = 0;
    m_BFH.bfReserved2 = 0;

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_BitmapEx::Draw()
{
    HDC hDC;
    HDC hMemDC;
    HBITMAP hBmp, hOldBmp;
    RECT rcDst;

    int i;
        
    if(!m_pImage)
    {
        return FALSE;
    }
    
    if((hDC = GetDC(m_hWnd)) == NULL)
    {
        return FALSE;
    }
    if((hMemDC = CreateCompatibleDC(hDC)) == NULL)
    {
        return FALSE;
    }
    
	SetBkMode(hMemDC, TRANSPARENT);

    GetClientRect(m_hWnd, &rcDst);
    rcDst.right = (rcDst.right >> m_nScale) << m_nScale;
    rcDst.bottom = (rcDst.bottom >> m_nScale) << m_nScale;

    if(m_nScale || rcDst.right < Width() || rcDst.bottom < Height())
    {
        RECT rcSrc;
        
        int nW = (Width() - m_nXScale) << m_nScale;
        int nH = (Height() - m_nYScale) << m_nScale;
        if(nW < rcDst.right)
        {
            rcDst.right = nW;
        }
        if(nH < rcDst.bottom)
        {
            rcDst.bottom = nH;
        }
        rcSrc.left = m_nXScale;
        rcSrc.top = Height() - m_nYScale - (rcDst.bottom >> m_nScale);
        rcSrc.right = (rcDst.right >> m_nScale);
        rcSrc.bottom = (rcDst.bottom >> m_nScale);
        
        hBmp = (HBITMAP)CreateCompatibleBitmap(hDC, rcDst.right, rcDst.bottom);
        hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

        if(m_fRaster)
        {
            m_pImage->Draw(hMemDC, hBmp, &m_LowOverflows.nOverflows,  m_LowOverflows.pptOverflows, 
                    &m_HiOverflows.nOverflows, m_HiOverflows.pptOverflows);
            if(m_fShowUserGraphic)
            {
                m_VG.DrawList(hMemDC);
            }
            m_VG.DrawGrid(hMemDC);
            m_VG.DrawVectors(hMemDC);
            rcSrc.top = m_nYScale;
            StretchBlt(hDC, 0, 0, rcDst.right, rcDst.bottom,
                    hMemDC, rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, SRCCOPY);            
        }
        else
        {
            m_pImage->StretchDraw(hMemDC, &rcDst, &rcSrc,&m_LowOverflows.nOverflows,
                    m_LowOverflows.pptOverflows, &m_HiOverflows.nOverflows, m_HiOverflows.pptOverflows);
            rcSrc.top = m_nYScale;
            if(m_fShowUserGraphic)
            {
                m_VG.DrawList(hMemDC, m_nXScale, m_nYScale, rcSrc.right,
                        rcSrc.bottom, m_nScale);
            }
            m_VG.DrawGrid(hMemDC, m_nXScale, m_nYScale, rcSrc.right,
                    rcSrc.bottom, m_nScale);
            m_VG.DrawVectors(hMemDC, m_nXScale, m_nYScale, rcSrc.right,
                    rcSrc.bottom, m_nScale);
            BitBlt(hDC, 0, 0, rcDst.right, rcDst.bottom, hMemDC, 0, 0, SRCCOPY);
        }
    }
    else
    {
        hBmp = (HBITMAP)CreateCompatibleBitmap(hDC, Width(), Height());
        hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);
        m_pImage->Draw(hMemDC, hBmp, &m_LowOverflows.nOverflows,
                m_LowOverflows.pptOverflows, &m_HiOverflows.nOverflows, 
                m_HiOverflows.pptOverflows);

        if(m_fShowOverflows)
        {
            int nOverflows = m_LowOverflows.nOverflows;
            POINT *pt = m_LowOverflows.pptOverflows; 
            for(i=0; i<nOverflows; i++)
            {
                ::SetPixel(hMemDC, pt[i].x, pt[i].y , LOW_OVERFLOW_CLR);
            }
            nOverflows = m_HiOverflows.nOverflows;
            pt = m_HiOverflows.pptOverflows; 
            for(i=0; i<nOverflows; i++)
            {
                ::SetPixel(hMemDC, pt[i].x, pt[i].y, HI_OVERFLOW_CLR);
            }
        }
        if(m_fShowUserGraphic)
        {
            m_VG.DrawList(hMemDC);
        }
        m_VG.DrawGrid(hMemDC);
        m_VG.DrawVectors(hMemDC);
        BitBlt(hDC, 0, 0, Width(), Height(), hMemDC, 0, 0, SRCCOPY);
    }

    SelectObject(hMemDC, hOldBmp);
    DeleteDC(hMemDC);
    DeleteObject(hBmp);
    ReleaseDC(m_hWnd, hDC);

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_BitmapEx::SetVectors(int nBlockSize, int *pnVectors, COLORREF VectorsColor)
{
    if(nBlockSize < MIN_BLOCK_SIZE || nBlockSize > MAX_BLOCK_SIZE)
    {
        return FALSE;
    }

    if(VectorsColor != 0x80000000)
    {
        m_VG.Vectors(nBlockSize, pnVectors, VectorsColor);
    }
    else
    {
        m_VG.Vectors(nBlockSize, pnVectors, m_VG.m_Vectors.crColor);
    }
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_BitmapEx::SetVectors(int nBlockSize, float *pfltVectors, COLORREF VectorsColor)
{
    if(nBlockSize < MIN_BLOCK_SIZE || nBlockSize > MAX_BLOCK_SIZE)
    {
        return FALSE;
    }

    if(VectorsColor != 0x80000000)
    {
        m_VG.Vectors(nBlockSize, pfltVectors, VectorsColor);
    }
    else
    {
        m_VG.Vectors(nBlockSize, pfltVectors, m_VG.m_Vectors.crColor);
    }
    return TRUE;
}
//------------------------------------------------------------------------
void C_BitmapEx::SetScale(int nXMouse, int nYMouse, BOOL fMin)
{
    GetScaleMouseFromMouse(nXMouse, nYMouse, &nXMouse, &nYMouse);

    if(fMin)
    {
        if(m_nScale > 0)
        {
            m_nScale--;
        }
    }
    else if(m_nScale < MAX_SCALE)
    {
        m_nScale++;
    }

    m_nXScale = nXMouse - (Width() >> (m_nScale + 1));
    m_nYScale = nYMouse - (Height() >> (m_nScale + 1));

    if(m_nXScale < 0)
    {
        m_nXScale = 0;
    }

    if(m_nYScale < 0)
    {
        m_nYScale = 0;
    }

    int a = Width() - (Width() >> (m_nScale));
    if(m_nXScale > a)
    {
        m_nXScale = a;
    }

    a = Height() - (Height() >> (m_nScale));
    if(m_nYScale > a)
    {
        m_nYScale = a;
    }
    
    if(m_nXScale < 0)
    {
        m_nXScale = 0;
    }

    if(m_nYScale < 0)
    {
        m_nYScale = 0;
    }
}
//------------------------------------------------------------------------
void C_BitmapEx::MoveImage(int nXDelta, int nYDelta)
{
    RECT rcWnd;
    GetClientRect(m_hWnd, &rcWnd);
    
    rcWnd.right = (rcWnd.right >> m_nScale) << m_nScale;
    rcWnd.bottom = (rcWnd.bottom >> m_nScale) << m_nScale;

    int a = m_nXScale + nXDelta;
    int b = (Width() - a) << m_nScale;

    if(((nXDelta > 0) && (b > rcWnd.right)) || ((nXDelta < 0) && (a >= 0)))
    {
        m_nXScale = a;
    }

    a = m_nYScale - nYDelta;
    b = (Height() - a) << m_nScale;
    
    if(((nYDelta > 0) && (a >= 0)) || ((nYDelta < 0) && (b > rcWnd.bottom)))
    {
        m_nYScale = a;
    }
    else
    {
        b = 0;
    }

    Draw();
}
//------------------------------------------------------------------------
void C_BitmapEx::MoveImage()
{
    m_nXScale = 0;
    m_nYScale = 0;
    Draw();    
}
//------------------------------------------------------------------------
void C_BitmapEx::GetScaleMouseFromMouse(int nXMouse, int nYMouse, int *pnXScaleMouse, int *pnYScaleMouse)
{
    *pnXScaleMouse = (nXMouse >> m_nScale) + m_nXScale;
    *pnYScaleMouse = (nYMouse >> m_nScale) + m_nYScale;  
}
//------------------------------------------------------------------------
void C_BitmapEx::GetMouseFromScaleMouse(int nXScaleMouse, int nYScaleMouse, int *pnXMouse, int *pnYMouse)
{
    *pnXMouse = (nXScaleMouse - m_nXScale) << m_nScale;
    *pnYMouse = (nYScaleMouse - m_nYScale) << m_nScale;
}
//------------------------------------------------------------------------
BOOL C_BitmapEx::SaveImage(char *szFileName)
{
    HDC hMemDC;
    HBITMAP hBmp;
    BITMAPFILEHEADER BFH;
    BITMAPINFO BI;
	BITMAP Bmp;
    RECT rcSrc, rcDst;
    BYTE *pbImgBuffer = NULL;
    FILE *fp = NULL;
    BOOL fAvi;
       
    if(lstrcmp(&szFileName[lstrlen(szFileName) - 3], "avi"))
    {
        fAvi = FALSE;
    }
    else
    {
        fAvi = TRUE;
    }
    
	ZeroMemory(&BI,sizeof(BITMAPINFOHEADER));
	BI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BI.bmiHeader.biWidth = Width() << m_nScale;
	BI.bmiHeader.biHeight = Height() << m_nScale;
	BI.bmiHeader.biPlanes = 1;
	BI.bmiHeader.biBitCount = 24;
    if(BI.bmiHeader.biWidth & 0x3)
    {
        BI.bmiHeader.biSizeImage = (BI.bmiHeader.biWidth * 3 + (4 - ((BI.bmiHeader.biWidth * 3) & 0x3))) * BI.bmiHeader.biHeight;
    }
	else
    {
        BI.bmiHeader.biSizeImage = BI.bmiHeader.biWidth * BI.bmiHeader.biHeight * 3;
    }

    ZeroMemory(&BFH, sizeof(BITMAPFILEHEADER));
    BFH.bfType = 'MB';
    BFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    BFH.bfSize = m_BFH.bfOffBits + BI.bmiHeader.biSizeImage;

    hMemDC = CreateCompatibleDC(NULL);
	SetBkMode(hMemDC, TRANSPARENT);
	hBmp = CreateDIBSection(NULL, &BI, DIB_RGB_COLORS, (void**)&pbImgBuffer, NULL, NULL);
	GetObject(hBmp, sizeof(BITMAP), &Bmp);
	SelectObject(hMemDC, hBmp);

	rcSrc.left = 0;
    rcSrc.top = 0;
    rcSrc.right = Width();
    rcSrc.bottom = Height();

    rcDst.left = 0;
    rcDst.top = 0;
    rcDst.right = Width() << m_nScale;
    rcDst.bottom = Height() << m_nScale;
      
    m_pImage->StretchDraw(hMemDC, &rcDst, &rcSrc, &m_LowOverflows.nOverflows,
            m_LowOverflows.pptOverflows, &m_HiOverflows.nOverflows, m_HiOverflows.pptOverflows);

    if(m_fShowUserGraphic)
    {
        m_VG.DrawList(hMemDC, 0, 0, rcDst.right, rcDst.bottom, m_nScale);
    }
    m_VG.DrawGrid(hMemDC, 0, 0, rcDst.right, rcDst.bottom, m_nScale);
    m_VG.DrawVectors(hMemDC, 0, 0, Width(), Height(), m_nScale);
    
    if(fAvi)
    {
        if(!m_pAVIW)
        {
	        CreateAVIWriter(szFileName);
		}
		m_pAVIW->WriteFrame(pbImgBuffer);
    }
    else
    {
        fp = fopen(szFileName, "wb");

        fwrite(&BFH, sizeof(BITMAPFILEHEADER), 1, fp);
        fwrite(&BI, sizeof(BITMAPINFOHEADER), 1, fp);
		fwrite(pbImgBuffer, BI.bmiHeader.biSizeImage, 1, fp);
        fclose(fp);
    }
    
    DeleteDC(hMemDC);
    DeleteObject(hBmp);

    return TRUE;
}
//------------------------------------------------------------------------
void C_BitmapEx::GetGridCoordinates(int *pnXGrid, int *pnYGrid)
{
    if(!m_VG.m_Grid.fShow)
    {
        *pnXGrid = 0;
        *pnYGrid = 0;
        return;
    }

    *pnXGrid /= m_VG.m_Grid.nX;
    *pnYGrid /= m_VG.m_Grid.nY;
}
//------------------------------------------------------------------------
void C_BitmapEx::GetVectorsInBlock(float *pfltXVector, float *pfltYVector)
{
    if(!m_VG.m_Vectors.fShow || (m_VG.m_Vectors.nBlockSize == 0))
    {
        *pfltXVector = 0.0f;
        *pfltYVector = 0.0f;
        return;
    }

    int nWidthInBlocks = Width() / m_VG.m_Vectors.nBlockSize;
    int nBlock = (int)(*pfltYVector / m_VG.m_Vectors.nBlockSize * nWidthInBlocks) + 
            (int)(*pfltXVector / m_VG.m_Vectors.nBlockSize);

    if(m_VG.m_Vectors.pnVectors)
    {
        *pfltXVector = (float)m_VG.m_Vectors.pnVectors[nBlock * 2];
        *pfltYVector = (float)m_VG.m_Vectors.pnVectors[nBlock * 2 + 1];
    }
    else if(m_VG.m_Vectors.pfltVectors)
    {
        *pfltXVector = m_VG.m_Vectors.pfltVectors[nBlock * 2];
        *pfltYVector = m_VG.m_Vectors.pfltVectors[nBlock * 2 + 1];
    }
}
//------------------------------------------------------------------------
BOOL C_BitmapEx::CorrectSignedImage()
{
    int nSize = Width() * Height();
    int i, nMin, nMax, nBrig;
    float fltMul;
    char *pcData;
    short *pshData;
    int *pnData;

    switch(Type())
    {
    case VS_RGB8_8:
        pcData = (char*)m_pImage->Data();
        nMin = nMax = pcData[0];
        for(i=1; i<nSize; i++)
        {
            if(nMin > pcData[i])
            {
                nMin = pcData[i];
            }
            if(nMax < pcData[i])
            {
                nMax = pcData[i];
            }
        }
        break;
    case VS_RGB8_16:
        pshData = (short*)m_pImage->Data();
        nMin = nMax = pshData[0];
        for(i=1; i<nSize; i++)
        {
            if(nMin > pshData[i])
            {
                nMin = pshData[i];
            }
            if(nMax < pshData[i])
            {
                nMax = pshData[i];
            }
        }
        break;
    case VS_RGB8_32:
        pnData = (int*)m_pImage->Data();
        nMin = nMax = pnData[0];
        for(i=1; i<nSize; i++)
        {
            if(nMin > pnData[i])
            {
                nMin = pnData[i];
            }
            if(nMax < pnData[i])
            {
                nMax = pnData[i];
            }
        }
        break;
    default:
        return FALSE;
    }
    
    if(nMax == nMin)
    {
        if(nMax > 0)
        {
            fltMul = 127.0f / (float)nMax;
            nBrig = 255;
        }
        else if(nMax < 0)
        {
            fltMul = -127.0f / (float)nMax;
            nBrig = 255;
        }
        else
        {
            fltMul = 1.0f;
            nBrig = 127;
        }
    }
    else if(nMax <= 0)
    {
        fltMul = -255.0f / (float)nMin;
        nBrig = 255;
    }
    else if(nMin >= 0)
    {
        fltMul = 255.0f / (float)nMax;
        nBrig = 0;
    }
    else
    {
        if(abs(nMin) < nMax)
        {
            fltMul = 127.0f / nMax;
        }
        else
        {
            fltMul = -127.0f / nMin;
        }
        nBrig = 128;
    }

    S_Adjust Adjust;
    GetAdjust(&Adjust);
    Adjust.fltMinMult = 0.0f;
    Adjust.fltMult = fltMul;
    Adjust.fltMaxMult = fltMul * 2.0f;
    Adjust.fltStepMult = fltMul * 2.0f / 100.0f;
    Adjust.nBrightness = nBrig;
    SetAdjust(&Adjust);
    
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_BitmapEx::CreateAVIWriter(char *szFileName)
{
	BITMAPINFOHEADER BIH;

	ZeroMemory(&BIH, sizeof(BITMAPINFOHEADER));
	BIH.biSize = sizeof(BITMAPINFOHEADER);
	BIH.biWidth = Width() << m_nScale;
	BIH.biHeight = Height() << m_nScale;
	BIH.biPlanes = 1;
	BIH.biBitCount = 24;
	if(BIH.biWidth & 0x3)
	{
		BIH.biSizeImage = (BIH.biWidth + (4 - (BIH.biWidth & 0x3))) * BIH.biHeight * 3;
	}
	else
	{
		BIH.biSizeImage = BIH.biWidth * BIH.biHeight * 3;
	}

    DeleteAVIWriter();
    if((m_pAVIW = new C_AVIWriter) == NULL)
    {
        return FALSE;
    }
    if(!m_pAVIW->Open(szFileName, &BIH))
    {
        DeleteAVIWriter();
        return FALSE;
    }
    
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_BitmapEx::DeleteAVIWriter()
{
    if(m_pAVIW)
    {
        delete m_pAVIW;
        m_pAVIW = NULL;
    }

    return TRUE;
}
//------------------------------------------------------------------------
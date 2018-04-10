    //--------------------------------------------------------------------
#include <Windows.h>
#include <math.h>
#include "VShell.h"
#include "Image.h"

    //--------------------------------------------------------------------
C_ImageRGB1::C_ImageRGB1()
{
    m_pBIH = NULL;
    m_nWidth = 0;
    m_nLineWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 1;
    m_nType = VS_RGB1;
    m_nBufSize = 0;
    m_pbData = NULL;
    m_pbAdjustData = NULL;
    m_Adjust.fAdjust = FALSE;
    m_Adjust.fltMinMult = 0.0f;
    m_Adjust.fltMaxMult = 1.0f;
    m_Adjust.fltStepMult = 0.01f;
    m_Adjust.fltMult = 1.0f;
    m_Adjust.nBrightness = 0;
    m_Adjust.nContrast = 0;
}

    //--------------------------------------------------------------------
C_ImageRGB1::~C_ImageRGB1()
{
    if(m_pBIH)
    {
        GlobalFree((HGLOBAL)m_pBIH);
    }
    delete [] m_pbData;
    delete [] m_pbAdjustData;
}

    //--------------------------------------------------------------------
BOOL C_ImageRGB1::Init(int nWidth, int nHeight, LPRGBQUAD prgbPal)
{
    int i;

    if(m_pBIH)
    {
        GlobalFree(m_pBIH);
    }
    
    if((m_pBIH = (PBITMAPINFOHEADER)GlobalAlloc(GPTR, 
            sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2)) == NULL)
    {
        return FALSE;
    }

    LPRGBQUAD pBIHPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));

    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nLineWidth = ((m_nWidth + 31) & ~31) >> 3;

    if(nWidth & 0x7)
    {
        m_nBufSize = (nWidth + 1) * m_nHeight;
    }
    else
    {
        m_nBufSize = nWidth * m_nHeight;
    }
    m_nBufSize >>= 3;

#ifdef __DEMO__
    for(i=0; i<2; i++)
    {
        pBIHPal[i].rgbBlue = i * 255;
        pBIHPal[i].rgbGreen = i * 255;
        pBIHPal[i].rgbRed = i * 255;
        pBIHPal[i].rgbReserved = 0;
    }
#else
    if(prgbPal)
    {
        memcpy(pBIHPal, prgbPal, sizeof(RGBQUAD) * 2);
    }
    else
    {
        for(i=0; i<2; i++)
        {
            pBIHPal[i].rgbBlue = (BYTE)(i * 255);
            pBIHPal[i].rgbGreen = (BYTE)(i * 255);
            pBIHPal[i].rgbRed = (BYTE)(i * 255);
            pBIHPal[i].rgbReserved = 0;
        }
    }
#endif

    m_pBIH->biSize = sizeof(BITMAPINFOHEADER);
    m_pBIH->biPlanes = 1;
    m_pBIH->biWidth = nWidth;
    m_pBIH->biHeight = -m_nHeight;
	m_pBIH->biBitCount = (WORD)m_nBitCount;
    m_pBIH->biSizeImage = m_nHeight * m_nLineWidth;
    
    if((m_pbData = new BYTE[m_pBIH->biSizeImage]) == NULL)
    {
        return FALSE;
    }
    ZeroMemory(m_pbData, m_pBIH->biSizeImage);

    if((m_pbAdjustData = new BYTE[m_pBIH->biSizeImage]) == NULL)
    {
        return FALSE;
    }
    ZeroMemory(m_pbAdjustData, m_pBIH->biSizeImage);

    return TRUE;
}

    //--------------------------------------------------------------------
BOOL C_ImageRGB1::SetData(void *pvData)
{
    if(!pvData || !m_pBIH || !m_pbData)
    {
        return FALSE;
    }

    if(m_nWidth & 0x1F)
    {
        //BYTE *pbData = (BYTE*)pvData;
        int k = m_nWidth >> 3;
        if(m_nWidth & 0x7)
        {
            k++;
        }
        for(int i=0; i<m_nHeight; i++)
        {
            memcpy(&m_pbData[i*m_nLineWidth], &((BYTE*)pvData)[i*k], k);
        }
    }
    else
    {
        memcpy(m_pbData, pvData, m_pBIH->biSizeImage);
    }

    return TRUE;
}

    //--------------------------------------------------------------------
BOOL C_ImageRGB1::GetData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }

    if(m_nWidth & 0x1F)
    {
        //BYTE *pbData = (BYTE*)pvData;
        int k = m_nWidth >> 3;
        if(m_nWidth & 0x7)
        {
            k++;
        }
        for(int i=0; i<m_nHeight; i++)
        {
            memcpy(&((BYTE*)pvData)[i*k], &m_pbData[i*m_nLineWidth], k);
        }
    }
    else
    {
        memcpy(pvData, m_pbData, m_pBIH->biSizeImage);
    }

    return TRUE;
}

    //--------------------------------------------------------------------
BOOL C_ImageRGB1::GetGrayData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }

    BYTE pbTab[2], bVal;
    BYTE *pbDstData = (BYTE*)pvData;
    int i, j, k, m, n;
    LPRGBQUAD pPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));

    pbTab[0] = RGB2Y(pPal[0].rgbRed, pPal[0].rgbGreen, pPal[0].rgbBlue);
    pbTab[1] = RGB2Y(pPal[1].rgbRed, pPal[1].rgbGreen, pPal[1].rgbBlue);
    k = m_nWidth >> 3;
    for(i=0; i<m_nHeight; i++)
    {
        m = m_nWidth;
        for(j=0; j<k; j++, m-=8)
        {
            bVal = m_pbData[i * m_nLineWidth + j];
            for(n=0; n<8; n++)
            {
                *pbDstData++ = pbTab[(bVal >> 7) & 1];
                bVal <<= 1;
            }
        }
        bVal = m_pbData[i * m_nLineWidth + j];
        for(j=0; j<m; j++)
        {
            *pbDstData++ = pbTab[(bVal >> 7) & 1];
            bVal <<= 1;
        }
    }

    return TRUE;
}

    //--------------------------------------------------------------------
BOOL C_ImageRGB1::GetBGRData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }

    BYTE bVal;
    BYTE *pbDstData = (BYTE*)pvData;
    int i, j, k, m, n;
    RGBQUAD rgbQuad;
    LPRGBQUAD pPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));

    k = m_nWidth >> 3;
    for(i=0; i<m_nHeight; i++)
    {
        m = m_nWidth;
        for(j=0; j<k; j++, m-=8)
        {
            bVal = m_pbData[i * m_nLineWidth + j];
            for(n=0; n<8; n++)
            {
                rgbQuad = pPal[(bVal >> 7) & 1];
                *pbDstData++ = rgbQuad.rgbBlue;
                *pbDstData++ = rgbQuad.rgbGreen;
                *pbDstData++ = rgbQuad.rgbRed;
                bVal <<= 1;
            }
        }
        bVal = m_pbData[i * m_nLineWidth + j];
        for(j=0; j<m; j++)
        {
            rgbQuad = pPal[(bVal >> 7) & 1];
            *pbDstData++ = rgbQuad.rgbBlue;
            *pbDstData++ = rgbQuad.rgbGreen;
            *pbDstData++ = rgbQuad.rgbRed;
            bVal <<= 1;
        }
    }
    
    return TRUE;
}

    //--------------------------------------------------------------------
DWORD C_ImageRGB1::GetPixelValue(int nX, int nY)
{
    if(!m_pbData || !m_pBIH || (DWORD)nX >= (DWORD)m_nWidth || (DWORD)nY >= (DWORD)m_nHeight)
    {
        return 0;
    }

    BYTE bPix;
    bPix = m_pbData[(nY * m_nLineWidth + nX) >> 3];
    nX = 7 - (nX & 7);
    return (DWORD)((bPix >> nX) & 1);
}

    //--------------------------------------------------------------------
BOOL C_ImageRGB1::SetPalette(RGBQUAD *prgbPal)
{
#ifndef __DEMO__
    if(!m_pBIH)
    {
        return FALSE;
    }

    LPRGBQUAD pBIHPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));
    if(prgbPal)
    {
        memcpy(pBIHPal, prgbPal, sizeof(RGBQUAD) * 2);
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            pBIHPal[i].rgbBlue = (BYTE)(i * 255);
            pBIHPal[i].rgbGreen = (BYTE)(i * 255);
            pBIHPal[i].rgbRed = (BYTE)(i * 255);
            pBIHPal[i].rgbReserved = 0;
        }
    }
#endif
    return TRUE;
}

    //--------------------------------------------------------------------
BOOL C_ImageRGB1::GetPalette(RGBQUAD *prgbPal)
{
    if(!prgbPal || !m_pBIH)
    {
        return FALSE;
    }
    LPRGBQUAD pBIHPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));
    memcpy(prgbPal, pBIHPal, sizeof(RGBQUAD) * 2);
    return TRUE;
}

    //--------------------------------------------------------------------
BOOL C_ImageRGB1::GetHistogram(int *pnHistogram, int nHistoHeight)
{
    if(!pnHistogram || !m_pbData)
    {
        return FALSE;
    }

    int i, j, k, m, n;
    BYTE bVal;
    float pfltFreq[2], fltTmp;
    BYTE *pbData = Adjust(NULL, NULL, NULL, NULL);

    if(!pbData)
    {
        return FALSE;
    }

    k = m_nWidth >> 3;

    memset(pfltFreq, 0, sizeof(float) * 2);
    for(i=0; i<m_nHeight; i++)
    {
        m = m_nWidth;
        for(j=0; j<k; j++, m-=8)
        {
            bVal = pbData[i * m_nLineWidth + j];
            for(n=0; n<8; n++)
            {
                pfltFreq[bVal & 1] += 1.0f;
                bVal >>= 1;
            }
        }
        bVal = pbData[i * m_nLineWidth + j];
        for(j=0; j<m; j++)
        {
            pfltFreq[(bVal >> 7) & 1] += 1.0f;
            bVal <<= 1;
        }
    }

    fltTmp = (float)(nHistoHeight - 1);
    if(pfltFreq[0] > pfltFreq[1])
    {
        fltTmp /= pfltFreq[0];
    }
    else
    {
        fltTmp /= pfltFreq[1];    
    }

    pnHistogram[0] = (int)(pfltFreq[0] * fltTmp + 0.5f);
    pnHistogram[1] = (int)(pfltFreq[1] * fltTmp + 0.5f);

    return TRUE;
}

    //--------------------------------------------------------------------
BYTE *C_ImageRGB1::Adjust(int *pnLowOverflows, POINT *, int *pnHiOverflows, POINT *)
{
    if(pnLowOverflows)
    {
        *pnLowOverflows = 0;
    }
    if(pnHiOverflows)
    {
        *pnHiOverflows = 0;
    }

    if(!m_pbData)
    {
        return NULL;
    }

    if(!m_Adjust.fAdjust || !m_Adjust.nBrightness)
    {
        return m_pbData;
    }

    if(m_Adjust.nBrightness > 0)
    {
        memset(m_pbAdjustData, 0xFF, m_pBIH->biSizeImage);
    }
    else
    {
        memset(m_pbAdjustData, 0, m_pBIH->biSizeImage);
    }

    return m_pbAdjustData;
}

    //--------------------------------------------------------------------
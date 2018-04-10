//------------------------------------------------------------------------
#include <Windows.h>
#include <math.h>
#include "VShell.h"
#include "Image.h"
//------------------------------------------------------------------------
C_ImageRGB4::C_ImageRGB4()
{
    m_pBIH = NULL;
    m_nWidth = 0;
    m_nLineWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 4;
    m_nType = VS_RGB4;
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
//------------------------------------------------------------------------
C_ImageRGB4::~C_ImageRGB4()
{
    if(m_pBIH)
    {
        GlobalFree((HGLOBAL)m_pBIH);
    }
    delete [] m_pbData;
    delete [] m_pbAdjustData;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB4::Init(int nWidth, int nHeight, LPRGBQUAD prgbPal)
{
    int i;

    if(m_pBIH)
    {
        GlobalFree(m_pBIH);
    }
    
    if((m_pBIH = (PBITMAPINFOHEADER)GlobalAlloc(GPTR, 
            sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 16)) == NULL)
    {
        return FALSE;
    }

    LPRGBQUAD pBIHPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));

    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nLineWidth = ((m_nWidth + 7) & ~7) >> 1;
    
    if(nWidth & 0x1)
    {
        m_nBufSize = (nWidth + 1) * nHeight;
    }
    else
    {
        m_nBufSize = nWidth * nHeight;
    }
    m_nBufSize >>= 1;

#ifdef __DEMO__
    for(i=0; i<16; i++)
    {
        pBIHPal[i].rgbBlue = i * 17;
        pBIHPal[i].rgbGreen = i * 17;
        pBIHPal[i].rgbRed = i * 17;
        pBIHPal[i].rgbReserved = 0;
    }
#else
    if(prgbPal)
    {
        memcpy(pBIHPal, prgbPal, sizeof(RGBQUAD) * 16);
    }
    else
    {
        for(i=0; i<16; i++)
        {
            pBIHPal[i].rgbBlue = (BYTE)(i * 17);
            pBIHPal[i].rgbGreen = (BYTE)(i * 17);
            pBIHPal[i].rgbRed = (BYTE)(i * 17);
            pBIHPal[i].rgbReserved = 0;
        }
    }
#endif

    m_pBIH->biSize = sizeof(BITMAPINFOHEADER);
    m_pBIH->biPlanes = 1;
    m_pBIH->biHeight = -m_nHeight;
    m_pBIH->biWidth = m_nWidth;
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
//------------------------------------------------------------------------
BOOL C_ImageRGB4::SetData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }

    if(m_nWidth & 0x3)
    {
        //BYTE *pbData = (BYTE*)pvData;
        int k = m_nWidth >> 1;
        k += (m_nWidth & 0x1);
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
//------------------------------------------------------------------------
BOOL C_ImageRGB4::GetData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }

    if(m_nWidth & 0x3)
    {
        //BYTE *pbData = (BYTE*)pvData;
        int k = m_nWidth >> 1;
        k += (m_nWidth & 0x1);
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
//------------------------------------------------------------------------
BOOL C_ImageRGB4::GetGrayData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }

    BYTE pbTab[16], bVal;
    BYTE *pbDstData = (BYTE*)pvData;
    int i, j, k;
    LPRGBQUAD pPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));

    for(i=0; i<16; i++)
    {
        pbTab[i] = RGB2Y(pPal[i].rgbRed, pPal[i].rgbGreen, pPal[i].rgbBlue);
    }
    k = m_nWidth >> 1;
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<k; j++)
        {
            bVal = m_pbData[i * m_nLineWidth + j];
            *pbDstData++ = pbTab[bVal >> 4];
            *pbDstData++ = pbTab[bVal & 0xF];
        }
        if(m_nWidth & 1)
        {
            *pbDstData++ = pbTab[m_pbData[i * m_nLineWidth + j] >> 4];
        }
    }

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB4::GetBGRData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }

    BYTE bVal;
    BYTE *pbDstData = (BYTE*)pvData;
    int i, j, k;
    RGBQUAD rgbQuad;
    LPRGBQUAD pPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));

    k = m_nWidth >> 1;
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<k; j++)
        {
            bVal = m_pbData[i * m_nLineWidth + j];
            rgbQuad = pPal[bVal >> 4];
            *pbDstData++ = rgbQuad.rgbBlue;
            *pbDstData++ = rgbQuad.rgbGreen;
            *pbDstData++ = rgbQuad.rgbRed;
            rgbQuad = pPal[bVal & 0xF];
            *pbDstData++ = rgbQuad.rgbBlue;
            *pbDstData++ = rgbQuad.rgbGreen;
            *pbDstData++ = rgbQuad.rgbRed;
        }
        if(m_nWidth & 1)
        {
            rgbQuad = pPal[m_pbData[i * m_nLineWidth + j] >> 4];
            *pbDstData++ = rgbQuad.rgbBlue;
            *pbDstData++ = rgbQuad.rgbGreen;
            *pbDstData++ = rgbQuad.rgbRed;
        }
    }
    
    return TRUE;
}
//------------------------------------------------------------------------
DWORD C_ImageRGB4::GetPixelValue(int nX, int nY)
{
    if(!m_pbData || !m_pBIH || (DWORD)nX >= (DWORD)m_nWidth || (DWORD)nY >= (DWORD)m_nHeight)
    {
        return 0;
    }

    BYTE bPix;

    bPix = m_pbData[(nY * m_nLineWidth + nX) >> 1];
    if(nX & 1)
    {
        return (DWORD)(bPix & 0xF);
    }
    
    return (DWORD)(bPix >> 4);
}
//------------------------------------------------------------------------
BOOL C_ImageRGB4::SetPalette(RGBQUAD *prgbPal)
{
#ifndef __DEMO__
    if(!m_pBIH)
    {
        return FALSE;
    }

    LPRGBQUAD pBIHPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));
    if(prgbPal)
    {
        memcpy(pBIHPal, prgbPal, sizeof(RGBQUAD) * 16);
    }
    else
    {
        for(int i=0; i<16; i++)
        {
            pBIHPal[i].rgbBlue = (BYTE)(i * 17);
            pBIHPal[i].rgbGreen = (BYTE)(i * 17);
            pBIHPal[i].rgbRed = (BYTE)(i * 17);
            pBIHPal[i].rgbReserved = 0;
        }
    }
#endif
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB4::GetPalette(RGBQUAD *prgbPal)
{
    if(!prgbPal || !m_pBIH)
    {
        return FALSE;
    }

    LPRGBQUAD pBIHPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));
    memcpy(prgbPal, pBIHPal, sizeof(RGBQUAD) * 16);
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB4::GetHistogram(int *pnHistogram, int nHistoHeight)
{
    if(!pnHistogram || !m_pbData)
    {
        return FALSE;
    }

    int i, j, nW;
    float pfltFreq[16], fltTmp;
    BYTE *pbData = Adjust(NULL, NULL, NULL, NULL);
    
    if(!pbData)
    {
        return FALSE;    
    }

    nW = m_nWidth >> 1;

    memset(pfltFreq, 0, sizeof(float) * 16);
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<nW; j++)
        {
            pfltFreq[pbData[i*m_nLineWidth+j] & 0xF] += 1.0;
            pfltFreq[pbData[i*m_nLineWidth+j] >> 4] += 1.0;
        }
        if(m_nWidth & 1)
        {
            pfltFreq[m_pbData[i*m_nLineWidth+j] >> 4] += 1.0;
        }
    }

    fltTmp = 0.0f;
    for(i=0; i<16; i++)
    {
        if(fltTmp < pfltFreq[i])
        {
            fltTmp = pfltFreq[i];
        }
    }
    fltTmp = (float)(nHistoHeight - 1) / fltTmp;
    for(i=0; i<16; i++)
    {
        pnHistogram[i] = (int)(pfltFreq[i] * fltTmp + 0.5f);
    }

    return TRUE;
}

//------------------------------------------------------------------------
BYTE *C_ImageRGB4::Adjust(int *pnLowOverflows, POINT *, 
        int *pnHiOverflows, POINT *)
{
    if(!m_pbData || !m_pBIH)
    {
        return NULL;
    }

    int nBrightness = m_Adjust.nBrightness / 16;
    int nContrast = m_Adjust.nContrast / 16;

    if(!m_Adjust.fAdjust || (!nContrast && !nBrightness))
    {
        return m_pbData;
    }

    int i, j, nW;
    BYTE pbTab[16];
	double dVal, dK;

    if(pnLowOverflows)
    {
        *pnLowOverflows = 0;
    }
    if(pnHiOverflows)
    {
        *pnHiOverflows = 0;
    }
    
    nW = (m_nWidth >> 1) + (m_nWidth & 1);

    dK = 1.0 / 7.0 * ((double)nContrast + 7.0);
    for(i=0; i<16; i++)
	{
        dVal = dK * (double)i + nBrightness; 		    
		if(dVal < 0)
        {
			pbTab[i] = 0;
        }
		else if(dVal > 15.0)
        {
			pbTab[i] = 15;
        }
        else
        {
            pbTab[i] = (BYTE)dVal;
        }
	}

    BYTE bVal1, bVal2;
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<nW; j++)
        {
            bVal1 = pbTab[m_pbData[i*m_nLineWidth+j] >> 4];
            bVal2 = pbTab[m_pbData[i*m_nLineWidth+j] & 0xF];
            m_pbAdjustData[i*m_nLineWidth+j] = (bVal1 << 4) | bVal2;
        }
    }

    return m_pbAdjustData;
}
//------------------------------------------------------------------------
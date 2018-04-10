//------------------------------------------------------------------------
#include <Windows.h>
#include <math.h>
#include "VShell.h"
#include "Image.h"
//------------------------------------------------------------------------
C_ImageRGB8_8::C_ImageRGB8_8()
{
    m_pBIH = NULL;
    m_nWidth = 0;
    m_nLineWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 8;
    m_nType = VS_RGB8_8;
    m_nBufSize = 0;
    m_pbData = NULL;
    m_pcData = NULL;
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
C_ImageRGB8_8::~C_ImageRGB8_8()
{
    if(m_pBIH)
    {
        GlobalFree((HGLOBAL)m_pBIH);
    }
    delete [] m_pbData;
    delete [] m_pcData;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB8_8::Init(int nWidth, int nHeight, LPRGBQUAD prgbPal)
{
    int i;

    if(m_pBIH)
    {
        GlobalFree(m_pBIH);
    }
    
    if((m_pBIH = (PBITMAPINFOHEADER)GlobalAlloc(GPTR, 
            sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256)) == NULL)
    {
        return FALSE;
    }

    LPRGBQUAD pBIHPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));

    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nBufSize = nWidth * nHeight;
    m_nLineWidth = (m_nWidth + 3) & ~3;

    if(prgbPal)
    {
        memcpy(pBIHPal, prgbPal, sizeof(RGBQUAD) * 256);
    }
    else
    {
        for(i=0; i<256; i++)
        {
            pBIHPal[i].rgbBlue = (BYTE)i;
            pBIHPal[i].rgbGreen = (BYTE)i;
            pBIHPal[i].rgbRed = (BYTE)i;
            pBIHPal[i].rgbReserved = 0;
        }
    }
    delete [] m_pcData;
    if((m_pcData = new char[m_nWidth * m_nHeight]) == NULL)
    {
        return FALSE;
    }
    ZeroMemory(m_pcData, m_nWidth * m_nHeight);

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

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB8_8::SetData(void *pvData)
{
    if(!pvData || !m_pcData || !m_pBIH)
    {
        return FALSE;
    }

    memcpy(m_pcData, pvData, m_nBufSize);

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB8_8::GetData(void *pvData)
{
    if(!pvData || !m_pcData || !m_pBIH)
    {
        return FALSE;
    }

    memcpy(pvData, m_pcData, m_nBufSize);

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB8_8::GetGrayData(void *pvData)
{
    if(!pvData || !m_pcData || !m_pBIH)
    {
        return FALSE;
    }
    
    BYTE pbTab[256];
    BYTE *pbDstData = (BYTE*)pvData;
    int i, j;
    LPRGBQUAD pPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));

    for(i=0; i<256; i++)
    {
        pbTab[i] = RGB2Y(pPal[i].rgbRed, pPal[i].rgbGreen, pPal[i].rgbBlue);
    }
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<m_nWidth; j++)
        {
            *pbDstData++ = pbTab[(BYTE)m_pcData[i * m_nWidth + j] ^ 0x80];
        }
    }

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB8_8::GetBGRData(void *pvData)
{
    if(!pvData || !m_pcData || !m_pBIH)
    {
        return FALSE;
    }

    BYTE *pbDstData = (BYTE*)pvData;
    int i, j;
    RGBQUAD rgbQuad;
    LPRGBQUAD pPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));
    
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<m_nWidth; j++)
        {
            rgbQuad = pPal[(BYTE)m_pcData[i * m_nWidth + j] ^ 0x80];
            *pbDstData++ = rgbQuad.rgbBlue;
            *pbDstData++ = rgbQuad.rgbGreen;
            *pbDstData++ = rgbQuad.rgbRed;
        }
    }

    return TRUE;
}
//------------------------------------------------------------------------
DWORD C_ImageRGB8_8::GetPixelValue(int nX, int nY)
{
    if(!m_pcData || (DWORD)nX >= (DWORD)m_nWidth || (DWORD)nY >= (DWORD)m_nHeight)
    {
        return 0;
    }

    return (DWORD)m_pcData[nY * m_nWidth + nX];
}
//------------------------------------------------------------------------
BOOL C_ImageRGB8_8::SetPalette(RGBQUAD *prgbPal)
{
    if(!m_pBIH)
    {
        return FALSE;
    }

    LPRGBQUAD pBIHPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));
    if(prgbPal)
    {
        memcpy(pBIHPal, prgbPal, sizeof(RGBQUAD) * 256);
    }
    else
    {
        for(int i=0; i<256; i++)
        {
            pBIHPal[i].rgbBlue = (BYTE)i;
            pBIHPal[i].rgbGreen = (BYTE)i;
            pBIHPal[i].rgbRed = (BYTE)i;
            pBIHPal[i].rgbReserved = 0;
        }
    }

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB8_8::GetPalette(RGBQUAD *prgbPal)
{
    if(!prgbPal || !m_pBIH)
    {
        return FALSE;
    }

    LPRGBQUAD pBIHPal = (LPRGBQUAD)((PBYTE)m_pBIH + sizeof(BITMAPINFOHEADER));
    memcpy(prgbPal, pBIHPal, sizeof(RGBQUAD) * 256);

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB8_8::GetHistogram(int *pnHistogram, int nHistoHeight)
{
    if(!pnHistogram || !m_pbData)
    {
        return FALSE;
    }

    float pfltFreq[256], fltTmp;
    int i, j;
    BYTE *pbData = Adjust(NULL, NULL, NULL, NULL);

    if(!pbData)
    {
        return FALSE;
    }

    memset(pfltFreq, 0, sizeof(float) * 256);
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<m_nWidth; j++)
        {
            pfltFreq[pbData[i*m_nLineWidth+j]] += 1.0;
        }
    }
    fltTmp = 0.0f;
    for(i=0; i<256; i++)
    {
        if(fltTmp < pfltFreq[i])
        {
            fltTmp = pfltFreq[i];
        }
    }
    fltTmp = (float)(nHistoHeight - 1) / fltTmp;
    for(i=0; i<256; i++)
    {
        pnHistogram[i] = (int)(pfltFreq[i] * fltTmp + 0.5f);
    }

    return TRUE;
}
//------------------------------------------------------------------------
BYTE *C_ImageRGB8_8::Adjust(int *pnLowOverflows, POINT *, 
        int *pnHiOverflows, POINT *)
{
    if(!m_pbData || !m_pBIH)
    {
        return NULL;
    }

    int i, j;
    int nVal;

    if(pnLowOverflows)
    {
        *pnLowOverflows = 0;
    }
    if(pnHiOverflows)
    {
        *pnHiOverflows = 0;
    }

    if(!m_Adjust.fAdjust || ((m_Adjust.fltMult == 1.0f) && !m_Adjust.nBrightness))
    {
        for(i=0; i<m_nHeight; i++)
        {
            for(j=0; j<m_nWidth; j++)
            {
                m_pbData[i * m_nLineWidth + j] = m_pcData[i * m_nWidth + j] ^ 0x80;
            }
        }
    }
    else
    {
        for(i=0; i<m_nHeight; i++)
        {
            for(j=0; j<m_nWidth; j++)
            {
                nVal = (int)((float)m_pcData[i*m_nWidth+j] * m_Adjust.fltMult) + m_Adjust.nBrightness;

                if(nVal < -128)
                {
                    m_pbData[i*m_nLineWidth+j] = 0;
                }
                else if(nVal > 127)
                {
                    m_pbData[i*m_nLineWidth+j] = 255;
                }
                else
                {
                    m_pbData[i*m_nLineWidth+j] = (BYTE)nVal ^ 0x80;
                }
            }
        }
    }

    return m_pbData;
}
//------------------------------------------------------------------------
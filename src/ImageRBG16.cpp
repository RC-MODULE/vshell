//------------------------------------------------------------------------
#include <Windows.h>
#include <math.h>
#include "VShell.h"
#include "Image.h"
//------------------------------------------------------------------------
C_ImageRGB16::C_ImageRGB16()
{
    m_pBIH = NULL;
    m_nWidth = 0;
    m_nLineWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 16;
    m_nType = VS_RGB16;
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
C_ImageRGB16::~C_ImageRGB16()
{
    if(m_pBIH)
    {
        GlobalFree((HGLOBAL)m_pBIH);
    }
    delete [] m_pbData;
    delete [] m_pbAdjustData;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB16::Init(int nWidth, int nHeight, LPRGBQUAD )
{
    if(m_pBIH)
    {
        GlobalFree(m_pBIH);
    }
    
    if((m_pBIH = (PBITMAPINFOHEADER)GlobalAlloc(GPTR, sizeof(BITMAPINFOHEADER))) == NULL)
    {
        return FALSE;
    }

    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nLineWidth = ((m_nWidth + 1) & ~1) * 2;
    m_nBufSize = nWidth * nHeight * 2;

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
BOOL C_ImageRGB16::SetData(void *pvData)
{
    if(!pvData || !m_pBIH || !m_pbData)
    {
        return FALSE;
    }

    if(m_nWidth & 1)
    {
        BYTE *pbData = (BYTE*)pvData;
        int nW = m_nWidth * 2;
        for(int i=0; i<m_nHeight; i++)
        {
            memcpy(&m_pbData[i*m_nLineWidth], &pbData[i*nW], nW);        
        }
    }
    else
    {
        memcpy(m_pbData, pvData, m_pBIH->biSizeImage);
    }

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB16::GetData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }

    if(m_nWidth & 1)
    {
        BYTE *pbData = (BYTE*)pvData;
        int nW = m_nWidth * 2;
        for(int i=0; i<m_nHeight; i++)
        {
            memcpy(&pbData[i*nW], &m_pbData[i*m_nLineWidth], nW);        
        }
    }
    else
    {
        memcpy(pvData, m_pbData, m_pBIH->biSizeImage);
    }

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB16::GetGrayData(void *pvData)
{
    if(!pvData || !m_pbData)
    {
        return FALSE;
    }
    
    BYTE *pbDstData = (BYTE*)pvData;
    WORD wVal, *pwSrcData = (WORD*)m_pbData;
    int i, j, k;

    k = m_nLineWidth >> 1;
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<m_nWidth; j++)
        {
            wVal = pwSrcData[i * k + j];
            *pbDstData++ = RGB2Y((BYTE)((wVal & 0x7C00) >> 7), (BYTE)((wVal & 0x3E0) >> 2), (BYTE)((wVal & 0x1F) << 3));
        }
    }

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB16::GetBGRData(void *pvData)
{
    if(!pvData || !m_pbData)
    {
        return FALSE;
    }

    BYTE *pbDstData = (BYTE*)pvData;
    WORD wVal, *pwSrcData = (WORD*)m_pbData;
    int i, j, k;

    k = m_nLineWidth >> 1;
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<m_nWidth; j++)
        {
            wVal = pwSrcData[i * k + j];
            *pbDstData++ = (BYTE)((wVal & 0x1F) << 3);
            *pbDstData++ = (BYTE)((wVal & 0x3E0) >> 2);
            *pbDstData++ = (BYTE)((wVal & 0x7C00) >> 7);
        }
    }
    
    return TRUE;
}
//------------------------------------------------------------------------
DWORD C_ImageRGB16::GetPixelValue(int nX, int nY)
{
    if(!m_pbData || !m_pBIH || (DWORD)nX >= (DWORD)m_nWidth || (DWORD)nY >= (DWORD)m_nHeight)
    {
        return 0;
    }

    WORD *pshData = (WORD*)m_pbData;
    return (DWORD)pshData[nY * m_nLineWidth / 2 + nX];
}
//------------------------------------------------------------------------
BOOL C_ImageRGB16::SetPalette(RGBQUAD *)
{
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB16::GetPalette(RGBQUAD *)
{
    return FALSE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB16::GetHistogram(int *pnHistogram, int nHistoHeight)
{
    if(!pnHistogram || !m_pbData)
    {
        return FALSE;
    }

    float pfltFreq[256], fltTmp;
    int nW, nVal, i, j;
    WORD *pshData = (WORD*)Adjust(NULL, NULL, NULL, NULL);

    if(!pshData)
    {
        return FALSE;
    }

    nW = m_nLineWidth / 2;
    memset(pfltFreq, 0, sizeof(float) * 256);
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<m_nWidth; j++)
        {
            nVal = pshData[i * nW + j];
            nVal = ((nVal & 0x1F) + ((nVal >> 5) & 0x1F) + ((nVal >> 10) & 0x1F)) / 3;
            pfltFreq[nVal * 8] += 1.0f;
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
BYTE *C_ImageRGB16::Adjust(int *pnLowOverflows, POINT *, 
        int *pnHiOverflows, POINT *)
{
    if(!m_pBIH || !m_pbData)
    {
        return NULL;
    }

    int nBrightness = m_Adjust.nBrightness / 8;
    int nContrast = m_Adjust.nContrast / 8;

    if(!m_Adjust.fAdjust || (!nContrast && !nBrightness))
    {
        return m_pbData;
    }

    int i, j;
    BYTE pbTab[32];
	double dVal, dK;

    if(pnLowOverflows)
    {
        *pnLowOverflows = 0;
    }
    if(pnHiOverflows)
    {
        *pnHiOverflows = 0;
    }
    
    //int nSize = m_nWidth * m_nHeight;

    dK = 1.0 / 15.0 * ((double)nContrast + 15.0);
    for(i=0; i<32; i++)
	{
        dVal = dK * (double)i + nBrightness;
        if(dVal < 0)
        {
			pbTab[i] = 0;
        }
		else if(dVal > 31.0)
        {
			pbTab[i] = 31;
        }
        else
        {
            pbTab[i] = (BYTE)dVal;
        }
	}

    int nW, nVal, nR, nG, nB;
    short *pshSrc = (short*)m_pbData;
    short *pshDst = (short*)m_pbAdjustData;
    nW = m_nLineWidth / 2;
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<m_nWidth; j++)
        {
            nVal = pshSrc[i*nW+j];
            nB = pbTab[nVal & 0x1F];
            nG = pbTab[(nVal >> 5) & 0x1F];
            nR = pbTab[(nVal >> 10) & 0x1F];
            pshDst[i*nW+j] = (short)(nB | (nG << 5) | (nR << 10));
        }
    }

    return m_pbAdjustData;
}
//------------------------------------------------------------------------
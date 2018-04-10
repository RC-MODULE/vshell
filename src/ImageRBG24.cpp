//------------------------------------------------------------------------
#include <Windows.h>
#include <math.h>
#include "VShell.h"
#include "Image.h"
//------------------------------------------------------------------------
C_ImageRGB24::C_ImageRGB24()
{
    m_pBIH = NULL;
    m_nWidth = 0;
    m_nLineWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 24;
    m_nType = VS_RGB24;
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
C_ImageRGB24::~C_ImageRGB24()
{
    if(m_pBIH)
    {
        GlobalFree((HGLOBAL)m_pBIH);
    }
    delete [] m_pbData;
    delete [] m_pbAdjustData;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB24::Init(int nWidth, int nHeight, LPRGBQUAD )
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
    m_nLineWidth = ((m_nWidth * 3) + 3) & ~3;
    m_nBufSize = m_nWidth * m_nHeight * 3;

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
BOOL C_ImageRGB24::SetData(void *pvData)
{
    if(!pvData || !m_pBIH || !m_pbData)
    {
        return FALSE;
    }

    if((m_nWidth * 3) & 0x3)
    {
        BYTE *pbData = (BYTE*)pvData;
        int nW = m_nWidth * 3;
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
BOOL C_ImageRGB24::GetData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }
    
    if((m_nWidth * 3) & 0x3)
    {
        BYTE *pbData = (BYTE*)pvData;
        int nW = m_nWidth * 3;
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
BOOL C_ImageRGB24::GetGrayData(void *pvData)
{
    if(!pvData || !m_pbData || !m_pBIH)
    {
        return FALSE;
    }

    BYTE *pbDstData = (BYTE*)pvData;
    int i, j, k, m;

    for(i=0, k=0; i<m_nHeight; i++, k+=m_nLineWidth)
    {
        for(j=0, m=0; j<m_nWidth; j++, m+=3)
        {
            *pbDstData++ = RGB2Y(m_pbData[k+m+2], m_pbData[k+m+1], m_pbData[k+m]);
        }
    }

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB24::GetBGRData(void *pvData)
{
    return GetData(pvData);
}
//------------------------------------------------------------------------
DWORD C_ImageRGB24::GetPixelValue(int nX, int nY)
{
    if(!m_pbData || !m_pBIH || (DWORD)nX >= (DWORD)m_nWidth || (DWORD)nY >= (DWORD)m_nHeight)
    {
        return 0;
    }

    DWORD dwRes = *(DWORD*)&m_pbData[nY * m_nLineWidth + nX * 3] & 0xFFFFFF;

    return dwRes;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB24::SetPalette(RGBQUAD *)
{
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB24::GetPalette(RGBQUAD *)
{
    return FALSE;
}
//------------------------------------------------------------------------
BOOL C_ImageRGB24::GetHistogram(int *pnHistogram, int nHistoHeight)
{
    if(!pnHistogram || !m_pbData)
    {
        return FALSE;
    }
    
    float pfltFreq[256], fltTmp;
    int nVal, i, j;
    int nW3 = m_nWidth * 3;
    BYTE *pbData = Adjust(NULL, NULL, NULL, NULL);

    if(!pbData)
    {
        return FALSE;
    }

    memset(pfltFreq, 0, sizeof(float) * 256);
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<nW3; j+=3)
        {
            nVal = (pbData[i*m_nLineWidth+j] + pbData[i*m_nLineWidth+j+1] + pbData[i*m_nLineWidth+j+2]) / 3;
            pfltFreq[nVal] += 1.0f;
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
BYTE *C_ImageRGB24::Adjust(int *pnLowOverflows, POINT *, 
        int *pnHiOverflows, POINT *)
{
    if(!m_pBIH || !m_pbData)
    {
        return NULL;
    }

    if(!m_Adjust.fAdjust || (!m_Adjust.nContrast && !m_Adjust.nBrightness))
    {
        return m_pbData;
    }

    int i, j;
    BYTE pbTab[256];
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
    dK = 1.0 / 127.0 * ((double)m_Adjust.nContrast + 127.0);
    for(i=0; i<256; i++)
	{
        dVal = dK * double(i) + m_Adjust.nBrightness;
		if(dVal < 0)
        {
			pbTab[i] = 0;
        }
		else if(dVal > 255.0)
        {
			pbTab[i] = 255;
        }
        else
        {
            pbTab[i] = (BYTE)dVal;
        }
	}

    int nW3 = m_nWidth * 3;
    for(i=0; i<m_nHeight; i++)
    {
        for(j=0; j<nW3; j+=3)
        {
            m_pbAdjustData[i*m_nLineWidth+j] = pbTab[m_pbData[i*m_nLineWidth+j]];
            m_pbAdjustData[i*m_nLineWidth+j+1] = pbTab[m_pbData[i*m_nLineWidth+j+1]];
            m_pbAdjustData[i*m_nLineWidth+j+2] = pbTab[m_pbData[i*m_nLineWidth+j+2]];
        }
    }

    return m_pbAdjustData;
}
//------------------------------------------------------------------------
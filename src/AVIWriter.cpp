    //--------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include "AVIWriter.h"
    //--------------------------------------------------------------------
C_AVIWriter::C_AVIWriter()
{
    m_pAF = NULL;
    m_pGF = NULL;
    m_pAS = NULL;
    m_nSizeImage = 0;
    AVIFileInit();
}
    //--------------------------------------------------------------------
C_AVIWriter::~C_AVIWriter()
{
    Close();
    AVIFileExit();
}
    //--------------------------------------------------------------------
BOOL C_AVIWriter::Open(LPSTR szFileName, LPBITMAPINFOHEADER pBIH)
{
    //AVIFILEINFO AFI;     
    AVISTREAMINFO ASI;
    //AVICOMPRESSOPTIONS ACO;

    int a = AVIFileOpen(&m_pAF, szFileName, OF_WRITE | OF_CREATE, NULL);
    if(a)
    {
        int b = AVIERR_BADFORMAT;
        b = AVIERR_MEMORY;
        b = AVIERR_FILEREAD;
        b = AVIERR_FILEOPEN;
        b = REGDB_E_CLASSNOTREG;
        return FALSE;
    }

        // Create a stream using AVIFileCreateStream. 
    ZeroMemory(&ASI, sizeof(AVISTREAMINFO));
	ASI.fccType                = streamtypeVIDEO;// stream type
	ASI.fccHandler             = 0;
	ASI.dwScale                = 1;
	ASI.dwRate                 = 30;		    // 15 fps
	ASI.dwSuggestedBufferSize  = pBIH->biSizeImage;
	SetRect(&ASI.rcFrame, 0, 0,	pBIH->biWidth, pBIH->biHeight);

    if(AVIFileCreateStream(m_pAF, &m_pAS, &ASI)) 
    {            
            //Stream created OK? If not, close file. 
        Close(); 
        return FALSE; 
    } 
/*
    ZeroMemory(&ACO, sizeof(AVICOMPRESSOPTIONS));
	if(!AVISaveOptions(NULL, 0, 1, &m_pAS, (LPAVICOMPRESSOPTIONS FAR *)&ACO))
	{
		Close(); 
        return FALSE;
	}
*/
 
        // Set format of new stream using AVIStreamSetFormat. 
    if(AVIStreamSetFormat(m_pAS, 0, pBIH, sizeof(BITMAPINFOHEADER))) 
    { 
        Close();
        return FALSE; 
    } 

    m_nSizeImage = pBIH->biSizeImage;

//-----------
/*	if(AVIFileOpen(&m_pAF, szFileName, OF_WRITE | OF_CREATE, NULL))
    {
        return FALSE;
    }
	if(AVIFileInfo(m_pAF, &AFI, sizeof(AVIFILEINFO)))
    {
        return FALSE;
    }
    if(AVIFileGetStream(m_pAF, &m_pAS, streamtypeVIDEO, 0))
    {
        return FALSE;
    }
	if(AVIStreamInfo(m_pAS, &ASI, sizeof(AVISTREAMINFO)))
    {
        return FALSE;
    }
	if((m_pGF = AVIStreamGetFrameOpen(m_pAS, 0)) == NULL)
    {
        return FALSE;
    }
    LPBITMAPINFO pBMPI;
    if((pBMPI = (LPBITMAPINFO)AVIStreamGetFrame(m_pGF, 0)) == NULL)
    {
        return TRUE;
    }

    m_nWidth = pBMPI->bmiHeader.biWidth;
    m_nHeight = pBMPI->bmiHeader.biHeight;
    m_nBitCount = pBMPI->bmiHeader.biBitCount;
    m_nClrUsed = pBMPI->bmiHeader.biClrUsed;
    m_nFirstFrame = 0;
    
    memcpy(m_pPal, pBMPI->bmiColors, sizeof(RGBQUAD) * m_nClrUsed);

    m_nLastFrame = AFI.dwLength - 1;

    m_nPixelStringSize = m_nWidth * (m_nBitCount >> 3);
    m_nRem = (4 - (m_nPixelStringSize & 3)) & 3;
*/
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_AVIWriter::Close()
{
    int a;
    if(m_pGF)
    {
        a = AVIStreamGetFrameClose(m_pGF);
        m_pGF = NULL;
    }
    if(m_pAS)
    {
        a = AVIStreamRelease(m_pAS);
        m_pAS = NULL;
    }
    if(m_pAF)
    {
        a = AVIFileRelease(m_pAF);
        m_pAF = NULL;
    }
	return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_AVIWriter::WriteFrame(BYTE *pbImage)
{
    static int nCnt = 0;

    AVIStreamWrite(m_pAS, nCnt++, 1, pbImage, m_nSizeImage, AVIIF_KEYFRAME, NULL, NULL);
    
    return TRUE;
}
    //--------------------------------------------------------------------

    //--------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include "FrameGrabber.h"
    //--------------------------------------------------------------------
static const int MAX_FRAMES     = 0x7FFFFFFF; 
static const int MAXVIDDRIVERS  = 10; 
static const int MS_FOR_15FPS   = 66;
static HWND hwndPar; 
    //--------------------------------------------------------------------
void C_FGTemplate::GetPalette(RGBQUAD *pPal)
{
    if(pPal)
    {
        memcpy(pPal, m_pPal, sizeof(RGBQUAD) * 256);
    }
}
    //--------------------------------------------------------------------
C_FGBMP::C_FGBMP()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nClrUsed = 0;
    m_nPixelStringSize = 0;
    m_nRem = 0;
    m_nFirstFrame = 0;
    m_nLastFrame = 0;
    m_nOffset = 0;
    m_nNumLength = 0;
    ZeroMemory(m_pPal, sizeof(RGBQUAD) * 256);
    ZeroMemory(m_szFN, _MAX_PATH);
}
    //--------------------------------------------------------------------
C_FGBMP::~C_FGBMP()
{
}
    //--------------------------------------------------------------------
BOOL C_FGBMP::Bind(LPSTR szFileName)
{
    BITMAPFILEHEADER BFH;
    BITMAPINFOHEADER BIH;
    FILE *fp;

    if(!ParseFileName(szFileName))
    {
        return FALSE;
    }

    if((fp = fopen(szFileName, "rb")) == NULL)
    {
        return FALSE;
    }

    fread(&BFH, sizeof(BITMAPFILEHEADER), 1, fp);
    if(BFH.bfType != 'MB')
    {
        return FALSE;
    }


    fread(&BIH, sizeof(BITMAPINFOHEADER), 1, fp);
    if(BIH.biBitCount <= 8 && BIH.biClrUsed == 0) 
    {
        BIH.biClrUsed = 1 << BIH.biBitCount;
    }
    fread(m_pPal, sizeof(RGBQUAD) * BIH.biClrUsed, 1, fp);
    fclose(fp);

    m_nWidth = BIH.biWidth;
    m_nHeight = BIH.biHeight;
    m_nBitCount = BIH.biBitCount;
    m_nClrUsed = BIH.biClrUsed;
    m_nOffset = BFH.bfOffBits;

    switch(BIH.biBitCount)
    {
    case 1:
        m_nPixelStringSize = m_nWidth >> 3;
        if(m_nWidth & 0x7)
        {
            m_nPixelStringSize++;
        }
        break;
    case 4:
        m_nPixelStringSize = (m_nWidth >> 1) + (m_nWidth & 1);
        break;
    case 8:
    case 16:
    case 24:
    case 32:
        m_nPixelStringSize = m_nWidth * (m_nBitCount >> 3);
        break;
    default:
        m_nPixelStringSize = 0;
    }
    m_nRem = (4 - (m_nPixelStringSize & 3)) & 3;
    
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_FGBMP::GetFrame(BYTE *pbFrame, int nFrameNum)
{
    FILE *fp;
    int i;

    if((pbFrame == NULL) || (nFrameNum < m_nFirstFrame) || (nFrameNum > m_nLastFrame))
    {
        return NULL;
    }

    char szCurFileName[_MAX_PATH];

    switch(m_nNumLength)
    {
    case 0:
        sprintf(szCurFileName, "%s.bmp", m_szFN);
        break;
    case 1:
        sprintf(szCurFileName, "%s%d.bmp", m_szFN, nFrameNum);
        break;
    case 2:
        sprintf(szCurFileName, "%s%02d.bmp", m_szFN, nFrameNum);
        break;
    case 3:
        sprintf(szCurFileName, "%s%03d.bmp", m_szFN, nFrameNum);
        break;
    case 4:
        sprintf(szCurFileName, "%s%04d.bmp", m_szFN, nFrameNum);
        break;
    case 5:
        sprintf(szCurFileName, "%s%05d.bmp", m_szFN, nFrameNum);
        break;
    case 6:
        sprintf(szCurFileName, "%s%06d.bmp", m_szFN, nFrameNum);
        break;
    case 7:
        sprintf(szCurFileName, "%s%07d.bmp", m_szFN, nFrameNum);
        break;
    case 8:
        sprintf(szCurFileName, "%s%08d.bmp", m_szFN, nFrameNum);
        break;
    case 9:
        sprintf(szCurFileName, "%s%09d.bmp", m_szFN, nFrameNum);
        break;
    }            
    if((fp = fopen(szCurFileName, "rb")) == NULL)
    {
        return FALSE;
    }
    fseek(fp, m_nOffset, SEEK_SET);
    PBYTE pbCur = pbFrame + m_nPixelStringSize * (m_nHeight - 1);

    if(m_nRem)
    {
        for(i=0; i<m_nHeight; i++)
        {
            fread(pbCur, m_nPixelStringSize, 1, fp);
            fseek(fp, m_nRem, SEEK_CUR);
            pbCur -= m_nPixelStringSize;
        }
    }
    else
    {
        for(i=0; i<m_nHeight; i++)
        {
            fread(pbCur, m_nPixelStringSize, 1, fp);
            pbCur -= m_nPixelStringSize;
        }
    }
    fclose(fp);
   
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_FGBMP::ParseFileName(PSTR szFileName)
{
    int i;

    m_nNumLength = 0;
    for(i=lstrlen(szFileName)-5; i>0; i--)
    {
        if(isdigit(szFileName[i]))
        {
            m_nNumLength++;
        }
        else
        {
            break;
        }
    }
    
    if(m_nNumLength > 9)
    {
        MessageBox(NULL, "Too long number in file name", "VShell Error", MB_OK);
        return FALSE;
    }

    lstrcpy(m_szFN, szFileName);
    m_szFN[++i] = '\0';

    if(m_nNumLength == 0)
    {
        
        return TRUE;
    }

    char szFirstNum[_MAX_PATH];
    ZeroMemory(szFirstNum, _MAX_PATH);
    for(int j=0; i<lstrlen(szFileName)-4; i++, j++)
    {
        szFirstNum[j] = szFileName[i];
    }
    m_nFirstFrame = atoi(szFirstNum); 

    for(i=m_nFirstFrame; i<MAX_FRAMES; i++)
    {
        switch(m_nNumLength)
        {
        case 0:
            sprintf(szFirstNum, "%s.bmp", m_szFN);
            break;
        case 1:
            sprintf(szFirstNum, "%s%d.bmp", m_szFN, i);
            break;
        case 2:
            sprintf(szFirstNum, "%s%02d.bmp", m_szFN, i);
            break;
        case 3:
            sprintf(szFirstNum, "%s%03d.bmp", m_szFN, i);
            break;
        case 4:
            sprintf(szFirstNum, "%s%04d.bmp", m_szFN, i);
            break;
        case 5:
            sprintf(szFirstNum, "%s%05d.bmp", m_szFN, i);
            break;
        case 6:
            sprintf(szFirstNum, "%s%06d.bmp", m_szFN, i);
            break;
        case 7:
            sprintf(szFirstNum, "%s%07d.bmp", m_szFN, i);
            break;
        case 8:
            sprintf(szFirstNum, "%s%08d.bmp", m_szFN, i);
            break;
        case 9:
            sprintf(szFirstNum, "%s%09d.bmp", m_szFN, i);
            break;
        }            
        if(GetFileAttributes(szFirstNum) == 0xFFFFFFFF)
        {
            break;
        }
    }
    m_nLastFrame = m_nFirstFrame + i - 1;
    return TRUE;
}
    //--------------------------------------------------------------------
C_FGAVI::C_FGAVI()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nClrUsed = 0;
    m_nPixelStringSize = 0;
    m_nRem = 0;
    m_nFirstFrame = 0;
    m_nLastFrame = 0;
    ZeroMemory(m_pPal, sizeof(RGBQUAD) * 256);
    m_pAF = NULL;
    m_pGF = NULL;
    m_pAS = NULL;
    AVIFileInit();
}
    //--------------------------------------------------------------------
C_FGAVI::~C_FGAVI()
{
    if(m_pGF)
    {
        AVIStreamGetFrameClose(m_pGF);
    }
    if(m_pAS)
    {
        AVIStreamRelease(m_pAS);
    }
    if(m_pAF)
    {
        AVIFileRelease(m_pAF);
    }
    AVIFileExit();
}
    //--------------------------------------------------------------------
BOOL C_FGAVI::Bind(LPSTR szFileName)
{
    AVIFILEINFO AFI;     
    AVISTREAMINFO ASI;

    if(m_pGF)
    {
        AVIStreamGetFrameClose(m_pGF);
        m_pGF = NULL;
    }
    if(m_pAF)
    {
        AVIFileRelease(m_pAF);
        m_pAF = NULL;
    }

	if(AVIFileOpen(&m_pAF, szFileName, OF_READ, 0L))
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
        return FALSE;
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

    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_FGAVI::GetFrame(BYTE *pbFrame, int nFrameNum)
{
    if(m_pGF)
    {
        int i;
	    LPBITMAPINFO pBMPI = (LPBITMAPINFO)AVIStreamGetFrame(m_pGF, nFrameNum);
	    if(pBMPI)
	    {
            PBYTE pbSrc = (PBYTE)pBMPI + sizeof(BITMAPINFOHEADER) +
                    sizeof(RGBQUAD) * m_nClrUsed;
            PBYTE pbCur = pbFrame + m_nPixelStringSize * (m_nHeight - 1);

            for(i=0; i<m_nHeight; i++)
            {
                memcpy(pbCur, pbSrc, m_nPixelStringSize);
                pbCur -= m_nPixelStringSize;
                pbSrc += (m_nPixelStringSize + m_nRem);
            }
		    return TRUE;
	    }
    }
	return FALSE;
}
    //--------------------------------------------------------------------
LRESULT CALLBACK FrameCallbackProc(HWND hWnd, LPVIDEOHDR)
{
    WORD VideoFormatSize = (WORD)capGetVideoFormatSize(hWnd);
    if(VideoFormatSize==NULL)
    {
        return FALSE;
    }
    BITMAPINFO *lpBmpInfo =(BITMAPINFO*)(new char[VideoFormatSize]);
    ((LPBITMAPINFOHEADER)lpBmpInfo)->biSize = sizeof(BITMAPINFOHEADER);
    if(capGetVideoFormat(hWnd, lpBmpInfo, VideoFormatSize))
    {

        //LPBITMAPINFOHEADER lpBI=(LPBITMAPINFOHEADER)lpBmpInfo; 
    }
    delete lpBmpInfo;

	return 1;
}
    //--------------------------------------------------------------------
LRESULT CALLBACK VideoCallbackProc(HWND, LPVIDEOHDR) 
{
	return 1;
}
    //--------------------------------------------------------------------
C_FGV::C_FGV()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nClrUsed = 0;
    m_nPixelStringSize = 0;
    m_nRem = 0;
    m_nFirstFrame = 0;
    m_nLastFrame = 0;
    ZeroMemory(m_pPal, sizeof(RGBQUAD) * 256);
    m_hwndCap = NULL;
    m_wDeviceIndex = 0;
}
    //--------------------------------------------------------------------
C_FGV::~C_FGV()
{
    if(m_hwndCap)
    {
        DestroyWindow(m_hwndCap);
    }
}
    //--------------------------------------------------------------------
BOOL C_FGV::Bind(LPSTR)
{
	//char	szDeviceName[80]; 
	//char	szDeviceVersion[100]; 
	//WORD    wDriverCount = 0;

    m_hwndCap = capCreateCaptureWindow("Capture Window", 
            WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, 352, 288, hwndPar, 0xffff);

	if(!m_hwndCap)
    {
        return FALSE;
    }

    capSetCallbackOnFrame(m_hwndCap, FrameCallbackProc);
    if(!capDriverConnect(m_hwndCap, 0))
    {
    
        return FALSE;
    }
    capDriverGetCaps(m_hwndCap, &m_CapDriverCaps, sizeof(CAPDRIVERCAPS));
    capSetUserData(m_hwndCap, (long)this);
    capOverlay(m_hwndCap, TRUE);
    capGrabFrame(m_hwndCap);

/*	for(int i=0; i<MAXVIDDRIVERS; i++) 
	{ 
        if(capGetDriverDescription(i, (LPTSTR)szDeviceName, 
			sizeof(szDeviceName)/sizeof(TCHAR), (LPTSTR)szDeviceVersion, 
			sizeof(szDeviceVersion)/sizeof(TCHAR))) 
		{ 

            if(wDriverCount++ == 0) 
            {
                if(capDriverConnect(m_hwndCap, i))
                {
				    m_wDeviceIndex = i;
                }
            }
        }  
    } 

	capOverlay(m_hwndCap, TRUE);

	    // Get capture driver settings and update menu 
    capDriverGetCaps(m_hwndCap, &m_CapDriverCaps, sizeof(CAPDRIVERCAPS)) ; 

	if(m_CapDriverCaps.fHasDlgVideoSource) 
    {
        capDlgVideoSource(m_hwndCap); 
    }

        // Get video format and adjust capture window 
    capGetStatus(m_hwndCap, &m_CapStatus, sizeof(CAPSTATUS)) ; 
    SetWindowPos(m_hwndCap, NULL, 0, 0, m_CapStatus.uiImageWidth, 
                 m_CapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE) ; 
 
        // Start preview by default 
    capPreviewRate(m_hwndCap, MS_FOR_15FPS); 
    capPreview(m_hwndCap, TRUE);  
*/
    m_nWidth = 352;
    m_nHeight = 288;
    m_nBitCount = 16;
    m_nClrUsed = 0;
    m_nPixelStringSize = 320 * 2;
    m_nRem = 0;
    m_nFirstFrame = 0;
    m_nLastFrame = 0x7FFFFFFF;

    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_FGV::GetFrame(BYTE *, int)
{
    capOverlay(m_hwndCap, TRUE);
    capGrabFrame(m_hwndCap);
    return TRUE;
}
    //--------------------------------------------------------------------
C_FrameGrabber::C_FrameGrabber()
{
    m_nCurFrame = 0;
    m_pFG = NULL;
}
    //--------------------------------------------------------------------
C_FrameGrabber::~C_FrameGrabber()
{
    delete m_pFG;
}
    //--------------------------------------------------------------------
BOOL C_FrameGrabber::Bind(HWND hWnd, LPSTR szFileName)
{
    hwndPar = hWnd;
    FILE *fp;
    BOOL fOpen = FALSE;
    
    char szFN[_MAX_PATH + 16];
    ZeroMemory(szFN, sizeof(szFN));
    if(szFileName)
    {
        lstrcpy(szFN, szFileName);
        if(lstrcmp(szFN, "GRABBER"))
        {
            fp = fopen(szFN, "rb");
            if(fp == NULL)
            {
                fOpen = TRUE; 
            }
            else
            {
                fclose(fp);
            }
        }
    }
    else
    {
        fOpen = TRUE;
    }
 
    if(fOpen)
    {
        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        //ZeroMemory(szFN, sizeof(szFN));
		GetCurrentDirectory(sizeof(szFN) - 16, szFN);
		strcat(szFN, "\\*.*");
        ofn.lStructSize         = sizeof(OPENFILENAME);
        ofn.hwndOwner           = hWnd;
        ofn.lpstrFilter         = "BMP & AVI Files  (*.bmp; *.avi)\0*.BMP;*.AVI\0\0";
        ofn.lpstrFile           = szFN;
        ofn.nMaxFile            = _MAX_PATH;
        ofn.nMaxFileTitle       = _MAX_FNAME + _MAX_EXT;
        ofn.Flags               = OFN_FILEMUSTEXIST;
        ofn.lpstrTitle          = "Bind";
		ofn.lpstrInitialDir		= szFN;

		if(!GetOpenFileName(&ofn))
        {
            return FALSE;
        }
    }

    if(m_pFG)
    {
        delete m_pFG;
        m_pFG = NULL;
    }
    
    if(!lstrcmp(szFN, "GRABBER"))
    {
        m_pFG = (C_FGTemplate*)new C_FGV;
    }
    else if((!lstrcmp(&szFN[lstrlen(szFN) - 3], "avi")) ||
        (!lstrcmp(&szFN[lstrlen(szFN) - 3], "AVI")))
    {
        m_pFG = (C_FGTemplate*)new C_FGAVI;
    }
    else if((!lstrcmp(&szFN[lstrlen(szFN) - 3], "bmp")) ||
        (!lstrcmp(&szFN[lstrlen(szFN) - 3], "BMP")))
    {
        m_pFG = (C_FGTemplate*)new C_FGBMP;
    }
    else
    {
        return FALSE;
    }
    m_pFG->Bind(szFN);
    m_nCurFrame = m_pFG->GetFirst() - 1;
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_FrameGrabber::GetFrame(BYTE *pbFrame, int nDirect)
{
    if((m_pFG == NULL) || (pbFrame == NULL))
    {
        return FALSE;
    }

    if(nDirect == FG_NEXT)
    {
        if(++m_nCurFrame > m_pFG->GetLast())
        {
            m_nCurFrame = m_pFG->GetFirst();
        }
    }
    else if(nDirect == FG_PREV)
    {
        if(--m_nCurFrame < m_pFG->GetFirst())
        {
            m_nCurFrame = m_pFG->GetLast();
        }
    }
    m_pFG->GetFrame(pbFrame, m_nCurFrame);
    
    return TRUE;
}
    //--------------------------------------------------------------------
int C_FrameGrabber::GetFrames()
{
    if(m_pFG == NULL)
    {
        return 0;
    }
    return m_pFG->GetLast() - m_pFG->GetFirst() + 1;
}
    //--------------------------------------------------------------------
int C_FrameGrabber::GetFrameNum()
{
    if(m_pFG == NULL)
    {
        return 0;
    }
    return m_nCurFrame - m_pFG->GetFirst() - 1;
}
    //--------------------------------------------------------------------
void C_FrameGrabber::Reset()
{
    if(m_pFG)
    {
        m_nCurFrame = m_pFG->GetFirst() - 1;
    }
}
    //--------------------------------------------------------------------
void C_FrameGrabber::Seek(int nFrameNum) {
	if(nFrameNum < m_pFG->GetFirst()) 
		m_nCurFrame = m_pFG->GetFirst();
	else if(nFrameNum > m_pFG->GetLast())
		m_nCurFrame = m_pFG->GetLast();
	else
		m_nCurFrame = nFrameNum;
}
	//--------------------------------------------------------------------

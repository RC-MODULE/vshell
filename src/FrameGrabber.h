    //--------------------------------------------------------------------
#ifndef _FRAMEGRABBER_H_
#define _FRAMEGRABBER_H_
    //--------------------------------------------------------------------
#include <vfw.h>
    //--------------------------------------------------------------------
const int FG_CUR = 0;
const int FG_NEXT = 1;
const int FG_PREV = 2;
    //--------------------------------------------------------------------
class C_FGTemplate
{
protected:
    int m_nWidth;
    int m_nHeight;
    int m_nBitCount;
    int m_nClrUsed;
    int m_nPixelStringSize;
    int m_nRem;
    int m_nFirstFrame;
    int m_nLastFrame;
    RGBQUAD m_pPal[256];
public:
    virtual ~C_FGTemplate() {}
    virtual BOOL Bind(LPSTR szFileName) = 0;
    virtual BOOL GetFrame(BYTE *pbFrame, int nFrameNum) = 0;
    int GetWidth() {return m_nWidth;}
    int GetLineWidth() {return m_nPixelStringSize;}
    int GetHeight() {return m_nHeight;}
    int GetBitCount() {return m_nBitCount;}
    void GetPalette(RGBQUAD *pPal);
    int GetFirst() {return m_nFirstFrame;}
    int GetLast() {return m_nLastFrame;}
};
    //--------------------------------------------------------------------
class C_FGBMP : public C_FGTemplate
{
private:
    char m_szFN[_MAX_PATH];
    int m_nOffset;
    int m_nNumLength;
    
    BOOL ParseFileName(char *szFN);
public:
    C_FGBMP();
    ~C_FGBMP();
    virtual BOOL Bind(LPSTR szFileName);
    virtual BOOL GetFrame(BYTE *pbFrame, int nFrameNum);
};
    //--------------------------------------------------------------------
class C_FGAVI : public C_FGTemplate
{
private:
    PAVIFILE m_pAF;
    PGETFRAME m_pGF;
    PAVISTREAM m_pAS;
public:
    C_FGAVI();
    ~C_FGAVI();
    virtual BOOL Bind(LPSTR szFileName);
    virtual BOOL GetFrame(BYTE *pbFrame, int nFrameNum);
};
	//--------------------------------------------------------------------
class C_FGV : public C_FGTemplate
{
private:
    HWND m_hwndCap;
    WORD m_wDeviceIndex;
	CAPDRIVERCAPS m_CapDriverCaps;
	CAPSTATUS m_CapStatus;
	CAPTUREPARMS m_CapParms;
public:
    C_FGV();
    ~C_FGV();
    virtual BOOL Bind(LPSTR szFileName);
    virtual BOOL GetFrame(BYTE *pbFrame, int nFrameNum);
};
    //--------------------------------------------------------------------
class C_FrameGrabber
{
private:
    int m_nCurFrame;
    C_FGTemplate *m_pFG;
public:
    C_FrameGrabber();
    ~C_FrameGrabber();

    BOOL Bind(HWND hWnd, LPSTR szFileName);
    BOOL GetFrame(BYTE *pbFrame, int nDirect);
    int GetFrames();
    int GetFrameNum();
    void Reset();
    int GetWidth() {if(m_pFG) return m_pFG->GetWidth(); return 0;}
    int GetLineWidth() {if(m_pFG) return m_pFG->GetLineWidth(); return 0;}
    int GetHeight() {if(m_pFG) return m_pFG->GetHeight(); return 0;}
    int GetBitCount() {if(m_pFG) return m_pFG->GetBitCount(); return 0;}
    void GetPalette(RGBQUAD *pPal) {if(m_pFG) m_pFG->GetPalette(pPal);}
	void Seek(int nFrameNum);
};
    //--------------------------------------------------------------------
#endif  // _FRAMEGRABBER_H_
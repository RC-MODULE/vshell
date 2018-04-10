//------------------------------------------------------------------------
#ifndef _FRAMEWND_H_
#define _FRAMEWND_H_
//------------------------------------------------------------------------
#include "BitmapEx.h"
#include "FrameGrabber.h"
//------------------------------------------------------------------------
struct S_ChildData
{
    int nID;
	char szName[256];
    C_BitmapEx *pBitmap;
};
//------------------------------------------------------------------------
struct S_VShellWindow
{
    int nID;
    BOOL fMinimized;
    POINT ptPos;
    BOOL fVectors;
    COLORREF VectorColor;
    S_Grid Grid;
    S_Adjust Adjust;
    BOOL fShowOverflows;
};
//------------------------------------------------------------------------
struct S_VShellContext
{
    BOOL fTexMinimized;
    RECT rcTextRect;
    int nWindows;
    S_VShellWindow *pWindow;
};
//------------------------------------------------------------------------
class C_FrameWnd
{
private:
    HMENU m_hFrameMenu;
    HWND m_hwndStatus;
    
    int m_nStatusHeight;
    BOOL m_fRunVideo;
    BOOL m_fPauseVideo;
    BOOL m_fStepVideo;
    BOOL m_fBack;
    
    S_VShellContext m_Context;

    char m_szStreamFileName[_MAX_PATH];
    int m_nStreamType;
    int m_nStreamBPP;
    int m_nStreamBufSize;
    BYTE *m_pbStreamBuffer;
    RGBQUAD m_pStreamPal[256];
    BOOL m_fBind;
    BOOL GetNextStreamFrame(BOOL fIter, BOOL fBack, BOOL fPause);
    HWND FindWindowData(int nID, S_ChildData **ppChData);
    BOOL Align(HWND hWnd);
    void SetMenuState(int nID, BOOL fEnable, BOOL fCheck);
    
    HINSTANCE m_hInstance;
    
    HMENU m_hPopupMenu;
    HCURSOR m_hCursorArrow;
    HCURSOR m_hCursorFlatHand;
    HCURSOR m_hCursorMoveHand;
    HCURSOR m_hCursorScale;
    HCURSOR m_hCurCursor;
    HACCEL m_hAccel;

    HWND m_hwndMouse;
    int m_nMouseX;
    int m_nMouseY;
    int m_nScaleMouseX;
    int m_nScaleMouseY;
    int m_nMouseKey;

    HWND m_hwndText;
    HWND m_hwndTextEdit;
    char *m_szTextBuffer;
    int m_nTextBufferSize;
    HFONT m_hTextFont;

    BOOL m_fSaveSequence;
    char m_szSaveFileName[_MAX_PATH];
	char m_szSaveSeqAsFileName[_MAX_PATH];
    int m_nSaveID;
    
    char m_szExeDir[_MAX_PATH];

    C_FrameGrabber m_FG;

public:
    HWND m_hwndView;
    HWND m_hwndFrame;
    BOOL m_fVSRun;

    C_FrameWnd();
    ~C_FrameWnd();

    int VSRun(HINSTANCE hInstance);  
    BOOL Bind(PSTR szFirstFileName);
    BOOL SetData(int nID, void *pvData);
    BOOL GetData(int nID, void *pvData);
    BOOL GetGrayData(int nID, void *pvData);
    BOOL GetBGRData(int nID, void *pvData);
    BOOL GetMouseStatus(int *pnID, int *pnX, int *pnY, int *pnScaleX, int *pnScaleY, 
            int *pnKey, int *pnCursor);
    BOOL SetVectors(int nID, int nBlockSize, int *pnVectors, COLORREF VectorsColor);
    BOOL SetVectors(int nID, int nBlockSize, float *pfltVectors, COLORREF VectorsColor);
    int GetWidth(int nID);
    int GetHeight(int nID);
    int GetType(int nID);
    int GetBitsPerPixel(int nID);
    int GetBufSize(int nID);
    BOOL SetPalette(int nID, RGBQUAD *prgbPal);
    BOOL GetPalette(int nID, RGBQUAD *prgbPal);
    BOOL SetText(char *szText);

    LRESULT OnCreate(HWND hWnd);
    LRESULT OnSize(WPARAM wp, LPARAM lp);
    LRESULT OnClose(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT OnDestroy();

    LRESULT OnMMFSaveImage();
    LRESULT OnMMFSaveSequence();
    LRESULT OnMMFSaveSetting();
    LRESULT OnMMFSaveControlSetting();
    LRESULT OnMMFLoadControlSetting();
    LRESULT OnMMFExit();
    LRESULT OnMMWCascade();
    LRESULT OnMMWTile();
    LRESULT OnMMWArrange();
    LRESULT OnMMWControls();
    LRESULT OnMMWClose();
    LRESULT OnMMOReset();
    LRESULT OnMMORun(BOOL fBack);
    LRESULT OnMMOPause();
    LRESULT OnMMOStep(BOOL fBack);
    LRESULT OnMMODrag();
    LRESULT OnMMOScale();
    LRESULT OnMMOScaleAccel(BOOL fZoom);
    LRESULT OnMMOVectors();
    LRESULT OnMMOGrid();
    LRESULT OnMMOShowUserGraphic();
    LRESULT OnMMOAlign();
    LRESULT OnMMORaster();
    LRESULT OnMMOShowOverflows();
    LRESULT OnMMSVectors();
    LRESULT OnMMSGrid();
    LRESULT OnMMSAdjust();
	LRESULT OnMMSSaveSequenceIn();
    LRESULT OnMMHAboutVS();
    LRESULT OnDefaultCommand(WPARAM wp, LPARAM lp);

    LRESULT OnChildCreate(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnChildPaint(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnChildMDIActivate(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnChildClose(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnChildDestroy(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnChildLButtonDown(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnChildLButtonUp(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnChildRButtonDown(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnChildActivate(HWND hWnd);
    LRESULT OnChildMouseMove(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnChildSize(HWND hWnd, WPARAM wp, LPARAM lp);

    LRESULT OnTextCreate(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnTextSize(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnTextClose(HWND hWnd, WPARAM wp, LPARAM lp);

    BOOL CreateChildWindow(PSTR szName, int nID, int nWidth, int nHeight, 
            int nType, RGBQUAD *prgbPalette);
    BOOL DeleteChildWindow(int nID);
    int Iteration();
    BOOL Draw(int nID);

    BOOL SetPixel(int nID, int nX, int nY, COLORREF crColor);
    BOOL Line(int nID, int nX1, int nY1, int nX2, int nY2, COLORREF crColor, int nDashDot);
    BOOL Polyline(int nID, POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot);
    BOOL PolyBezier(int nID, POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot);
    BOOL Arc(int nID, int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
            COLORREF crColor, int nDashDot);
    BOOL Rectangle(int nID, int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Polygon(int nID, POINT *pptPoints, int nPoints, 
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Ellipse(int nID, int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Chord(int nID, int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXRadial1, int nYRadial1,
            int nXRadial2, int nYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Pie(int nID, int nLeftRect, int nTopRect, 
            int nRightRect, int nBottomRect,
            int nXRadial1, int nYRadial1,
            int nXRadial2, int nYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    
    BOOL SetPixel(int nID, float fltX, float fltY, COLORREF crColor);
	BOOL Line(int nID, float fltX1, float fltY1, float fltX2, float fltY2, COLORREF crColor, int nDashDot);
    BOOL Polyline(int nID, float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot);
    BOOL PolyBezier(int nID, float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot);
    BOOL Arc(int nID, float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            float fltXStartArc, float fltYStartArc, float fltXEndArc, float fltYEndArc,
            COLORREF crColor, int nDashDot);
    BOOL Rectangle(int nID, float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Polygon(int nID, float *pfltPoints, int nPoints, 
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Ellipse(int nID, float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Chord(int nID, float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            float fltXRadial1, float fltYRadial1,
            float fltXRadial2, float fltYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL Pie(int nID, float fltLeftRect, float fltTopRect, 
            float fltRightRect, float fltBottomRect,
            float fltXRadial1, float fltYRadial1,
            float fltXRadial2, float fltYRadial2,
            COLORREF crBoundColor, COLORREF crFillColor, int nDashDot);
    BOOL TextOut(int nID, char *szText, int nX, int nY, int nSize, COLORREF clrColor);

    BOOL CorrectSignedImage(int nID);

    int GetSrcFrames();
    int GetSrcFrameNum();

    void SetCircleColors(COLORREF *pColor, int nColorCount, int nColorDisp);
    BOOL SetCirclePalette(int nID, int nColorDisp);
    BOOL SetRandomPalette(int nID);
    BOOL SetGrayPalette(int nID);

	void Seek(int nFrameNum);
};
//-------------------------------------------------------------------
#endif  // _FRAMEWND_H_ 
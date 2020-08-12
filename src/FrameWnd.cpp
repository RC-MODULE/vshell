//------------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <io.h>
#include "VShell.h"
#include "FrameWnd.h"
#include "ToolbarWnd.h"
#include "ControlWnd.h"
#include "VectorsSetupDlg.h"
#include "GridSetupDlg.h"
#include "AdjustSetupDlg.h"
#include "AboutDlg.h"
#include "SaveSequenceAsDlg.h"
#include "resource.h"
//------------------------------------------------------------------------
static const int FIRSTCHILD_ID          = 50000;
static const int ID_STATUSBAR           = 1000;
static const int STATUS_PARTS           = 4;
static const int WINDOW_MENU_POS        = 3;
static const int CONTROL_WIDTH          = 170;
static const char CONTEXT_FILE_NAME[]   = "VShell.dat";
static const HKEY REG_KEY               = HKEY_CURRENT_USER;
static const char REG_SUBKEY[]          = "SOFTWARE\\VSHELL";
static const char REG_KEYNAME[]         = "Toolbar";
static const int TEXT_BUFFER_SIZE       = 65536;
static const int DEF_TEXT_WIDTH         = 300;
static const int DEF_TEXT_HEIGHT        = 100;
static const int TOOLBAR_WND_HEIGHT     = 26;

//------------------------------------------------------------------------
C_FrameWnd FrameWnd;
extern C_ToolbarWnd ToolbarWnd;
extern C_ControlWnd ControlWnd;
extern C_VectorsSetupDlg VectorsSetupDlg;
extern C_GridSetupDlg GridSetupDlg;
extern C_AdjustSetupDlg AdjustSetupDlg;
extern C_AboutDlg AboutDlg;
extern C_SaveSequenceAsDlg SaveSequenceAsDlg;
//------------------------------------------------------------------------
BOOL CALLBACK CloseEnumProc(HWND hWnd, LPARAM)
{
    if(GetWindow(hWnd, GW_OWNER))
    {
        return TRUE;
    }
    SendMessage(GetParent(hWnd), WM_MDIRESTORE, (WPARAM)hWnd, 0);
    if(!SendMessage(hWnd, WM_QUERYENDSESSION, 0, 0))
    {
        return TRUE;
    }
    SendMessage(GetParent(hWnd), WM_MDIDESTROY, (WPARAM)hWnd, 0);
    return TRUE;
}
//------------------------------------------------------------------------
LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
    case WM_CREATE:
        return FrameWnd.OnCreate(hWnd);
    case WM_SIZE:
        return FrameWnd.OnSize(wp, lp);
    case WM_QUERYENDSESSION:
    case WM_CLOSE:
        return FrameWnd.OnClose(hWnd, msg, wp, lp);
    case WM_DESTROY:
        return FrameWnd.OnDestroy();
    case WM_COMMAND:
        switch(LOWORD(wp))
        {
        case MMW_CLOSE:
            return FrameWnd.OnMMWClose();
#ifndef __DEMO__
        case MMF_SAVE_IMAGE:
            return FrameWnd.OnMMFSaveImage();
        case MMF_SAVE_SEQUENCE:
            return FrameWnd.OnMMFSaveSequence();
#endif  // __DEMO__
        case MMF_SAVE_SETTING:
            return FrameWnd.OnMMFSaveSetting();
        case MMF_SAVE_CONTROL_SETTING:
            return FrameWnd.OnMMFSaveControlSetting();
        case MMF_LOAD_CONTROL_SETTING:
            return FrameWnd.OnMMFLoadControlSetting();
        case MMF_EXIT:
            return FrameWnd.OnMMFExit();
        case MMW_CASCADE:
            return FrameWnd.OnMMWCascade();
        case MMW_TILE:
            return FrameWnd.OnMMWTile();
        case MMW_ARRANGE:
            return FrameWnd.OnMMWArrange();
        case MMW_CONTROLS:
            return FrameWnd.OnMMWControls();
        case MMO_RESET:
            return FrameWnd.OnMMOReset();
        case MMO_RUN:
            return FrameWnd.OnMMORun(FALSE);
        case MMO_PAUSE:
            return FrameWnd.OnMMOPause();
        case MMO_STEP:
            return FrameWnd.OnMMOStep(FALSE);
        case MMO_RUN_BACK:
            return FrameWnd.OnMMORun(TRUE);
        case MMO_STEP_BACK:
            return FrameWnd.OnMMOStep(TRUE);
        case MMO_DRAG:
            return FrameWnd.OnMMODrag();
        case MMO_SCALE:
            return FrameWnd.OnMMOScale();
        case MMO_SCALE_PLUS:
            return FrameWnd.OnMMOScaleAccel(TRUE);
        case MMO_SCALE_MINUS:
            return FrameWnd.OnMMOScaleAccel(FALSE);
        case MMO_VECTORS:
            return FrameWnd.OnMMOVectors();
        case MMO_GRID:
            return FrameWnd.OnMMOGrid();
        case MMO_ALIGN:
            return FrameWnd.OnMMOAlign();
        case MMO_SHOW_USER_GRAPHIC:
            return FrameWnd.OnMMOShowUserGraphic();
        case MMO_RASTER:
            return FrameWnd.OnMMORaster();
        case MMO_SHOW_OVERFLOWS:
            return FrameWnd.OnMMOShowOverflows();
        case MMS_VECTORS:
            return FrameWnd.OnMMSVectors();
        case MMS_GRID:
            return FrameWnd.OnMMSGrid();
        case MMS_ADJUST:
            return FrameWnd.OnMMSAdjust();
		case MMS_SAVE_SEQUENCE_AS:
			return FrameWnd.OnMMSSaveSequenceIn();
        case MMH_ABOUT_VSHELL:
            return FrameWnd.OnMMHAboutVS();
        default:
            FrameWnd.OnDefaultCommand(wp, lp);
            break;
        }
    }

    return DefFrameProc(hWnd, FrameWnd.m_hwndView, msg, wp, lp);
}
//------------------------------------------------------------------------
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
    case WM_CREATE:
        return FrameWnd.OnChildCreate(hWnd, wp, lp);
    case WM_PAINT:
        return FrameWnd.OnChildPaint(hWnd, wp, lp);
    case WM_MDIACTIVATE:
        return FrameWnd.OnChildMDIActivate(hWnd, wp, lp);
    case WM_MOUSEMOVE:
        return FrameWnd.OnChildMouseMove(hWnd, wp, lp);
    case WM_LBUTTONDOWN:
        return FrameWnd.OnChildLButtonDown(hWnd, wp, lp);
    case WM_LBUTTONUP:
        return FrameWnd.OnChildLButtonUp(hWnd, wp, lp);
    case WM_RBUTTONDOWN:
        return FrameWnd.OnChildRButtonDown(hWnd, wp, lp);
    case WM_CLOSE:
        return FrameWnd.OnChildClose(hWnd, wp, lp);
    case WM_DESTROY:
        return FrameWnd.OnChildDestroy(hWnd, wp, lp);

    }

    return DefMDIChildProc(hWnd, msg, wp, lp);
}
//------------------------------------------------------------------------
LRESULT CALLBACK TextWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
    case WM_CREATE:
        return FrameWnd.OnTextCreate(hWnd, wp, lp);
    case WM_SIZE:
        return FrameWnd.OnTextSize(hWnd, wp, lp);
    case WM_CLOSE:
        return FrameWnd.OnTextClose(hWnd, wp, lp);
    }

    return DefMDIChildProc(hWnd, msg, wp, lp);
}
//------------------------------------------------------------------------
C_FrameWnd::C_FrameWnd()
{
    m_fRunVideo = FALSE;
    m_fPauseVideo = FALSE;
    m_fStepVideo = FALSE;
    m_fVSRun = FALSE;
    m_hCursorArrow = NULL;
    m_hCursorFlatHand = NULL;
    m_hCursorMoveHand = NULL;
    m_hCursorScale = NULL;
    m_szStreamFileName[0] = '\0';
    m_nStreamType = 0;
    m_nStreamBPP = 0;
    m_nStreamBufSize = 0;
    m_pbStreamBuffer = NULL;
    m_fBind = FALSE;
    m_hwndText = FALSE;
    m_hwndTextEdit = FALSE;
    m_szTextBuffer = NULL;
    m_nTextBufferSize = 0;
    m_hTextFont = NULL;
    m_fSaveSequence = FALSE;
    m_nMouseX = 0;
    m_nMouseY = 0;
    m_nScaleMouseX = 0;
    m_nScaleMouseY = 0;
    GetCurrentDirectory(_MAX_PATH, m_szExeDir);
    m_hInstance = 0;
	m_szSaveSeqAsFileName[0] = '\0';
}
//------------------------------------------------------------------------
C_FrameWnd::~C_FrameWnd()
{
    delete [] m_pbStreamBuffer;
    delete [] m_szTextBuffer;
    if(m_hTextFont)
    {
        DeleteObject(m_hTextFont);
    }
}
//------------------------------------------------------------------------
int C_FrameWnd::VSRun(HINSTANCE hInstance)
{
    if(m_fVSRun)
    {
        return 0;
    }

    if(!m_hInstance)
    {
        WNDCLASSEX wc;

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = FrameWndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_hInstance = hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_APPWORKSPACE + 1);                            
        wc.lpszMenuName = NULL;
        wc.lpszClassName = "VShellFrameWndClass";
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        if(!RegisterClassEx(&wc))
        {
            return 0;
        }
    
        wc.lpfnWndProc = ChildWndProc;
        wc.cbWndExtra = sizeof(HANDLE);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   
        wc.hCursor = NULL;
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);                            
        wc.lpszClassName = "VShellChildWndClass";
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        if(!RegisterClassEx(&wc))
        {    
            return 0;
        }

        wc.lpfnWndProc = TextWndProc;
        wc.cbWndExtra = 0;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);                            
        wc.lpszClassName = "VShellTextChildWndClass";
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
        if(!RegisterClassEx(&wc))
        {    
            return 0;
        }

        if(!ToolbarWnd.RegisterWndClass(m_hInstance))
        {
            return 0;
        }
    
        if(!ControlWnd.RegisterWndClass(m_hInstance))
        {
            return 0;
        }

        m_hCursorArrow = LoadCursor(NULL, IDC_ARROW);
        m_hCursorFlatHand = LoadCursor(m_hInstance, MAKEINTRESOURCE(CURSOR_FLAT_HAND));
        m_hCursorMoveHand = LoadCursor(m_hInstance, MAKEINTRESOURCE(CURSOR_MOVE_HAND));
        m_hCursorScale = LoadCursor(m_hInstance, MAKEINTRESOURCE(CURSOR_SCALE));
        m_hCurCursor = m_hCursorArrow;

        m_hFrameMenu = LoadMenu(m_hInstance, MAKEINTRESOURCE(MAIN_MENU));
        m_hPopupMenu = LoadMenu(m_hInstance, MAKEINTRESOURCE(POPUP_MENU));
        m_hPopupMenu = GetSubMenu(m_hPopupMenu, 0);
        
        m_hAccel = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(ACCELERATOR));
    }
    
    char szPath[MAX_PATH];
    char szName[MAX_PATH];
    int i;

    GetModuleFileName(NULL, szPath, MAX_PATH - 1);
    for(i=lstrlen(szPath); i>=0; i--)
    {
        if(szPath[i] == '\\')
        {
            break;
        }
    }
    sprintf(szName, "VShell [%s]", &szPath[i+1]);
    if((m_hwndFrame = CreateWindow("VShellFrameWndClass",
            szName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, m_hFrameMenu, m_hInstance, NULL)) == NULL)
    {
        return 0;      
    }
    
    ShowWindow(m_hwndFrame, SW_SHOWDEFAULT);
    UpdateWindow(m_hwndFrame);

    m_fVSRun = TRUE;

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        if(!TranslateMDISysAccel(m_hwndView, &msg) && !TranslateAccelerator(m_hwndFrame, m_hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    DestroyMenu(m_hFrameMenu);
    DestroyMenu(m_hPopupMenu);
    m_fVSRun = FALSE;

    return (int)msg.wParam;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnCreate(HWND hWnd)
{
    CLIENTCREATESTRUCT ccs;
    RECT rc;

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC  = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);

    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadImage(m_hInstance, 
            MAKEINTRESOURCE(IDI_VSHELL), IMAGE_ICON, 48, 48, 0));    
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadImage(m_hInstance, 
            MAKEINTRESOURCE(IDI_VSHELL), IMAGE_ICON, 32, 32, 0));
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadImage(m_hInstance, 
            MAKEINTRESOURCE(IDI_VSHELL), IMAGE_ICON, 16, 16, 0));


    ControlWnd.Create(hWnd);
    ControlWnd.Move(0, 0, CONTROL_WIDTH, 0);
    
    m_hwndStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE |
            WS_CLIPSIBLINGS | CCS_BOTTOM | SBARS_SIZEGRIP,
            NULL, hWnd, ID_STATUSBAR);
    GetWindowRect(m_hwndStatus, &rc);
    m_nStatusHeight = rc.bottom - rc.top;
    
    ToolbarWnd.Create(hWnd);

    ccs.hWindowMenu = GetSubMenu(m_hFrameMenu, WINDOW_MENU_POS);
    ccs.idFirstChild = FIRSTCHILD_ID;
    m_hwndView = CreateWindow("MDICLIENT", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
            0, 0, 0, 0, hWnd, (HMENU)1, m_hInstance, (PSTR)&ccs);

    FILE *fp;
    fp = fopen(CONTEXT_FILE_NAME, "rb");
    if(fp)
    {
        BOOL fMax, fControl;
        fread(&fMax, sizeof(BOOL), 1, fp);
        fread(&fControl, sizeof(BOOL), 1, fp);
        fread(&rc, sizeof(RECT), 1, fp);
        fread(&m_Context.fTexMinimized, sizeof(BOOL), 1, fp);
        fread(&m_Context.rcTextRect, sizeof(RECT), 1, fp);
        fread(&m_Context.nWindows, sizeof(int), 1, fp);
        m_Context.pWindow = new S_VShellWindow[m_Context.nWindows];
        fread(m_Context.pWindow, sizeof(S_VShellWindow) * m_Context.nWindows, 1, fp);
        fclose(fp);
        if(fMax)
        {
            ShowWindow(hWnd, SW_MAXIMIZE);
        }
        else
        {
            MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
        }
        if(fControl)
        {
            OnMMWControls();
        }
    }
    else
    {
        m_Context.fTexMinimized = FALSE;
        m_Context.rcTextRect.left = 0;
        m_Context.rcTextRect.right = 0;
        m_Context.rcTextRect.right = DEF_TEXT_WIDTH;
        m_Context.rcTextRect.bottom = DEF_TEXT_HEIGHT;
    }

    m_szTextBuffer = new char[TEXT_BUFFER_SIZE];
    ZeroMemory(m_szTextBuffer, TEXT_BUFFER_SIZE);

    ToolbarWnd.SaveRestore(REG_KEY, (char*)REG_SUBKEY, (char*)REG_KEYNAME);

#ifdef __DEMO__
    SetMenuState(MMF_SAVE_IMAGE, FALSE, FALSE);
    SetMenuState(MMF_SAVE_SEQUENCE, FALSE, FALSE);
#endif  //__DEMO__

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnSize(WPARAM wp, LPARAM lp)
{
    int cx = LOWORD(lp);
    int cy = HIWORD(lp);

    MoveWindow(m_hwndStatus, 0, cy - m_nStatusHeight, 
            cx, m_nStatusHeight, TRUE);
    ToolbarWnd.Move(0, 0, cx, TOOLBAR_WND_HEIGHT);

    if((wp == SIZEICONIC) || (m_hwndView == NULL))
    {
        return NULL;
    }

    int nSPWidth = cx / STATUS_PARTS;
    int nBorders[STATUS_PARTS];
	int i;
    for(i=0; i<STATUS_PARTS - 1; i++)
    {
        nBorders[i] = nSPWidth * (i + 1);
    }
    nBorders[i] = -1;
    SendMessage(m_hwndStatus, SB_SETPARTS, (WPARAM)STATUS_PARTS, (LPARAM)nBorders);

    if(MF_CHECKED == CheckMenuItem(m_hFrameMenu, MMW_CONTROLS, MF_BYCOMMAND))
    {
        CheckMenuItem(m_hFrameMenu, MMW_CONTROLS, MF_CHECKED | MF_BYCOMMAND);
        cx -= CONTROL_WIDTH;
        ControlWnd.Show(TRUE);
        ControlWnd.Move(cx, TOOLBAR_WND_HEIGHT, CONTROL_WIDTH, 
                cy - (TOOLBAR_WND_HEIGHT + m_nStatusHeight));
    }
    else
    {
        ControlWnd.Show(FALSE);
    }

    MoveWindow(m_hwndView, 0, TOOLBAR_WND_HEIGHT, cx, cy - (TOOLBAR_WND_HEIGHT + m_nStatusHeight), TRUE);

    return NULL; 
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnClose(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    m_fRunVideo = FALSE;
    m_fStepVideo = FALSE;
    Sleep(100);
    EnumChildWindows(m_hwndView, &CloseEnumProc, 0);
    if(NULL != GetWindow(m_hwndView, GW_CHILD))
    {
        return NULL;
    }
    return DefFrameProc(hWnd, m_hwndView, msg, wp, lp);
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnDestroy()
{
    PostQuitMessage(0);
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMWClose()
{
    SendMessage((HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0), WM_CLOSE, 0, 0);
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMFSaveImage()
{
    if(m_fRunVideo || m_fStepVideo)
    {
        return NULL;
    }

    HWND hwndCur = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    S_ChildData *pChData;

    pChData = (S_ChildData*)GetWindowLongPtr(hwndCur, 0);
    if(pChData == NULL || pChData->pBitmap == NULL)
    {
        return NULL;
    }

    char szFileName[_MAX_PATH];
    OPENFILENAME ofn;
    
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ZeroMemory(szFileName, sizeof(szFileName));
    ofn.lStructSize         = sizeof( OPENFILENAME );
    ofn.hwndOwner           = NULL;
    ofn.hInstance           = m_hInstance;
    ofn.lpstrFilter         = "BMP Files\0*.bmp\0\0";
    ofn.lpstrFile           = szFileName;
    ofn.nMaxFile            = _MAX_PATH;
    ofn.nMaxFileTitle       = _MAX_FNAME + _MAX_EXT;
    ofn.lpstrTitle          = "Save image as";
   
    if(GetSaveFileName(&ofn))
    {
        char szExt[10];
        int nLen;

        nLen = lstrlen(szFileName);
        if(nLen > 4)
        {
            lstrcpy(szExt, &szFileName[lstrlen(szFileName) - 3]);
            if(lstrcmp(szExt, "bmp") && lstrcmp(szExt, "BMP"))
            {
                lstrcat(szFileName, ".bmp");
            }
        }
        else
        {
            lstrcat(szFileName, ".bmp");
        }

        pChData->pBitmap->SaveImage(szFileName);
    }

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMFSaveSequence()
{
    S_ChildData *pChData = NULL;

    if(MF_CHECKED == CheckMenuItem(m_hFrameMenu, MMF_SAVE_SEQUENCE, MF_BYCOMMAND))
    {
		SetMenuState(MMO_SCALE, TRUE, FALSE);
        CheckMenuItem(m_hFrameMenu, MMF_SAVE_SEQUENCE, MF_UNCHECKED);
        CheckMenuItem(m_hPopupMenu, MMF_SAVE_SEQUENCE, MF_UNCHECKED);
		m_fSaveSequence = FALSE;
		FindWindowData(m_nSaveID, &pChData);
		if(pChData == NULL || pChData->pBitmap == NULL)
		{
			return NULL;
		}
        pChData->pBitmap->DeleteAVIWriter();
        return NULL;
    }
    
	HWND hwndCur = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
	pChData = (S_ChildData*)GetWindowLongPtr(hwndCur, 0);
	if(pChData == NULL || pChData->pBitmap == NULL)
	{
		return NULL;
	}

	if(lstrlen(m_szSaveSeqAsFileName)) 
	{
		lstrcpy(m_szSaveFileName, m_szSaveSeqAsFileName);
	}
	else
	{
		OPENFILENAME ofn;
	    
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		m_szSaveFileName[0] = '\0';
		ofn.lStructSize         = sizeof(OPENFILENAME);
		ofn.hwndOwner           = NULL;
		ofn.hInstance           = m_hInstance;
		ofn.lpstrFilter         = "AVI Files\0*.avi\0BMP Files\0*.bmp\0\0";
		ofn.lpstrFile           = m_szSaveFileName;
		ofn.nMaxFile            = _MAX_PATH;
		ofn.nMaxFileTitle       = _MAX_FNAME + _MAX_EXT;
		ofn.lpstrTitle          = "Save sequence as";
		if(!GetSaveFileName(&ofn))
		{
			return NULL;
		}
		if(lstrlen(m_szSaveFileName) <= 4 || (lstrlen(m_szSaveFileName) > 4 && 
				lstrcmpi(&m_szSaveFileName[lstrlen(m_szSaveFileName) - 4], ofn.nFilterIndex == 1 ? ".avi" : ".bmp")))
		{
			lstrcat(m_szSaveFileName, ofn.nFilterIndex == 1 ? ".avi" : ".bmp");
		}
	}
	if(!lstrcmpi(&m_szSaveFileName[lstrlen(m_szSaveFileName) - 4], ".bmp")) 
	{
		lstrcpy(&m_szSaveFileName[lstrlen(m_szSaveFileName) - 4], "0000");
	}
	else 
	{
		pChData->pBitmap->CreateAVIWriter(m_szSaveFileName);
	}

	CheckMenuItem(m_hFrameMenu, MMF_SAVE_SEQUENCE, MF_CHECKED);
    CheckMenuItem(m_hPopupMenu, MMF_SAVE_SEQUENCE, MF_CHECKED);
    m_fSaveSequence = TRUE;
    m_nSaveID = pChData->nID;
	if(m_hCurCursor == m_hCursorScale)
	{
		OnMMOScale();
	}
	SetMenuState(MMO_SCALE, FALSE, FALSE);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMFSaveSetting()
{
    FILE *fp;
    BOOL fMax, fControl;
    RECT rc;
    HWND hwndChild;
    int i = 0;
    S_VShellWindow *pWindow;
    S_ChildData *pChData;
    int nWindows = 0;

    SetCurrentDirectory(m_szExeDir);
    
    fMax = IsZoomed(m_hwndFrame);
    if(MF_CHECKED == CheckMenuItem(m_hFrameMenu, MMW_CONTROLS, MF_BYCOMMAND))
    {
        CheckMenuItem(m_hFrameMenu, MMW_CONTROLS, MF_CHECKED);
        fControl = TRUE;
    }
    else
    {
        CheckMenuItem(m_hFrameMenu, MMW_CONTROLS, MF_UNCHECKED);
        fControl = FALSE;
    }
    GetWindowRect(m_hwndFrame, &rc);
    fp = fopen(CONTEXT_FILE_NAME, "wb");
    fwrite(&fMax, sizeof(BOOL), 1, fp);
    fwrite(&fControl, sizeof(BOOL), 1, fp);
    fwrite(&rc, sizeof(RECT), 1, fp);
    if(m_hwndText)
    {
        fMax = IsIconic(m_hwndText);
        GetWindowRect(m_hwndText, &rc);
        ScreenToClient(m_hwndView, (POINT*)&rc);
        ScreenToClient(m_hwndView, (POINT*)&rc.right);
        fwrite(&fMax, sizeof(BOOL), 1, fp);
        fwrite(&rc, sizeof(RECT), 1, fp);
    }
    else
    {
        fMax = FALSE;
        rc.left = rc.top = 0;
        rc.right = 300;
        rc.bottom = 100;
        fwrite(&fMax, sizeof(BOOL), 1, fp);
        fwrite(&rc, sizeof(RECT), 1, fp);
    }
    
    while(hwndChild = GetDlgItem(m_hwndView, FIRSTCHILD_ID + i))
    {
        i++;
    }
    pWindow = new S_VShellWindow[nWindows = i];
    i = 0;
    while(hwndChild = GetDlgItem(FrameWnd.m_hwndView, FIRSTCHILD_ID + i))
    {
        pChData = (S_ChildData*)GetWindowLongPtr(hwndChild, 0);
        if(pChData && pChData->pBitmap)
        {
            pWindow[i].fMinimized = IsIconic(hwndChild);
            GetWindowRect(hwndChild, &rc);
            ScreenToClient(m_hwndView, (POINT*)&rc);
            ScreenToClient(m_hwndView, (POINT*)&rc.right);
            pWindow[i].ptPos.x = rc.left;
            pWindow[i].ptPos.y = rc.top;
        
            pWindow[i].nID = pChData->nID;
            pWindow[i].fShowOverflows = pChData->pBitmap->m_fShowOverflows;
            pWindow[i].fVectors = pChData->pBitmap->m_VG.m_Vectors.fShow;
            pWindow[i].VectorColor = pChData->pBitmap->m_VG.m_Vectors.crColor;
            memcpy(&(pWindow[i].Grid), &pChData->pBitmap->m_VG.m_Grid, sizeof(S_Grid));
            S_Adjust Adjust;
            pChData->pBitmap->GetAdjust(&Adjust);
            memcpy(&(pWindow[i].Adjust), &Adjust, sizeof(S_Adjust));
        }
        
        i++;
    }
    fwrite(&nWindows, sizeof(int), 1, fp);
    fwrite(pWindow, sizeof(S_VShellWindow) * nWindows, 1, fp);
    delete [] pWindow;
    fclose(fp);

    ToolbarWnd.SaveRestore(REG_KEY, (char*)REG_SUBKEY, (char*)REG_KEYNAME);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMFSaveControlSetting()
{
    ControlWnd.SaveSetting(); 
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMFLoadControlSetting()
{
    ControlWnd.LoadSetting(); 
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMFExit()
{
    SendMessage(m_hwndFrame, WM_CLOSE, 0, 0);
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMWCascade()
{
    SendMessage(m_hwndView, WM_MDICASCADE, 0, 0);
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMWTile()
{
    SendMessage(m_hwndView, WM_MDITILE, 0, 0);
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMWArrange()
{
    SendMessage(m_hwndView, WM_MDIICONARRANGE, 0, 0);
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMWControls()
{
    RECT rc;
    BOOL fChecked = (MF_CHECKED == CheckMenuItem(m_hFrameMenu, MMW_CONTROLS, MF_BYCOMMAND));
    
    SetMenuState(MMW_CONTROLS, TRUE, !fChecked);
    
    GetClientRect(m_hwndFrame, &rc);
    SendMessage(m_hwndFrame, WM_SIZE, 0, rc.right | (rc.bottom << 16));
    
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOReset()
{
    m_FG.Reset();
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMORun(BOOL fBack)
{
    m_fBack = fBack;
    int nRunID = fBack ? MMO_RUN_BACK : MMO_RUN;
    int nRunBackID = fBack ? MMO_RUN : MMO_RUN_BACK;
    int nStepID = fBack ? MMO_STEP_BACK : MMO_STEP;
    int nStepBackID = fBack ? MMO_STEP : MMO_STEP_BACK;
    
    m_fRunVideo = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, nRunID, MF_BYCOMMAND));

#ifndef __DEMO__
    SetMenuState(MMF_SAVE_IMAGE, !m_fRunVideo, FALSE);
#endif  //__DEMO__
    SetMenuState(nRunID, TRUE, m_fRunVideo);
    SetMenuState(MMO_BIND, !m_fRunVideo, FALSE);
    SetMenuState(nStepID, !m_fRunVideo, FALSE);
    SetMenuState(nRunBackID, !m_fRunVideo, FALSE);
    SetMenuState(nStepBackID, !m_fRunVideo, FALSE);
    
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOPause()
{
    m_fPauseVideo = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, MMO_PAUSE, MF_BYCOMMAND));
    SetMenuState(MMO_PAUSE, TRUE, m_fPauseVideo);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOStep(BOOL fBack)
{
    m_fBack = fBack;
    m_fStepVideo = TRUE;

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMODrag()
{
    BOOL fDrag = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, MMO_DRAG, MF_BYCOMMAND));

    SetMenuState(MMO_DRAG, TRUE, fDrag);
   
    if(fDrag)
    {
        m_hCurCursor = m_hCursorFlatHand;
        SetMenuState(MMO_SCALE, TRUE, FALSE);
    }
    else
    {
        m_hCurCursor = m_hCursorArrow;
    }

    SetCursor(m_hCurCursor);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOScale()
{
    BOOL fScale = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, MMO_SCALE, MF_BYCOMMAND));

    SetMenuState(MMO_SCALE, TRUE, fScale);
   
    if(fScale)
    {
        m_hCurCursor = m_hCursorScale;
        SetMenuState(MMO_DRAG, TRUE, FALSE);
    }
    else
    {
        m_hCurCursor = m_hCursorArrow;
    }

    SetCursor(m_hCurCursor);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOScaleAccel(BOOL fZoom)
{
    HWND hWnd;
    RECT rcWnd;
    LPARAM lp;
    WPARAM wp;
    POINT ptCursor;
    HCURSOR hCursor;
    UINT nAlignState;
    
	hWnd = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    if(!hWnd)
    {
        return NULL;
    }

    wp = fZoom ? 0 : (WPARAM)(MK_SHIFT + 1);
    hCursor = FrameWnd.m_hCurCursor;
    GetClientRect(hWnd, &rcWnd);
    lp = MAKELONG(rcWnd.right / 2, rcWnd.left / 2);
    GetCursorPos(&ptCursor);
    FrameWnd.m_hCurCursor = FrameWnd.m_hCursorScale;
    nAlignState = GetMenuState(m_hFrameMenu, MMO_ALIGN, MF_BYCOMMAND);
    CheckMenuItem(m_hFrameMenu, MMO_ALIGN, MF_CHECKED);

    OnChildLButtonDown(hWnd, wp, lp);
    
    CheckMenuItem(m_hFrameMenu, MMO_ALIGN, nAlignState);
    FrameWnd.m_hCurCursor = hCursor;
    SetCursorPos(ptCursor.x, ptCursor.y);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOVectors()
{
    HWND hwndCur = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    S_ChildData *pChData = (S_ChildData*)(GetWindowLongPtr(hwndCur, 0));
    if(!pChData)
    {
        return NULL;
    }

    BOOL fVector = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, MMO_VECTORS, MF_BYCOMMAND));

    SetMenuState(MMO_VECTORS, TRUE, fVector);
    pChData->pBitmap->m_VG.m_Vectors.fShow = fVector;

    InvalidateRect(hwndCur, NULL, FALSE);
    UpdateWindow(hwndCur);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOGrid()
{
    HWND hwndCur = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    S_ChildData *pChData = (S_ChildData*)(GetWindowLongPtr(hwndCur, 0));
    if(!pChData)
    {
        return NULL;
    }

    BOOL fGrid = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, MMO_GRID, MF_BYCOMMAND));
    SetMenuState(MMO_GRID, TRUE, fGrid);
    pChData->pBitmap->m_VG.m_Grid.fShow = fGrid;

    InvalidateRect(hwndCur, NULL, FALSE);
    UpdateWindow(hwndCur);
    
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOAlign()
{
    HWND hwndCur = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    S_ChildData *pChData = (S_ChildData*)(GetWindowLongPtr(hwndCur, 0));
    if(!pChData)
    {
        return NULL;
    }

    BOOL fAlign = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, MMO_ALIGN, MF_BYCOMMAND));
    SetMenuState(MMO_ALIGN, TRUE, fAlign);
    if(fAlign)
    {
        Align(hwndCur);
    }

    InvalidateRect(hwndCur, NULL, FALSE);
    UpdateWindow(hwndCur);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOShowUserGraphic()
{
    HWND hwndCur = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    S_ChildData *pChData = (S_ChildData*)(GetWindowLongPtr(hwndCur, 0));
    if(!pChData)
    {
        return NULL;
    }

    BOOL fGraph = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, MMO_SHOW_USER_GRAPHIC, MF_BYCOMMAND));
    SetMenuState(MMO_SHOW_USER_GRAPHIC, TRUE, fGraph);
    pChData->pBitmap->m_fShowUserGraphic = fGraph;

    InvalidateRect(hwndCur, NULL, FALSE);
    UpdateWindow(hwndCur);
    
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMORaster()
{
    HWND hwndCur = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    S_ChildData *pChData = (S_ChildData*)(GetWindowLongPtr(hwndCur, 0));
    if(!pChData)
    {
        return NULL;
    }

    BOOL fRast = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, MMO_RASTER, MF_BYCOMMAND));
    SetMenuState(MMO_RASTER, TRUE, fRast);
    pChData->pBitmap->m_fRaster = fRast;

    InvalidateRect(hwndCur, NULL, FALSE);
    UpdateWindow(hwndCur);
    
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMOShowOverflows()
{
    HWND hwndActive = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(hwndActive, 0);
    if(pChData == NULL)
    {
        return NULL;
    }

    BOOL fOF = (MF_CHECKED != CheckMenuItem(m_hFrameMenu, MMO_SHOW_OVERFLOWS, MF_BYCOMMAND));
    SetMenuState(MMO_SHOW_OVERFLOWS, TRUE, fOF);
    pChData->pBitmap->m_fShowOverflows = fOF;

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMSVectors()
{
    HWND hwndActive = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(hwndActive, 0);
    if(pChData == NULL)
    {
        return NULL;
    }

    VectorsSetupDlg.Show(m_hInstance, hwndActive, &(pChData->pBitmap->m_VG.m_Vectors.crColor));

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMSGrid()
{
    HWND hwndActive = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(hwndActive, 0);
    if(pChData == NULL)
    {
        return NULL;
    }

    GridSetupDlg.Show(m_hInstance, hwndActive, 
            &(pChData->pBitmap->m_VG.m_Grid.nX), 
            &(pChData->pBitmap->m_VG.m_Grid.nY), 
            &(pChData->pBitmap->m_VG.m_Grid.crColor));

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMSAdjust()
{
    HWND hwndActive = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    if(hwndActive)
    {
        AdjustSetupDlg.Show(m_hInstance, hwndActive);
    }

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMSSaveSequenceIn() 
{
	SaveSequenceAsDlg.Show(m_hInstance, m_hwndFrame, m_szSaveSeqAsFileName);
	return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnMMHAboutVS()
{
    AboutDlg.Show(m_hwndView, m_hInstance);
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnDefaultCommand(WPARAM wp, LPARAM lp)
{
    HWND hChildWnd = (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0);
    if(IsWindow(hChildWnd))
    {
        SendMessage(hChildWnd, WM_COMMAND, wp, lp);
    }
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildCreate(HWND hWnd, WPARAM, LPARAM)
{
    S_ChildData *pChData;

    pChData = (S_ChildData*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 
            sizeof(S_ChildData));
    ZeroMemory(pChData, sizeof(S_ChildData));
    
    SetWindowLongPtr(hWnd, 0, (long)pChData);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildActivate(HWND hWnd)
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(hWnd, 0);
    BOOL fValid = (pChData && pChData->pBitmap);

    SetMenuState(MMO_VECTORS, TRUE, fValid && pChData->pBitmap->m_VG.m_Vectors.fShow);
    SetMenuState(MMO_GRID, TRUE, fValid && pChData->pBitmap->m_VG.m_Grid.fShow);
    SetMenuState(MMO_RASTER, TRUE, fValid && pChData->pBitmap->m_fRaster);
    SetMenuState(MMO_SHOW_USER_GRAPHIC, TRUE, fValid && pChData->pBitmap->m_fShowUserGraphic);
    SetMenuState(MMO_SHOW_OVERFLOWS, TRUE, fValid && pChData->pBitmap->m_fShowOverflows);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildMouseMove(HWND hWnd, WPARAM wp, LPARAM lp)
{
    HDC hDC;
    char szMes[255];
    S_ChildData *pChData;
    COLORREF cr;
	DWORD dwPixVal;

    pChData = (S_ChildData*)GetWindowLongPtr(hWnd, 0);
    if(!pChData || !pChData->pBitmap)
    {
        return NULL;
    }
    
    if(FrameWnd.m_hCurCursor == FrameWnd.m_hCursorMoveHand)
    {
        pChData->pBitmap->MoveImage(m_nMouseX - (short)(LOWORD(lp)), 
                (short)(HIWORD(lp) - m_nMouseY));
    }

    m_hwndMouse = hWnd;
    m_nMouseX = (short)(LOWORD(lp));
    m_nMouseY = (short)(HIWORD(lp));
    m_nMouseKey = (int)wp;

    
    hDC = GetDC(hWnd);
    cr = GetPixel(hDC, LOWORD(lp), HIWORD(lp));
    ReleaseDC(hWnd, hDC);
    pChData->pBitmap->GetScaleMouseFromMouse(m_nMouseX, m_nMouseY, &m_nScaleMouseX, &m_nScaleMouseY);
    sprintf(szMes, "X:%d, Y:%d", m_nScaleMouseX, m_nScaleMouseY);
    SendMessage(m_hwndStatus, SB_SETTEXT, 0, (LPARAM)szMes);
    
	dwPixVal = pChData->pBitmap->GetPixelValue(m_nScaleMouseX, m_nScaleMouseY);
	sprintf(szMes, "RGB:%u,%u,%u; VAL:#%X,%u,%d", cr & 0xFF, (cr >> 8) & 0xFF, cr >> 16, dwPixVal, dwPixVal, (int)dwPixVal);
    SendMessage(m_hwndStatus, SB_SETTEXT, 1, (LPARAM)szMes);

    int nXG, nYG;
    float fltXV, fltYV;

    nXG = m_nScaleMouseX;
    nYG = m_nScaleMouseY;
    fltXV = (float)m_nScaleMouseX;
    fltYV = (float)m_nScaleMouseY;
    
    pChData->pBitmap->GetGridCoordinates(&nXG, &nYG);
    pChData->pBitmap->GetVectorsInBlock(&fltXV, &fltYV);

    if((int)fltXV == 0x80000000)
    {
        sprintf(szMes, "GX:%d, GY:%d, VX: none, VY: none", nXG, nYG);
    }
    else
    {
        sprintf(szMes, "GX:%d, GY:%d, VX:%.2f, VY:%.2f", nXG, nYG, fltXV, fltYV);
    }
    SendMessage(m_hwndStatus, SB_SETTEXT, 2, (LPARAM)szMes);

    SetCursor(m_hCurCursor);
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildPaint(HWND hWnd, WPARAM, LPARAM)
{
    PAINTSTRUCT ps;
    S_ChildData *pChData;

    BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);

    pChData = (S_ChildData*)GetWindowLongPtr(hWnd, 0);
    if(pChData == NULL || pChData->pBitmap == NULL)
    {
        return FALSE;
    }
    pChData->pBitmap->Draw();

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildMDIActivate(HWND hWnd, WPARAM, LPARAM lp)
{
    if(lp == (LPARAM)hWnd)
    {
        if(!IsWindowVisible(hWnd))
        {
            ShowWindow(hWnd, SW_SHOW);
        }
        FrameWnd.OnChildActivate(hWnd);
    }
    DrawMenuBar(GetParent(GetParent(hWnd)));
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildClose(HWND hWnd, WPARAM, LPARAM)
{
    if(IsIconic(hWnd))
    {
        return NULL;
    }
    HWND hViewWnd = GetParent(hWnd);
    SendMessage(hViewWnd, WM_MDINEXT, (WPARAM)hWnd, (LPARAM)1); 
    if(hWnd != (HWND)SendMessage(hViewWnd, WM_MDIGETACTIVE, 0, 0))
    {
        ShowWindow(hWnd, SW_MINIMIZE);
    }
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildDestroy(HWND hWnd, WPARAM, LPARAM)
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(hWnd, 0);
    if(pChData)
    {
        delete pChData->pBitmap;
        HeapFree(GetProcessHeap(), 0, (void*)pChData); 
    }
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildLButtonDown(HWND hWnd, WPARAM wp, LPARAM lp)
{
    m_hwndMouse = hWnd;
    m_nMouseX = LOWORD(lp);
    m_nMouseY = HIWORD(lp);
    m_nMouseKey = (int)wp;
    if(FrameWnd.m_hCurCursor == FrameWnd.m_hCursorFlatHand)
    {
        SetCapture(hWnd);
        FrameWnd.m_hCurCursor = FrameWnd.m_hCursorMoveHand;
        SetCursor(m_hCurCursor);
    }
    else if(FrameWnd.m_hCurCursor == FrameWnd.m_hCursorScale)
    {
        S_ChildData *pChData;
        POINT ptMouse;

        pChData = (S_ChildData*)GetWindowLongPtr(hWnd, 0);
            
        if(!pChData || !pChData->pBitmap)
        {
            return NULL;
        }
        
        pChData->pBitmap->SetScale(m_nMouseX, m_nMouseY, m_nMouseKey == (MK_SHIFT + 1));
        if(MF_CHECKED == GetMenuState(m_hFrameMenu, MMO_ALIGN, MF_BYCOMMAND))
        {
            RECT rcWnd;
            RECT rcChildWnd;

            Align((HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0));

            ptMouse.x = m_nScaleMouseX << pChData->pBitmap->Scale();
            ptMouse.y = m_nScaleMouseY << pChData->pBitmap->Scale();
            ClientToScreen(hWnd, &ptMouse);
        
            GetWindowRect(FrameWnd.m_hwndView, &rcWnd);
            GetWindowRect(hWnd, &rcChildWnd);
            if(rcChildWnd.left > rcWnd.left)
            {
                rcWnd.left = rcChildWnd.left;
            }
            if(rcChildWnd.right < rcWnd.right)
            {
                rcWnd.right = rcChildWnd.right;
            }
            if(rcChildWnd.top > rcWnd.top)
            {
                rcWnd.top = rcChildWnd.top;
            }
            if(rcChildWnd.bottom < rcWnd.bottom)
            {
                rcWnd.bottom = rcChildWnd.bottom;
            }

            if(ptMouse.x < rcWnd.left)
            {
                ptMouse.x = rcWnd.left;
            }
            else if(ptMouse.x > rcWnd.right)
            {
                ptMouse.x = rcWnd.right - 1; 
            }
            if(ptMouse.y < rcWnd.top)
            {
                ptMouse.y = rcWnd.top;
            }
            else if(ptMouse.y > rcWnd.bottom)
            {
                ptMouse.y = rcWnd.bottom - 1; 
            }
            ScreenToClient(hWnd, &ptMouse);
            OnChildMouseMove(hWnd, wp, (LPARAM)(ptMouse.x + (ptMouse.y << 16)));
        }
        else
        {
            pChData->pBitmap->GetMouseFromScaleMouse(m_nScaleMouseX, m_nScaleMouseY,
                    (int*)&ptMouse.x, (int*)&ptMouse.y);
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        }
        ClientToScreen(hWnd, &ptMouse);
        SetCursorPos(ptMouse.x, ptMouse.y);

		char szName[300];
		if(pChData->pBitmap->Scale())
		{
			// [MSV] sprintf_s(szName, 300, "%s (+%u)", pChData->szName, 1 << pChData->pBitmap->Scale());
			sprintf(szName,  "%s (+%u)", pChData->szName, 1 << pChData->pBitmap->Scale());
		}
		else
		{
			strcpy(szName, pChData->szName);
		}
		SetWindowText(hWnd, szName);
    }
    
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildLButtonUp(HWND hWnd, WPARAM wp, LPARAM lp)
{
    m_hwndMouse = hWnd;
    m_nMouseX = LOWORD(lp);
    m_nMouseY = HIWORD(lp);
    m_nMouseKey = (int)wp;
    if(m_hCurCursor == m_hCursorMoveHand)
    {
        m_hCurCursor = m_hCursorFlatHand;
        SetCursor(m_hCurCursor);
        ReleaseCapture();
    }
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildRButtonDown(HWND hWnd, WPARAM wp, LPARAM lp)
{
    m_hwndMouse = hWnd;
    m_nMouseX = LOWORD(lp);
    m_nMouseY = HIWORD(lp);
    m_nMouseKey = (int)wp;
    SendMessage(m_hwndView, WM_MDIACTIVATE, (WPARAM)hWnd, 0);
    POINT pt;
    pt.x = LOWORD(lp);
    pt.y = HIWORD(lp);
    ClientToScreen(hWnd, &pt);
    TrackPopupMenu(m_hPopupMenu, 0, pt.x, pt.y, 0, m_hwndFrame, NULL);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnChildSize(HWND hWnd, WPARAM, LPARAM) // Не вызывается.
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(hWnd, 0);
    if(pChData == NULL || pChData->pBitmap == NULL)
    {
        return FALSE;
    }

    return NULL;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Align(HWND hWnd)
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(hWnd, 0);
    if(pChData == NULL || pChData->pBitmap == NULL)
    {
        return NULL;
    }
    int nWidth = (pChData->pBitmap->Width() << pChData->pBitmap->Scale()) + 
            GetSystemMetrics(SM_CXSIZEFRAME) * 2;
    int nHeight = (pChData->pBitmap->Height() << pChData->pBitmap->Scale()) + 
            GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME) * 2;

    SetWindowPos(hWnd, NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER | SWP_NOMOVE);

    pChData->pBitmap->MoveImage();

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::CreateChildWindow(PSTR szName, int nID, 
        int nWidth, int nHeight, int nType, RGBQUAD *prgbPalette)
{
    HWND hwndChild;
    S_ChildData *pChData = NULL;

	hwndChild = FindWindowData(nID, &pChData);
    if(hwndChild && pChData && 
                (pChData->pBitmap->Width() == nWidth) &&
                (pChData->pBitmap->Height() == nHeight) &&
                (pChData->pBitmap->Type() == nType))
   {
        if(prgbPalette)
        {
            pChData->pBitmap->SetPalette(prgbPalette);
        }
		strncpy(pChData->szName, szName, 255);
        SetWindowText(hwndChild, szName);
        return TRUE;
    }
    if(hwndChild)
    {
        SendMessage(GetParent(hwndChild), WM_MDIDESTROY, (WPARAM)hwndChild, 0);
        DrawMenuBar(m_hwndFrame);
        UpdateWindow(m_hwndFrame);
    }

    MDICREATESTRUCT mdics;

    mdics.szClass = "VShellChildWndClass";
    mdics.szTitle = szName;
    mdics.hOwner = m_hInstance;
    mdics.x = CW_USEDEFAULT;
    mdics.y = CW_USEDEFAULT;
    mdics.cx = CW_USEDEFAULT;
    mdics.cy = CW_USEDEFAULT;
    mdics.style = 0;
    mdics.lParam = 0;
    
    if((hwndChild = (HWND)SendMessage(m_hwndView, WM_MDICREATE, 0,
            (LPARAM)(LPMDICREATESTRUCT)&mdics)) == NULL)
    {
        return FALSE;
    }
    SendMessage(hwndChild, WM_SETICON, ICON_BIG, (LPARAM)LoadImage(m_hInstance, 
            MAKEINTRESOURCE(IDI_VSHELL), IMAGE_ICON, 48, 48, 0));
    SendMessage(hwndChild, WM_SETICON, ICON_BIG, (LPARAM)LoadImage(m_hInstance, 
            MAKEINTRESOURCE(IDI_VSHELL), IMAGE_ICON, 32, 32, 0));
    SendMessage(hwndChild, WM_SETICON, ICON_SMALL, (LPARAM)LoadImage(m_hInstance, 
            MAKEINTRESOURCE(IDI_VSHELL), IMAGE_ICON, 16, 16, 0));

    if((pChData = (S_ChildData*)GetWindowLongPtr(hwndChild, 0)) == NULL)
    {
        return FALSE;
    }

    pChData->nID = nID;
	strncpy(pChData->szName, szName, 255);
    pChData->pBitmap = new C_BitmapEx();

    S_VShellWindow *pWindow = NULL;
    for(int i=0; i<m_Context.nWindows; i++)
    {
        if(m_Context.pWindow[i].nID == nID)
        {
            pWindow = &m_Context.pWindow[i];
            break;
        }
    }
    if(pWindow)
    {
        pChData->pBitmap->m_VG.m_Vectors.fShow = pWindow->fVectors;
        pChData->pBitmap->m_VG.m_Vectors.crColor = pWindow->VectorColor;
        pChData->pBitmap->m_VG.m_Grid = pWindow->Grid;
        pChData->pBitmap->SetAdjust(&pWindow->Adjust);
        pChData->pBitmap->m_fShowOverflows = pWindow->fShowOverflows;
        if(pWindow->fMinimized)
        {
            ShowWindow(hwndChild, SW_MINIMIZE);
        }
        else
        {
            MoveWindow(hwndChild, pWindow->ptPos.x, pWindow->ptPos.y,
                    pWindow->ptPos.x, pWindow->ptPos.y, TRUE);
        }
        if(pChData->pBitmap->m_VG.m_Vectors.fShow)
        {
            SendMessage(FrameWnd.m_hwndFrame, WM_COMMAND, (WPARAM)MMO_VECTORS, 0);
        }
        if(pChData->pBitmap->m_VG.m_Grid.fShow)
        {
            SendMessage(FrameWnd.m_hwndFrame, WM_COMMAND, (WPARAM)MMO_GRID, 0);
        }
    }
    pChData->pBitmap->Init(hwndChild, nWidth, nHeight, nType, prgbPalette);
    Align(hwndChild);

    SendMessage(m_hwndView, WM_MDISETMENU, (WPARAM)m_hFrameMenu, 
        (LPARAM)GetSubMenu(m_hFrameMenu, WINDOW_MENU_POS));
    DrawMenuBar(m_hwndFrame);
    UpdateWindow(m_hwndFrame);

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::DeleteChildWindow(int nID)
{
    S_ChildData *pChData;
    HWND hWnd;
    if((hWnd = FindWindowData(nID, &pChData)) == NULL)
    {
        return TRUE;
    }
    
    SendMessage(GetParent(hWnd), WM_MDIDESTROY, (WPARAM)hWnd, 0);
    DrawMenuBar(m_hwndFrame);
    UpdateWindow(m_hwndFrame);
    
    return NULL;
}
//------------------------------------------------------------------------
int C_FrameWnd::Iteration()
{
    int nRes = 0;
    static int nFrames = 0;
    static int nTime = GetTickCount();
    static float fltFPS = 0.0f;

    for(;;)
    {
        if(!m_fVSRun)
        {
            return nRes;
        }
        if(m_fRunVideo)
        {
            if(m_fBack)
            {
                nRes |= VS_BACK;
            }
            else
            {
                nRes |= VS_FORWARD;
            }
            break;
        }
        if(m_fStepVideo)
        {
            nRes |= VS_STEP;
            m_fStepVideo = FALSE;
            break;
        }
        Sleep(100);
    }

    if(m_fPauseVideo)
    {
        nRes |= VS_PAUSE;
    }
    
    GetNextStreamFrame(TRUE, m_fBack, m_fPauseVideo);
    
    nFrames++;
    if((nFrames & 0xF) == 0)
    {
        fltFPS = 16000.0f / (float)(GetTickCount() - nTime);
        nTime = GetTickCount();
    }
    char szMes[40];
    sprintf(szMes, "Src:%d, Cnt:%d, FPS:%f", m_FG.GetFrameNum() + 1, nFrames, fltFPS);
    SendMessage(m_hwndStatus, SB_SETTEXT, 3, (LPARAM)szMes);

    return nRes;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Draw(int nID)
{
    HWND hWnd;
    int i = 0;
    S_ChildData *pChData;

    if(nID != VS_DRAW_ALL)
    {
        if((FindWindowData(nID, &pChData)) == NULL)
        {
            return TRUE;
        }
        if(pChData != NULL && pChData->pBitmap != NULL)
        {
            pChData->pBitmap->m_VG.Swap();
            pChData->pBitmap->Draw();
        }
    }
    else 
    {
        while(hWnd = GetDlgItem(m_hwndView, FIRSTCHILD_ID + i))
        {
            pChData = (S_ChildData*)GetWindowLongPtr(hWnd, 0);
            if(pChData != NULL && pChData->pBitmap != NULL)
            {
                pChData->pBitmap->m_VG.Swap();
                pChData->pBitmap->Draw();
            }   
            i++;
        }
    }

    if(m_fSaveSequence)
    {
        if((FindWindowData(m_nSaveID, &pChData)) == NULL)
        {
            return TRUE;
        }
        if(!lstrcmp(&m_szSaveFileName[lstrlen(m_szSaveFileName) - 3], "avi"))
        {
            pChData->pBitmap->SaveImage(m_szSaveFileName);
        }
        else
        {    
            lstrcat(m_szSaveFileName, ".bmp");
            pChData->pBitmap->SaveImage(m_szSaveFileName);
            m_szSaveFileName[lstrlen(m_szSaveFileName) - 4] = '\0';
            int nCurNum = atoi(&m_szSaveFileName[lstrlen(m_szSaveFileName) - 4]);
            nCurNum++;
            if(nCurNum > 9999)
            {
                nCurNum = 0;
            }
            sprintf(&(m_szSaveFileName[lstrlen(m_szSaveFileName) - 4]), 
                    "%04d", nCurNum);
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Bind(PSTR szFileName)
{
    int nW;

    SetCurrentDirectory(m_szExeDir);
    if(!m_FG.Bind(m_hwndView, szFileName))
    {
		SetCurrentDirectory(m_szExeDir);
        return FALSE;
    }
	SetCurrentDirectory(m_szExeDir);
    m_nStreamType = 0;
    m_nStreamBPP = 0;
    m_nStreamBufSize = 0;

    m_FG.GetPalette(m_pStreamPal);
    
    int nWidth = m_FG.GetWidth();
    int nHeight = m_FG.GetHeight();
    int nBitCount = m_FG.GetBitCount();

    
    switch(nBitCount)
    {
    case 1:
        m_nStreamType = VS_RGB1;
        m_nStreamBPP = 1;
        nW = nWidth >> 3;
        if(nWidth & 0x7)
        {
            nW++;
        }
        m_nStreamBufSize = nW * nHeight;
        break;
    case 4:
        m_nStreamType = VS_RGB4;
        m_nStreamBPP = 4;
        nW = nWidth >> 1;
        if(nWidth & 0x1)
        {
            nW++;
        }
        m_nStreamBufSize = nW * nHeight;
        break;
    case 8:
        m_nStreamType = VS_RGB8;
        m_nStreamBPP = 8;
        m_nStreamBufSize = nWidth * nHeight;
        break;
    case 16:
        m_nStreamType = VS_RGB16;
        m_nStreamBPP = 16;
        m_nStreamBufSize = nWidth * nHeight * 2;
        break;
    case 24:
        m_nStreamType = VS_RGB24;
        m_nStreamBPP = 24;
        m_nStreamBufSize = nWidth * nHeight * 3;
        break;
    case 32:
        m_nStreamType = VS_RGB32;
        m_nStreamBPP = 32;
        m_nStreamBufSize = nWidth * nHeight * 4;
        break;
    default:
        return FALSE;
    }

    delete [] m_pbStreamBuffer;
    if((m_pbStreamBuffer = new BYTE[m_nStreamBufSize]) == NULL)
    {
        return FALSE;
    }
    ZeroMemory(m_pbStreamBuffer, m_nStreamBufSize);
    m_fBind = TRUE;

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::GetNextStreamFrame(BOOL fIter, BOOL fBack, BOOL fPause)
{
    if(fIter)
    {
        if(fPause)
        {
            return m_FG.GetFrame(m_pbStreamBuffer, FG_CUR);
        }
        if(fBack)
        {
            return m_FG.GetFrame(m_pbStreamBuffer, FG_PREV);
        }
        else
        {
            return m_FG.GetFrame(m_pbStreamBuffer, FG_NEXT);
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetData(int nID, void *pvData)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->SetData(pvData);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::GetData(int nID, void *pvData)
{
    if(nID == VS_SOURCE)
    {
        memcpy(pvData, m_pbStreamBuffer, m_nStreamBufSize);
        return TRUE;
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->GetData(pvData);
}
//------------------------------------------------------------------------
static BYTE RGB2Y(BYTE bR, BYTE bG, BYTE bB) 
{
    return (BYTE)(0.299 * (double)bR + 0.587 * (double)bG + 0.114 * (double)bB + 0.5);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::GetGrayData(int nID, void *pvData)
{
    if(nID == VS_SOURCE)
    {
        if(!pvData || !m_pStreamPal || !m_pbStreamBuffer)
        {
            return FALSE;
        }

        BYTE pbTab[256], bVal;
        BYTE *pbDstData = (BYTE*)pvData;
        BYTE *pbSrcData = m_pbStreamBuffer;
        WORD *pwSrcData;
        int i, j, k, m, n;
        WORD wVal;
        int nWidth, nHeight, nLineWidth;

        nWidth = m_FG.GetWidth();
        nHeight = m_FG.GetHeight();
        nLineWidth = m_FG.GetLineWidth();

        switch(m_nStreamType)
        {
        case VS_RGB1:
            for(i=0; i<2; i++)
            {
                pbTab[i] = RGB2Y(m_pStreamPal[i].rgbRed, m_pStreamPal[i].rgbGreen, m_pStreamPal[i].rgbBlue);
            }
            
            k = nWidth >> 3;
            for(i=0; i<nHeight; i++)
            {
                m = nWidth;
                for(j=0; j<k; j++, m-=8)
                {
                    bVal = pbSrcData[i * nLineWidth + j];
                    for(n=0; n<8; n++)
                    {
                        *pbDstData++ = pbTab[(bVal >> 7) & 1];
                        bVal <<= 1;
                    }
                }
                bVal = pbSrcData[i * nLineWidth + j];
                for(j=0; j<m; j++)
                {
                    *pbDstData++ = pbTab[(bVal >> 7) & 1];
                    bVal <<= 1;
                }
            }
            return TRUE;
        case VS_RGB4:
            for(i=0; i<16; i++)
            {
                pbTab[i] = RGB2Y(m_pStreamPal[i].rgbRed, m_pStreamPal[i].rgbGreen, m_pStreamPal[i].rgbBlue);
            }

            k = nWidth >> 1;
            for(i=0; i<nHeight; i++)
            {
                for(j=0; j<k; j++)
                {
                    bVal = pbSrcData[i*nLineWidth+j];
                    *pbDstData++ = pbTab[bVal >> 4];
                    *pbDstData++ = pbTab[bVal & 0xF];
                }
                if(nWidth & 1)
                {
                    *pbDstData++ = pbTab[pbSrcData[i*nLineWidth+j] >> 4];
                }
            }
            return TRUE;
        case VS_RGB8:
            for(i=0; i<256; i++)
            {
                pbTab[i] = RGB2Y(m_pStreamPal[i].rgbRed, m_pStreamPal[i].rgbGreen, m_pStreamPal[i].rgbBlue);
            }
            for(i=0; i<m_nStreamBufSize; i++)
            {
                *pbDstData++ = pbTab[*pbSrcData++];
            }
            return TRUE;
        case VS_RGB16:
            pwSrcData = (WORD*)pbSrcData;
            k = nWidth * nHeight;
            for(i=0; i<k; i++)
            {
                wVal = pwSrcData[i];
                *pbDstData++ = RGB2Y((BYTE)((wVal & 0x7C00) >> 7), (BYTE)((wVal & 0x3E0) >> 2), (BYTE)((wVal & 0x1F) << 3));
            }
            return TRUE;
        case VS_RGB24:
            k = nWidth * nHeight;
            for(i=0, j=0; i<k; i++, j+=3)
            {
                *pbDstData++ = RGB2Y(pbSrcData[j+2], pbSrcData[j+1], pbSrcData[j]);
            }
            return TRUE;
        case VS_RGB32:
            k = nWidth * nHeight;
            for(i=0, j=0; i<k; i++, j+=4)
            {
                *pbDstData++ = RGB2Y(pbSrcData[j+2], pbSrcData[j+1], pbSrcData[j]);
            }
            return TRUE;
        default:
            return FALSE;
        }
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->GetGrayData(pvData);    
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::GetBGRData(int nID, void *pvData)
{
    if(nID == VS_SOURCE)
    {
        if(!pvData || !m_pStreamPal || !m_pbStreamBuffer)
        {
            return FALSE;
        }

        BYTE bVal;
        BYTE *pbDstData = (BYTE*)pvData;
        BYTE *pbSrcData = m_pbStreamBuffer;
        WORD *pwSrcData;
        int i, j, k, m, n;
        WORD wVal;
        int nWidth, nHeight, nLineWidth;
        RGBQUAD rgbQuad;

        nWidth = m_FG.GetWidth();
        nHeight = m_FG.GetHeight();

        switch(m_nStreamType)
        {
        case VS_RGB1:
            nLineWidth = ((nWidth + 31) & ~31) >> 3;
            k = nWidth >> 3;
            for(i=0; i<nHeight; i++)
            {
                m = nWidth;
                for(j=0; j<k; j++, m-=8)
                {
                    bVal = pbSrcData[i * nLineWidth + j];
                    for(n=0; n<8; n++)
                    {
                        rgbQuad = m_pStreamPal[(bVal >> 7) & 1];
                        *pbDstData++ = rgbQuad.rgbBlue;
                        *pbDstData++ = rgbQuad.rgbGreen;
                        *pbDstData++ = rgbQuad.rgbRed;
                        bVal <<= 1;
                    }
                }
                bVal = pbSrcData[i * nLineWidth + j];
                for(j=0; j<m; j++)
                {
                    rgbQuad = m_pStreamPal[(bVal >> 7) & 1];
                    *pbDstData++ = rgbQuad.rgbBlue;
                    *pbDstData++ = rgbQuad.rgbGreen;
                    *pbDstData++ = rgbQuad.rgbRed;
                    bVal <<= 1;
                }
            }
            return TRUE;
        case VS_RGB4:
            nLineWidth = ((nWidth + 7) & ~7) >> 1;
            k = nWidth >> 1;
            for(i=0; i<nHeight; i++)
            {
                for(j=0; j<k; j++)
                {
                    bVal = pbSrcData[i*nLineWidth+j];
                    rgbQuad = m_pStreamPal[bVal >> 4];
                    *pbDstData++ = rgbQuad.rgbBlue;
                    *pbDstData++ = rgbQuad.rgbGreen;
                    *pbDstData++ = rgbQuad.rgbRed;
                    rgbQuad = m_pStreamPal[bVal & 0xF];
                    *pbDstData++ = rgbQuad.rgbBlue;
                    *pbDstData++ = rgbQuad.rgbGreen;
                    *pbDstData++ = rgbQuad.rgbRed;
                }
                if(nWidth & 1)
                {
                    rgbQuad = m_pStreamPal[pbSrcData[i*nLineWidth+j] >> 4];
                    *pbDstData++ = rgbQuad.rgbBlue;
                    *pbDstData++ = rgbQuad.rgbGreen;
                    *pbDstData++ = rgbQuad.rgbRed;
                }
            }
            return TRUE;
        case VS_RGB8:
            k = nWidth * nHeight;
            for(i=0; i<k; i++)
            {
                rgbQuad = m_pStreamPal[*pbSrcData++];
                *pbDstData++ = rgbQuad.rgbBlue;
                *pbDstData++ = rgbQuad.rgbGreen;
                *pbDstData++ = rgbQuad.rgbRed;
            }
            return TRUE;
        case VS_RGB16:
            pwSrcData = (WORD*)pbSrcData;
            k = nWidth * nHeight;
            for(i=0; i<k; i++)
            {
                wVal = pwSrcData[i];
                *pbDstData++ = (BYTE)((wVal & 0x1F) << 3);
                *pbDstData++ = (BYTE)((wVal & 0x3E0) >> 2);
                *pbDstData++ = (BYTE)((wVal & 0x7C00) >> 7);
            }
            return TRUE;
        case VS_RGB24:
            memcpy(pbDstData, m_pbStreamBuffer, m_nStreamBufSize);
            return TRUE;
        case VS_RGB32:
            k = nWidth * nHeight;
            for(i=0; i<k; i++)
            {
                *pbDstData++ = *pbSrcData++;
                *pbDstData++ = *pbSrcData++;
                *pbDstData++ = *pbSrcData++;
                pbSrcData++;
            }
            return TRUE;
        }
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->GetBGRData(pvData);
}
//------------------------------------------------------------------------
HWND C_FrameWnd::FindWindowData(int nID, S_ChildData **ppChData)
{
    HWND hWnd;
    int i = 0;

    *ppChData = NULL;
    if(!m_hwndView)
    {
        return NULL;
    }

    while(hWnd = GetDlgItem(m_hwndView, FIRSTCHILD_ID + i))
    {
        *ppChData = (S_ChildData*)GetWindowLongPtr(hWnd, 0);

        if(*ppChData && (nID == (*ppChData)->nID))
        {
            return hWnd;
        }
        i++;
    }
    *ppChData = NULL;
    return NULL;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::GetMouseStatus(int *pnID, int *pnX, int *pnY, int *pnScaleX, int *pnScaleY, 
        int *pnKey, int *pnCursor)
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(m_hwndMouse, 0);
    if(pChData == NULL)
    {
        return FALSE;
    }
    *pnID = pChData->nID;
    *pnX = m_nMouseX;
    *pnY = m_nMouseY;
    *pnScaleX = m_nScaleMouseX;
    *pnScaleY = m_nScaleMouseY;
    *pnKey = m_nMouseKey;
    if(m_hCurCursor == m_hCursorArrow)
    {
        *pnCursor = VS_CURSOR_ARROW;
    }
    else if(m_hCurCursor == m_hCursorScale)
    {
        *pnCursor = VS_CURSOR_MAGNIFY;
    }
    else
    {
        *pnCursor = VS_CURSOR_DRAG;
    }   
    
    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetVectors(int nID, int nVectorsize, int *pnVectors, COLORREF VectorsColor)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->SetVectors(nVectorsize, pnVectors, VectorsColor);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetVectors(int nID, int nVectorsize, float *pfltVectors, COLORREF VectorsColor)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->SetVectors(nVectorsize, pfltVectors, VectorsColor);
}
//------------------------------------------------------------------------
int C_FrameWnd::GetWidth(int nID)
{
    if(nID == VS_SOURCE)
    {
        return m_FG.GetWidth();
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return 0;
    }
    return pChData->pBitmap->Width();
}
//------------------------------------------------------------------------
int C_FrameWnd::GetHeight(int nID)
{
    if(nID == VS_SOURCE)
    {
        return m_FG.GetHeight();
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return 0;
    }
    return pChData->pBitmap->Height();
}
//------------------------------------------------------------------------
int C_FrameWnd::GetType(int nID)
{
    if(nID == VS_SOURCE)
    {
        return m_nStreamType;
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return 0;
    }
    return pChData->pBitmap->Type();
}
//------------------------------------------------------------------------
int C_FrameWnd::GetBitsPerPixel(int nID)
{
    if(nID == VS_SOURCE)
    {
        return m_nStreamBPP;
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return 0;
    }
    return pChData->pBitmap->BitCount();
}
//------------------------------------------------------------------------
int C_FrameWnd::GetBufSize(int nID)
{
    if(nID == VS_SOURCE)
    {
        return m_nStreamBufSize;
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return 0;
    }
    return pChData->pBitmap->BufSize();    
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetPalette(int nID, RGBQUAD *prgbPal)
{
    if(nID == VS_SOURCE)
    {
        if(m_nStreamBPP <= 8)
        {
            memcpy(m_pStreamPal, prgbPal, sizeof(RGBQUAD) * ((size_t)1 << m_nStreamBPP));
        }
        return TRUE;
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->SetPalette(prgbPal);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::GetPalette(int nID, RGBQUAD *prgbPal)
{
    if(nID == VS_SOURCE)
    {
        if(m_nStreamBPP <= 8)
        {
            memcpy(prgbPal, m_pStreamPal, sizeof(RGBQUAD) * ((size_t)1 << m_nStreamBPP));
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return 0;
    }
    return pChData->pBitmap->GetPalette(prgbPal);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetPixel(int nID, int nX, int nY, COLORREF crColor)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->SetPixel(nX, nY, crColor);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Line(int nID, int nX1, int nY1, int nX2, int nY2, COLORREF crColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Line(nX1, nY1, nX2, nY2, crColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Polyline(int nID, POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Polyline(pPoints, nPoints, crColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::PolyBezier(int nID, POINT *pPoints, int nPoints, COLORREF crColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->PolyBezier(pPoints, nPoints, crColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Arc(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
        COLORREF crColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Arc(nLeftRect, nTopRect, nRightRect, nBottomRect,
            nXStartArc, nYStartArc, nXEndArc, nYEndArc, crColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Rectangle(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Rectangle(nLeftRect, nTopRect, nRightRect, nBottomRect,
            crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Polygon(int nID, POINT *pptPoints, int nPoints, 
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Polygon(pptPoints, nPoints, crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Ellipse(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Ellipse(nLeftRect, nTopRect, nRightRect, nBottomRect,
            crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Chord(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Chord(nLeftRect, nTopRect, nRightRect, nBottomRect,
            nXRadial1, nYRadial1, nXRadial2, nYRadial2, crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Pie(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Pie(nLeftRect, nTopRect, nRightRect, nBottomRect,
            nXRadial1, nYRadial1, nXRadial2, nYRadial2, crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetPixel(int nID, float fltX, float fltY, COLORREF crColor)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->SetPixel(fltX, fltY, crColor);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Line(int nID, float fltX1, float fltY1, float fltX2, float fltY2, COLORREF crColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Line(fltX1, fltY1, fltX2, fltY2, crColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Polyline(int nID, float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Polyline(pfltPoints, nPoints, crColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::PolyBezier(int nID, float *pfltPoints, int nPoints, COLORREF crColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->PolyBezier(pfltPoints, nPoints, crColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Arc(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXStartArc, float fltYStartArc, float fltXEndArc, float fltYEndArc,
        COLORREF crColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Arc(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            fltXStartArc, fltYStartArc, fltXEndArc, fltYEndArc, crColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Rectangle(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Rectangle(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Polygon(int nID, float *pfltPoints, int nPoints, 
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Polygon(pfltPoints, nPoints, crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Ellipse(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Ellipse(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Chord(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXRadial1, float fltYRadial1,
        float fltXRadial2, float fltYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Chord(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            fltXRadial1, fltYRadial1, fltXRadial2, fltYRadial2, crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::Pie(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXRadial1, float fltYRadial1,
        float fltXRadial2, float fltYRadial2,
        COLORREF crBoundColor, COLORREF crFillColor, int nDashDot)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->Pie(fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            fltXRadial1, fltYRadial1, fltXRadial2, fltYRadial2, crBoundColor, crFillColor, nDashDot);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::TextOut(int nID, char *szText, int nX, int nY, int nSize, COLORREF clrColor)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->TextOut(szText, nX, nY, nSize, clrColor);
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetText(char *szText)
{
    int nStrLen = lstrlen(szText);
    if(!szText || (nStrLen == 0) || (nStrLen > TEXT_BUFFER_SIZE))
    {
        return FALSE;
    }
    
    if(m_hwndText == NULL)
    {
        MDICREATESTRUCT mdics;

        mdics.szClass = "VShellTextChildWndClass";
        mdics.szTitle = "Text";
        mdics.hOwner = m_hInstance;
        mdics.x = CW_USEDEFAULT;
        mdics.y = CW_USEDEFAULT;
        mdics.cx = CW_USEDEFAULT;
        mdics.cy = CW_USEDEFAULT;
        mdics.style = 0;
        mdics.lParam = 0;
    
        if((m_hwndText = (HWND)SendMessage(m_hwndView, WM_MDICREATE, 0,
                (LPARAM)(LPMDICREATESTRUCT)&mdics)) == NULL)
        {
            return FALSE;
        }

        SetWindowLongPtr(m_hwndText, GWL_STYLE, 
                GetWindowLongPtr(m_hwndText, GWL_STYLE) & (~WS_MAXIMIZEBOX));
        
        SendMessage(m_hwndView, WM_MDISETMENU, (WPARAM)m_hFrameMenu, 
            (LPARAM)GetSubMenu(m_hFrameMenu, WINDOW_MENU_POS));
        if(m_Context.fTexMinimized)
        {
            ShowWindow(m_hwndText, SW_MINIMIZE);
        }
        {
            MoveWindow(m_hwndText,
                    m_Context.rcTextRect.left,
                    m_Context.rcTextRect.top,
                    m_Context.rcTextRect.right - m_Context.rcTextRect.left,
                    m_Context.rcTextRect.bottom - m_Context.rcTextRect.top,
                    TRUE);
        }
        
        DrawMenuBar(m_hwndFrame);
        UpdateWindow(m_hwndFrame);
    }

    if((m_nTextBufferSize + nStrLen) > TEXT_BUFFER_SIZE)
    {
        int nCur = nStrLen;
        while(!isprint(m_szTextBuffer[nCur]))
        {
            nCur++;
        }
        nCur++;
        memmove(m_szTextBuffer, &m_szTextBuffer[nCur], lstrlen(m_szTextBuffer) - nCur + 1);
    }
    sprintf(&m_szTextBuffer[lstrlen(m_szTextBuffer)], "%s", szText);
    m_nTextBufferSize = lstrlen(m_szTextBuffer) + 1;
    SetWindowText(m_hwndTextEdit, m_szTextBuffer);

    SendMessage(m_hwndTextEdit, EM_LINESCROLL, 0, 
            SendMessage(m_hwndTextEdit, EM_GETLINECOUNT, 0, 0));

    if(m_nTextBufferSize > TEXT_BUFFER_SIZE)
    {
       MessageBox(NULL, "Text Overflow", "", MB_OK);
    }

    return TRUE;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnTextCreate(HWND hWnd, WPARAM, LPARAM)
{
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadImage(m_hInstance, 
            MAKEINTRESOURCE(IDI_VSHELL), IMAGE_ICON, 48, 48, 0));    
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadImage(m_hInstance, 
            MAKEINTRESOURCE(IDI_VSHELL), IMAGE_ICON, 32, 32, 0));
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadImage(m_hInstance, 
            MAKEINTRESOURCE(IDI_VSHELL), IMAGE_ICON, 16, 16, 0));

    m_hwndTextEdit = CreateWindow("edit", NULL, 
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
            ES_LEFT | ES_MULTILINE | 
            ES_AUTOHSCROLL | ES_AUTOVSCROLL,
            0, 0, 0, 0,
            hWnd,(HMENU) 1,
            m_hInstance, NULL);

    m_hTextFont = CreateFont(15, 7, 0, 0, FW_NORMAL, 0, 0, 0,
            RUSSIAN_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS,
            PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Courier New");
    SendMessage(m_hwndTextEdit, WM_SETFONT, WPARAM(m_hTextFont), TRUE);

    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnTextSize(HWND, WPARAM, LPARAM lp)
{
    RECT rc;

    SetWindowPos(m_hwndTextEdit, NULL, 0, 0, LOWORD(lp), HIWORD(lp), 
            SWP_NOMOVE | SWP_NOZORDER);
    rc.left = rc.top = 0;
    rc.right = LOWORD(lp);
    rc.bottom = HIWORD(lp);
    SendMessage(m_hwndTextEdit, EM_SETRECT, 0, (LPARAM)&rc);
    return NULL;
}
//------------------------------------------------------------------------
LRESULT C_FrameWnd::OnTextClose(HWND hWnd, WPARAM, LPARAM)
{
    if(IsIconic(hWnd))
    {
        return NULL;
    }
    SendMessage(m_hwndView, WM_MDINEXT, (WPARAM)hWnd, (LPARAM)1); 
    if(hWnd != (HWND)SendMessage(m_hwndView, WM_MDIGETACTIVE, 0, 0))
    {
        ShowWindow(hWnd, SW_MINIMIZE);
    }
    return NULL;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::CorrectSignedImage(int nID)
{
    S_ChildData *pChData;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    return pChData->pBitmap->CorrectSignedImage();
}
//------------------------------------------------------------------------
void C_FrameWnd::SetMenuState(int nID, BOOL fEnable, BOOL fCheck)
{
    EnableMenuItem(m_hFrameMenu, nID, fEnable ? MF_ENABLED : MF_GRAYED);
    CheckMenuItem(m_hFrameMenu, nID, fCheck ? MF_CHECKED : MF_UNCHECKED);
    EnableMenuItem(m_hPopupMenu, nID, fEnable ? MF_ENABLED : MF_GRAYED);
    CheckMenuItem(m_hPopupMenu, nID, fCheck ? MF_CHECKED : MF_UNCHECKED);
    ToolbarWnd.SetState(nID, fEnable, fCheck);
}
//------------------------------------------------------------------------
int C_FrameWnd::GetSrcFrames()
{
    return m_FG.GetFrames();
}
//------------------------------------------------------------------------
int C_FrameWnd::GetSrcFrameNum()
{
    return m_FG.GetFrameNum() + 1;
}
//------------------------------------------------------------------------
void C_FrameWnd::SetCircleColors(COLORREF *pColor, int nColorCount, int nColorDisp)
{
	int* vR=new int[nColorCount];
	int* vG=new int[nColorCount];
	int* vB=new int[nColorCount];
	
	memset(vR,0,nColorCount*4);
	memset(vG,0,nColorCount*4);
	memset(vB,0,nColorCount*4);
	int a;
	double k=nColorCount/360.;
	// RED
	for(a=int(120*k); a<int(240*k); a++){
		vR[a]=255;
	}
	for(a=int(60*k); a<int(120*k); a++){
		vR[a]=int((a-60*k)*255./60./k);
	}
	for(a=int(240*k); a<int(300*k); a++){
		vR[a]=int((300*k-a)*255./60./k);
	}
	
	// GREEN
	for(a=int(240*k); a<int(360*k); a++){
		vG[a]=255;
	}
	for(a=int(180*k); a<int(240*k); a++){
		vG[a]=int((a-180*k)*255./60./k);
	}
	for(a=int(0); a<int(60*k); a++){
		vG[a]=int((60*k-a)*255./60./k);
	}

	// BLUE
	for(a=int(0); a<int(120*k); a++){
		vB[a]=255;
	}
	for(a=int(300*k); a<int(360*k); a++){
		vB[a]=int((a-300*k)*255./60./k);
	}
	for(a=int(120*k); a<int(180*k); a++){
		vB[a]=int((180*k-a)*255./60./k);
	}

	int i;
	nColorDisp+=int(210*k);
	nColorDisp%=nColorCount;
	for(i=0; i<nColorCount-nColorDisp; i++){
		int indx=i+nColorDisp;
		pColor[indx]=RGB(vR[i],vG[i],vB[i]);
	}
	for(i=nColorCount-nColorDisp; i<nColorCount; i++){
		int indx=i-(nColorCount-nColorDisp);
		pColor[indx]=RGB(vR[i],vG[i],vB[i]);
	}
	
	delete [] vR;
	delete [] vB;
	delete [] vG;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetCirclePalette(int nID, int nColorDisp)
{
    S_ChildData *pChData;
    COLORREF *pColor;
    RGBQUAD *pPalette;
    int nColors;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    
    switch(pChData->pBitmap->Type())
    {
    case VS_RGB1:
        nColors = 2;
        break;
    case VS_RGB4:
        nColors = 16;
        break;
    case VS_RGB8:
    case VS_RGB8_8:
    case VS_RGB8_16:
    case VS_RGB8_32:
        nColors = 256;
        break;
    default:
        return TRUE;
    }

    pColor = new COLORREF[nColors];
    pPalette = new RGBQUAD[nColors];
	SetCircleColors(pColor, nColors, nColorDisp);
	for(int i=0; i<nColors; i++)
    {
		pPalette[i].rgbBlue= GetBValue(pColor[i]);
		pPalette[i].rgbGreen=GetGValue(pColor[i]);
		pPalette[i].rgbRed  =GetRValue(pColor[i]);
	}
    pChData->pBitmap->SetPalette(pPalette);

	delete [] pColor;
    delete [] pPalette;

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetRandomPalette(int nID)
{
    S_ChildData *pChData;
    RGBQUAD *pPalette;
    int nColors;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    
    switch(pChData->pBitmap->Type())
    {
    case VS_RGB1:
        nColors = 2;
        break;
    case VS_RGB4:
        nColors = 16;
        break;
    case VS_RGB8:
    case VS_RGB8_8:
    case VS_RGB8_16:
    case VS_RGB8_32:
        nColors = 256;
        break;
    default:
        return TRUE;
    }

    pPalette = new RGBQUAD[nColors];
    
    srand((unsigned)time(NULL));
	for(int i=0; i<nColors; i++)
    {
		pPalette[i].rgbBlue = rand() & 0xFF;
		pPalette[i].rgbGreen = rand() & 0xFF;
		pPalette[i].rgbRed = rand() & 0xFF;
	}
    pChData->pBitmap->SetPalette(pPalette);

    delete [] pPalette;

    return TRUE;
}
//------------------------------------------------------------------------
BOOL C_FrameWnd::SetGrayPalette(int nID)
{
    S_ChildData *pChData;
    RGBQUAD *pPalette;
    int i;
    
    if((FindWindowData(nID, &pChData)) == NULL)
    {
        return FALSE;
    }
    
    switch(pChData->pBitmap->Type())
    {
    case VS_RGB1:
        pPalette = new RGBQUAD[2];
        pPalette[0].rgbBlue = 0;
        pPalette[0].rgbGreen = 0;
        pPalette[0].rgbRed = 0;
        pPalette[1].rgbBlue = 255;
        pPalette[1].rgbGreen = 255;
        pPalette[1].rgbRed = 255;
        break;
    case VS_RGB4:
        pPalette = new RGBQUAD[16];
        for(i=0; i<16; i++)
        {
            pPalette[i].rgbBlue = (BYTE)(i * 17);
            pPalette[i].rgbGreen = (BYTE)(i * 17);
            pPalette[i].rgbRed = (BYTE)(i * 17);
        }
        break;
    case VS_RGB8:
    case VS_RGB8_8:
    case VS_RGB8_16:
    case VS_RGB8_32:
        pPalette = new RGBQUAD[256];
        for(i=0; i<256; i++)
        {
            pPalette[i].rgbBlue = (BYTE)i;
            pPalette[i].rgbGreen = (BYTE)i;
            pPalette[i].rgbRed = (BYTE)i;
        }
        break;
    default:
        return TRUE;
    }

    pChData->pBitmap->SetPalette(pPalette);
    delete [] pPalette;

    return TRUE;
}
//------------------------------------------------------------------------
void C_FrameWnd::Seek(int nFrameNum) {
	m_FG.Seek(nFrameNum);
}
    //--------------------------------------------------------------------
#include <windows.h>
#include <commctrl.h>
#include "ToolbarWnd.h"
#include "resource.h"

    //--------------------------------------------------------------------
C_ToolbarWnd ToolbarWnd;

    //--------------------------------------------------------------------
static const char WINDOW_CLASS_NAME[]   = "ToolbarWndClass";
static const int TB_BUTTONS             = 20;
static const int TB_IMG_BUTTONS         = 17;
static const int TB_BUTTON_WIDTH        = 16;
static const int TB_BUTTON_HEIGHT       = 15;

#ifdef _WIN64
#define TBB(B, C) (B), (C), TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0, 0, 0, NULL, NULL 
#define TBS 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0, 0, 0, 0, NULL, NULL
#else
#define TBB(B, C) (B), (C), TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, NULL, NULL 
#define TBS 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, NULL, NULL 
#endif

static const TBBUTTON tbb[TB_BUTTONS] = 
{
	TBB(0, MMO_RESET),
	TBB(1, MMO_RUN),
	TBB(2, MMO_PAUSE),
	TBB(3, MMO_STEP),
	TBB(4, MMO_RUN_BACK),
	TBB(5, MMO_STEP_BACK),
	TBS,
	TBB(6, MMO_DRAG),
	TBB(7, MMO_SCALE),
	TBB(8, MMO_ALIGN),
	TBS,
	TBB(9, MMO_VECTORS),
	TBB(10, MMO_GRID),
	TBB(11, MMO_SHOW_USER_GRAPHIC),
	TBB(12, MMO_RASTER),
	TBS,
	TBB(13, MMW_CONTROLS),
	TBB(14, MMF_SAVE_SETTING),
	TBB(15, MMF_SAVE_CONTROL_SETTING),
	TBB(16, MMF_LOAD_CONTROL_SETTING)
};

static const int TB_ENABLE_DEL[TB_BUTTONS] = 
{
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

static char pszTBCaptions[TB_BUTTONS][32];

    //--------------------------------------------------------------------
LRESULT CALLBACK ToolbarWndProc(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp)
{
    switch(nMsg)
    {
    case WM_CREATE:
        return ToolbarWnd.OnCreate(hWnd);
    case WM_DESTROY:
        return ToolbarWnd.OnDestroy();
    case WM_SIZE:
        return ToolbarWnd.OnSize(lp);
    case WM_NOTIFY:
        return ToolbarWnd.OnNotify(lp);
    case WM_COMMAND:
        SendMessage(ToolbarWnd.GetParentWnd(), WM_COMMAND, wp, lp);
        return NULL;
    }

    return DefWindowProc(hWnd, nMsg, wp, lp);
}

    //--------------------------------------------------------------------
C_ToolbarWnd::C_ToolbarWnd()
{
    m_hWnd = NULL;
    m_hwndToolbar = NULL;
    m_hwndToolTip = NULL;
	m_hBackBrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
}

    //--------------------------------------------------------------------
C_ToolbarWnd::~C_ToolbarWnd()
{
	DeleteObject(m_hBackBrush);
}

    //--------------------------------------------------------------------
BOOL C_ToolbarWnd::RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASSEX wc;
	
    m_hInstance = hInstance;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ToolbarWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance;
    wc.hIcon = NULL;   
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = m_hBackBrush;                            
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIconSm = NULL;

    if(!RegisterClassEx(&wc))
    {
        return FALSE;
    }

    return TRUE;
}

    //--------------------------------------------------------------------
BOOL C_ToolbarWnd::Create(HWND hwndParent)
{
    m_hwndParent = hwndParent;
    m_hWnd = CreateWindowEx(0, WINDOW_CLASS_NAME, "", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, 0, 0, 
            hwndParent, NULL, m_hInstance, NULL);

    return (m_hWnd != NULL);
}

    //--------------------------------------------------------------------
LRESULT C_ToolbarWnd::OnCreate(HWND hWnd)
{
    try
    {
            // Создаём панель инструментов...
		m_hwndToolbar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | 
                WS_CLIPSIBLINGS | CCS_TOP | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | CCS_ADJUSTABLE, 
                MAIN_TOOLBAR, TB_IMG_BUTTONS, m_hInstance, MAIN_TOOLBAR, tbb, 
                TB_BUTTONS, 0, 0, TB_BUTTON_WIDTH, TB_BUTTON_HEIGHT, sizeof(TBBUTTON));
        if(!m_hwndToolbar)
        {
            throw 0;
        }

        m_hwndToolTip = (HWND)SendMessage(m_hwndToolbar, TB_GETTOOLTIPS, 0, 0);

        LoadString(m_hInstance, IDS_TB_RESET, pszTBCaptions[0], 31);
        LoadString(m_hInstance, IDS_TB_RUN, pszTBCaptions[1], 31);
        LoadString(m_hInstance, IDS_TB_PAUSE, pszTBCaptions[2], 31);
        LoadString(m_hInstance, IDS_TB_STEP, pszTBCaptions[3], 31);
        LoadString(m_hInstance, IDS_TB_RUN_BACK, pszTBCaptions[4], 31);
        LoadString(m_hInstance, IDS_TB_STEP_BACK, pszTBCaptions[5], 31);
        ZeroMemory(pszTBCaptions[6], 32);
        LoadString(m_hInstance, IDS_TB_DRAG, pszTBCaptions[7], 31);
        LoadString(m_hInstance, IDS_TB_MAGNIFY, pszTBCaptions[8], 31);
        LoadString(m_hInstance, IDS_TB_ALIGN, pszTBCaptions[9], 31);
        ZeroMemory(pszTBCaptions[10], 32);
        LoadString(m_hInstance, IDS_TB_VECTORS, pszTBCaptions[11], 31);
        LoadString(m_hInstance, IDS_TB_GRID, pszTBCaptions[12], 31);
        LoadString(m_hInstance, IDS_TB_USER_GRAPHIC, pszTBCaptions[13], 31);
        LoadString(m_hInstance, IDS_TB_RASTER, pszTBCaptions[14], 31);
        ZeroMemory(pszTBCaptions[15], 32);
        LoadString(m_hInstance, IDS_TB_CONTROLS, pszTBCaptions[16], 31);
        LoadString(m_hInstance, IDS_TB_SAVE_VS_SETTINGS, pszTBCaptions[17], 31);
        LoadString(m_hInstance, IDS_TB_SAVE_CTRL_SETTINGS, pszTBCaptions[18], 31);
        LoadString(m_hInstance, IDS_TB_LOAD_CTRL_SETTINGS, pszTBCaptions[19], 31);
    }
    catch(...)
    {
    }

    return NULL;
}

    //--------------------------------------------------------------------
LRESULT C_ToolbarWnd::OnDestroy()
{
    PostQuitMessage(0);
    return NULL;
}

    //--------------------------------------------------------------------
LRESULT C_ToolbarWnd::OnSize(LPARAM lp)
{
    SendMessage(m_hwndToolbar, WM_SIZE, SIZE_RESTORED, lp);
    
    return NULL; 
}

    //--------------------------------------------------------------------
LRESULT C_ToolbarWnd::OnNotify(LPARAM lp)
{
    LRESULT Res = 0;
    int i;

    switch(((LPNMHDR)lp)->code)
    {
    case TTN_NEEDTEXT:
        for(i=0; i<TB_BUTTONS; i++)
        {
            if(tbb[i].idCommand == (int)((LPTOOLTIPTEXT)lp)->hdr.idFrom)
            {
                lstrcpy(((LPTOOLTIPTEXT)lp)->szText, pszTBCaptions[i]);
            }
        }
        break;
    case TBN_QUERYINSERT:
        Res = 1;
        break;
    case TBN_QUERYDELETE:
        if(TB_ENABLE_DEL[((LPTBNOTIFY)lp)->iItem])
        {
            Res = 1;
        }
        break;
    case TBN_GETBUTTONINFO:
        if(((LPTBNOTIFY)lp)->iItem < TB_BUTTONS)
        {
            lstrcpy(((LPTBNOTIFY)lp)->pszText, pszTBCaptions[((LPTBNOTIFY)lp)->iItem]);
            memcpy(&((LPTBNOTIFY)lp)->tbButton, &tbb[((LPTBNOTIFY)lp)->iItem], sizeof(TBBUTTON));
            Res = 1;
        }
        break;
    }
    return Res;
}

    //--------------------------------------------------------------------
void C_ToolbarWnd::SetState(int nID, BOOL fEnable, BOOL fCheck)
{
    SendMessage(m_hwndToolbar, TB_SETSTATE, nID, fEnable ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE);
    SendMessage(m_hwndToolbar, TB_CHECKBUTTON, nID, fCheck);
}

    //--------------------------------------------------------------------
void C_ToolbarWnd::SaveRestore(HKEY hKey, char *szSubKey, char *szKeyName)
{
    TBSAVEPARAMS tbs;
    tbs.hkr = hKey;
    tbs.pszSubKey = szSubKey;
    tbs.pszValueName = szKeyName;
    SendMessage(ToolbarWnd.m_hwndToolbar, TB_SAVERESTORE, (WPARAM)FALSE, (LPARAM)&tbs);
}

    //--------------------------------------------------------------------
void C_ToolbarWnd::Move(int nX, int nY, int nWidth, int nHeight)
{
    MoveWindow(m_hWnd, nX, nY, nWidth, nHeight, TRUE);
}

    //--------------------------------------------------------------------
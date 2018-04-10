    //--------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <commctrl.h>
#include "SeekSlider.h"

    //--------------------------------------------------------------------
static const int SLIDER_X               = 5;
static const int SLIDER_Y               = 10;
static const int SLIDER_WIDTH           = 200;
static const int SLIDER_HEIGHT          = 16;
static const int LABEL_WIDTH            = 100;
static const int FONT_HEIGHT            = 16;

    //--------------------------------------------------------------------
C_SeekSlider SeekSlider;

    //--------------------------------------------------------------------
LRESULT CALLBACK SeekSliderWndProc(HWND hWnd, UINT nMsg, WPARAM wp, LPARAM lp)
{
    switch(nMsg)
    {
    case WM_CREATE:
        return SeekSlider.OnCreate(hWnd, wp, lp);
    case WM_DESTROY:
        return SeekSlider.OnDestroy(hWnd, wp, lp);
    case WM_NOTIFY:
        if(((LPNMHDR)lp)->code == NM_CUSTOMDRAW)
        {
            return SeekSlider.OnSlider(lp);
        }
        break;
    }

    return DefWindowProc(hWnd, nMsg, wp, lp);
}

    //--------------------------------------------------------------------
C_SeekSlider::C_SeekSlider()
{
    m_pBitmapEx = NULL;
    m_hCtrlFont = NULL;
}

    //--------------------------------------------------------------------
C_SeekSlider::~C_SeekSlider()
{
    if(m_hCtrlFont)
    {
        DeleteObject(m_hCtrlFont);
    }
}

    //--------------------------------------------------------------------
BOOL C_SeekSlider::RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = SeekSliderWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);                            
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "VShellSeekSliderWndClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
        return FALSE;

    return TRUE;
}

    //--------------------------------------------------------------------
HWND C_SeekSlider::Create(HWND hParentWnd)
{
    m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME,
            "VShellSeekSliderWndClass", "SeekSlider", WS_CHILD | WS_VISIBLE, 
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            hParentWnd, NULL, m_hInstance, NULL);

    return m_hWnd;
}

    //--------------------------------------------------------------------
LRESULT C_SeekSlider::OnCreate(HWND hWnd, WPARAM, LPARAM)
{
    m_hCtrlFont = CreateFont(FONT_HEIGHT / 2, 5, 0, 0, FW_NORMAL, false, false, false, 
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
            DEFAULT_QUALITY, FIXED_PITCH+FF_SCRIPT, "MS SANS SERIF");

    m_hwndSlider = CreateWindow(TRACKBAR_CLASS, "", WS_CHILD | WS_VISIBLE,
            SLIDER_X, SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT, 
            hWnd, NULL, m_hInstance, NULL);
    m_hwndLabel = CreateWindow("static", "", WS_CHILD | WS_VISIBLE, 
            SLIDER_X + SLIDER_WIDTH + 5, SLIDER_Y, LABEL_WIDTH, SLIDER_HEIGHT,
            hWnd, NULL, m_hInstance, NULL);
    SendMessage(m_hwndSlider, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
    SendMessage(m_hwndLabel, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE); 

    return NULL;
}

    //--------------------------------------------------------------------
LRESULT C_SeekSlider::OnDestroy(HWND, WPARAM, LPARAM)
{
    PostQuitMessage(0);
    return NULL;
}

    //--------------------------------------------------------------------
LRESULT C_SeekSlider::OnSlider(LPARAM lp)
{
    int nPos;
    char szText[256];

    if(m_hwndSlider == NULL || ((LPNMHDR)lp)->hwndFrom != m_hwndSlider)
    {
        return NULL;
    }
        
	nPos = (int)SendMessage(m_hwndSlider, TBM_GETPOS, 0, 0);
    SetWindowText(m_hwndLabel, itoa(nPos, szText, 10));

    return NULL;
}

    //--------------------------------------------------------------------
BOOL C_SeekSlider::Update(C_BitmapEx *)
{
    return TRUE;
}

    //--------------------------------------------------------------------
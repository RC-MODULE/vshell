    //--------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include "ControlWnd.h"
#include "resource.h"
    //--------------------------------------------------------------------
C_ControlWnd ControlWnd;
static const int CONTROL_WIDTH          = 164;
static const int BOX_X                  = 10;
static const int BOX_Y_SPACE            = 5;
static const int SLIDER_BOX_HEIGHT      = 46;
static const int SLIDER_X               = 5;
static const int SLIDER_Y               = 12;
static const int SLIDER_HEIGHT          = 16;
static const int RADIO_X                = 5;
static const int RADIO_Y                = 15;
static const int RADIO_HEIGHT           = 20;
static const int CHECKBOX_BOX_HEIGHT    = 40;
static const int CHECKBOX_X             = 5;
static const int CHECKBOX_Y             = 15;
static const int CHECKBOX_HEIGHT        = 20;
static const int EDIT_BOX_HEIGHT        = 50;
static const int EDIT_X                 = 7;
static const int EDIT_Y                 = 15;
static const int EDIT_HEIGHT            = 16;
static const int FONT_HEIGHT            = 16;
static const int SCROLL_STEP            = 8;
static const int SCROLL_WIDTH           = 20;
static WNDPROC lpfnEditWndProc = NULL;
    //--------------------------------------------------------------------
LRESULT CALLBACK ControlWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
    case WM_CREATE:
        return ControlWnd.OnCreate(hWnd, wp, lp);
    case WM_SIZE:
        return ControlWnd.OnSize(hWnd, wp, lp);
    case WM_VSCROLL:
        return ControlWnd.OnVScroll(hWnd, wp, lp);
    case WM_QUERYENDSESSION:
    case WM_CLOSE:
        return ControlWnd.OnClose(hWnd, msg, wp, lp);
    case WM_DESTROY:
        return ControlWnd.OnDestroy(hWnd, wp, lp);
    case WM_NOTIFY:
        if(((LPNMHDR)lp)->code == NM_CUSTOMDRAW)
        {
            return ControlWnd.OnSlider(lp);
        }
        break;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}
    //--------------------------------------------------------------------
LRESULT CALLBACK EditWndProc(HWND hwnd, UINT msg, WPARAM wParam,
						 LPARAM lParam)
{
	if((msg == WM_KEYDOWN) &&(wParam == VK_RETURN))
    {
        ControlWnd.OnEditReturn(hwnd);
	}
	return CallWindowProc(lpfnEditWndProc, hwnd, msg, wParam, lParam); 
}
    //--------------------------------------------------------------------
C_ControlWnd::C_ControlWnd()
{
    int i;
    ZeroMemory(m_pSlider, sizeof(S_Slider) * VS_MAX_SLIDER);
    ZeroMemory(m_pRadioGroup, sizeof(S_RadioGroup) * VS_MAX_RADIO_GROUP);
    for(i=0; i<VS_MAX_RADIO_GROUP; i++)
    {
        ZeroMemory(m_pRadioGroup[i].pRadio, sizeof(S_Radio) * VS_MAX_RADIO);
    }
    ZeroMemory(m_pCheckBox, sizeof(S_CheckBox) * VS_MAX_CHECKBOX);
    ZeroMemory(m_pEdit, sizeof(S_Edit) * VS_MAX_EDIT);
    m_nSliders = 0;
    m_nRadioGroups = 0;
    m_nCheckBoxes = 0;
    m_nEdits = 0;
    m_hCtrlFont = NULL;
    m_nNextControlPos = BOX_Y_SPACE;
    m_nCurScrollPos = 0;
    m_nMaxScrollPos = 0;
    m_hBackBrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
}
    //--------------------------------------------------------------------
C_ControlWnd::~C_ControlWnd()
{
    if(m_hCtrlFont)
    {
        DeleteObject(m_hCtrlFont);
    }
    if(m_hBackBrush)
    {
        DeleteObject(m_hBackBrush);
    }
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ControlWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = m_hBackBrush;                            
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "MVBenchControlWndClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
        return FALSE;

    return TRUE;
}
    //--------------------------------------------------------------------
HWND C_ControlWnd::Create(HWND hParentWnd)
{
    m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME,
            "MVBenchControlWndClass", "Controls", 
            WS_CHILD | WS_VISIBLE | WS_VSCROLL,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            hParentWnd, NULL, m_hInstance, NULL);

    return m_hWnd;
}
    //--------------------------------------------------------------------
LRESULT C_ControlWnd::OnCreate(HWND hWnd, WPARAM, LPARAM)
{
    int i, j;
    DWORD dwGroup;

    m_hCtrlFont = CreateFont(FONT_HEIGHT / 2, 5, 0, 0, FW_NORMAL, false, false, false, 
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
            DEFAULT_QUALITY, FIXED_PITCH+FF_SCRIPT, "MS SANS SERIF");

    for(i=0; i<VS_MAX_SLIDER; i++)
    {    
        m_pSlider[i].hwndSlider = CreateWindow(TRACKBAR_CLASS, "", 
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
        m_pSlider[i].hwndBox = CreateWindow("button", "", 
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
        m_pSlider[i].hwndLabel = CreateWindow("static", "", 
                WS_CHILD | WS_VISIBLE | SS_CENTER, 
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
        SendMessage(m_pSlider[i].hwndSlider, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
        SendMessage(m_pSlider[i].hwndBox, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
        SendMessage(m_pSlider[i].hwndLabel, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE); 
        m_pSlider[i].nStartYPos = 0;
    }
    for(i=0; i<VS_MAX_RADIO_GROUP; i++)
    {    
        m_pRadioGroup[i].hwndBox = CreateWindow("button", "", 
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
        dwGroup = WS_GROUP;    
        for(j=0; j<VS_MAX_RADIO; j++)
        {
            m_pRadioGroup[i].pRadio[j].hWnd = CreateWindow("button", "", 
                WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | dwGroup, 
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
            SendMessage(m_pRadioGroup[i].pRadio[j].hWnd, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
            dwGroup = 0;
        }
        SendMessage(m_pRadioGroup[i].hwndBox, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
        m_pRadioGroup[i].nStartYPos = 0;
    }
    for(i=0; i<VS_MAX_CHECKBOX; i++)
    {    
        m_pCheckBox[i].hwndCheckBox = CreateWindow("button", "", 
                WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
        m_pCheckBox[i].hwndBox = CreateWindow("button", "", 
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
        SendMessage(m_pCheckBox[i].hwndCheckBox, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
        SendMessage(m_pCheckBox[i].hwndBox, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
        m_pCheckBox[i].nStartYPos = 0;
    }
    for(i=0; i<VS_MAX_EDIT; i++)
    {    
        m_pEdit[i].hwndEdit = CreateWindowEx(WS_EX_STATICEDGE, "edit", "", 
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
        m_pEdit[i].hwndBox = CreateWindow("button", "", 
                WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
        m_pEdit[i].hwndLabel = CreateWindow("static", "", 
                WS_CHILD | WS_VISIBLE | SS_CENTER, 
                0, 0, 0, 0, hWnd, NULL, m_hInstance, NULL);
        SendMessage(m_pEdit[i].hwndEdit, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
        SendMessage(m_pEdit[i].hwndBox, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
        SendMessage(m_pEdit[i].hwndLabel, WM_SETFONT, WPARAM(m_hCtrlFont), TRUE);
        /// Crash in x64 ---  lpfnEditWndProc = (WNDPROC)SetWindowLongPtr(m_pEdit[i].hwndEdit, GWLP_WNDPROC,    (LONG)EditWndProc);
        m_pEdit[i].nStartYPos = 0;
    }
    return NULL;
}
    //--------------------------------------------------------------------
LRESULT C_ControlWnd::OnSize(HWND hWnd, WPARAM, LPARAM lp)
{
    int nDif = m_nNextControlPos - HIWORD(lp);
    if(nDif <= 0)
    {
        m_nCurScrollPos = 0;
        m_nMaxScrollPos = 0;
    }
    else
    {
        m_nMaxScrollPos = nDif / SCROLL_STEP + 1;
    }
    SetScrollRange(hWnd, SB_VERT, 0, m_nMaxScrollPos, TRUE);
    SetScrollPos(hWnd, SB_VERT, m_nCurScrollPos, TRUE);
    ArrangeControls();

    return NULL;
}
    //--------------------------------------------------------------------
LRESULT C_ControlWnd::OnVScroll(HWND hWnd, WPARAM wp, LPARAM)
{
    switch(LOWORD(wp))
    {
    case SB_LINEDOWN:
        m_nCurScrollPos++;
        if(m_nCurScrollPos > m_nMaxScrollPos)
        {
            m_nCurScrollPos = m_nMaxScrollPos;
        }
        break;
    case SB_LINEUP:
        m_nCurScrollPos--;
        if(m_nCurScrollPos < 0)
        {
            m_nCurScrollPos = 0;
        }
        break;
    case SB_THUMBTRACK:
        m_nCurScrollPos = HIWORD(wp);
        break;
    }
    ArrangeControls();
    SetScrollPos(hWnd, SB_VERT, m_nCurScrollPos, TRUE);
    InvalidateRect(hWnd, NULL, TRUE);
    
    return NULL;
}
    //--------------------------------------------------------------------
LRESULT C_ControlWnd::OnClose(HWND, UINT, WPARAM, LPARAM)
{
    return NULL;
}
    //--------------------------------------------------------------------
LRESULT C_ControlWnd::OnDestroy(HWND, WPARAM, LPARAM)
{
    PostQuitMessage(0);
    return NULL;
}
    //--------------------------------------------------------------------
void C_ControlWnd::SetSliderParameters(S_Slider *pSlider, float fltMin, float fltMax, 
            float fltStep, float fltStartPos)
{
    int nMax = (int)((fltMax - fltMin) / fltStep + 0.5);
    int nStartPos;
    if(fltStartPos < fltMin)
    {
        nStartPos = 0;
        fltStartPos = fltMin;
    }
    else if(fltStartPos > fltMax)
    {
        nStartPos = nMax;
        fltStartPos = fltMax;
    }
    else
    {
        nStartPos = (int)(((float)nMax * (fltStartPos - fltMin)) / (fltMax - fltMin) + 0.5);
    }
    

    pSlider->fltMin = fltMin;
    pSlider->fltMax = fltMax;
    pSlider->fltStep = fltStep;
    pSlider->fltCurValue = fltStartPos;
    
    SendMessage(pSlider->hwndSlider, TBM_SETRANGEMIN, FALSE, 0);
    SendMessage(pSlider->hwndSlider, TBM_SETRANGEMAX, FALSE, nMax);
    SendMessage(pSlider->hwndSlider, TBM_SETPOS, TRUE, nStartPos);

    char szStr[32];
    sprintf(szStr, "%f", fltStartPos);
    SetWindowText(pSlider->hwndLabel, szStr);
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::CreateSlider(PSTR szName, int nID, 
        float fltMin, float fltMax, float fltStep, float fltStartPos)
{
    if(m_nSliders == VS_MAX_SLIDER)
    {
        return FALSE;
    }

    if((fltMin > fltMax) || (fltStep <= 0.0))
    {
        return FALSE;
    }
    
    RECT rcControl;
    GetClientRect(m_hWnd, &rcControl);  
    MoveWindow(m_pSlider[m_nSliders].hwndBox, 
            BOX_X, 
            m_nNextControlPos, 
            CONTROL_WIDTH - (2 * BOX_X) - SCROLL_WIDTH, 
            SLIDER_BOX_HEIGHT, 
            TRUE
            );
    m_pSlider[m_nSliders].nStartYPos = m_nNextControlPos;
    MoveWindow(m_pSlider[m_nSliders].hwndSlider, 
            BOX_X + SLIDER_X, 
            m_nNextControlPos + SLIDER_Y, 
            CONTROL_WIDTH - (2 * (BOX_X + SLIDER_X)) - SCROLL_WIDTH,
            SLIDER_HEIGHT, 
            TRUE
            );
    MoveWindow(m_pSlider[m_nSliders].hwndLabel, 
            BOX_X + SLIDER_X, 
            m_nNextControlPos + SLIDER_Y + SLIDER_HEIGHT, 
            CONTROL_WIDTH - (2 * (BOX_X + SLIDER_X)) - SCROLL_WIDTH,
            FONT_HEIGHT, 
            TRUE
            );
    m_pSlider[m_nSliders].nID = nID;
    SetSliderParameters(&m_pSlider[m_nSliders], fltMin, fltMax, fltStep, fltStartPos);
    SetWindowText(m_pSlider[m_nSliders].hwndBox, szName);
    m_pSlider[m_nSliders].fChanged = TRUE;
    m_nNextControlPos += SLIDER_BOX_HEIGHT + BOX_Y_SPACE;
    m_nSliders++;
    Update();
    
    return TRUE;
}
    //--------------------------------------------------------------------
S_Slider *C_ControlWnd::FindSlider(HWND hWnd)
{
    for(int i=0; i<m_nSliders; i++)
    {
        if(m_pSlider[i].hwndSlider == hWnd)
        {
            return &(m_pSlider[i]);
        }
    }
    return NULL;
}
    //--------------------------------------------------------------------
S_Slider *C_ControlWnd::FindSlider(int nID)
{
    for(int i=0; i<m_nSliders; i++)
    {
        if(m_pSlider[i].nID == nID)
        {
            return &(m_pSlider[i]);
        }
    }
    return NULL;
}
    //--------------------------------------------------------------------
LRESULT C_ControlWnd::OnSlider(LPARAM lp)
{
    int nPos;

    S_Slider *pSlider = FindSlider(((LPNMHDR)lp)->hwndFrom);
    
    if(pSlider == NULL)
    {
        return NULL;
    }
        
	nPos = (int)SendMessage(pSlider->hwndSlider, TBM_GETPOS, 0, 0);
    float fltCurValue = pSlider->fltMin + pSlider->fltStep * (float)nPos;
    if(fltCurValue != pSlider->fltCurValue)
    {
        pSlider->fltCurValue = fltCurValue;
        char szStr[100];
        sprintf(szStr, "%f", fltCurValue);
        SetWindowText(pSlider->hwndLabel, szStr);
        pSlider->fChanged = TRUE;
    }
	
    return NULL;
}
    //--------------------------------------------------------------------
float C_ControlWnd::GetSlider(int nID)
{
    S_Slider *pSlider = FindSlider(nID);
    
    if(pSlider == NULL)
    {
        return 0.0;
    }
    pSlider->fChanged = FALSE;
    return pSlider->fltCurValue;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::SetSlider(int nID, float fVal)
{
    S_Slider *pSlider = FindSlider(nID);
    
    if(pSlider == NULL)
    {
        return FALSE;
    }

    int nMax = (int)((pSlider->fltMax - pSlider->fltMin) / pSlider->fltStep + 0.5);
    int nStartPos;
    if(fVal < pSlider->fltMin)
    {
        nStartPos = 0;
    }
    else if(fVal > pSlider->fltMax)
    {
        nStartPos = nMax;
    }
    nStartPos = (int)(((float)nMax * (fVal - pSlider->fltMin)) / 
            (pSlider->fltMax - pSlider->fltMin) + 0.5);
    SendMessage(pSlider->hwndSlider, TBM_SETPOS, TRUE, nStartPos);

    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::IsSliderChanged(int nID)
{
    S_Slider *pSlider = FindSlider(nID);
    
    if(pSlider == NULL)
    {
        return FALSE;
    }

    return pSlider->fChanged;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::CreateRadioGroup(PSTR szGroupName, int nGroupID, int nQuantity, 
        PSTR szRadioName, int *pnRadioID, int nChecked)
{
    if((m_nRadioGroups == VS_MAX_RADIO_GROUP) || (nQuantity > VS_MAX_RADIO))
    {
        return FALSE;
    }

    RECT rcControl;
    int nRadioBoxHeight = nQuantity * RADIO_HEIGHT + (int)(RADIO_Y * 1.5);
    int i, j;
    GetClientRect(m_hWnd, &rcControl);
    MoveWindow(m_pRadioGroup[m_nRadioGroups].hwndBox, 
            BOX_X, 
            m_nNextControlPos, 
            CONTROL_WIDTH - (2 * BOX_X) - SCROLL_WIDTH, 
            nRadioBoxHeight, 
            TRUE
            );
    m_pRadioGroup[m_nRadioGroups].nStartYPos = m_nNextControlPos;
    SetWindowText(m_pRadioGroup[m_nRadioGroups].hwndBox, szGroupName);
    int nRNLen = lstrlen(szRadioName);
    int nRNCnt = 0;
    char *szRN = new char[nRNLen + 1];

    for(i=0; i<nQuantity; i++)
    {
        MoveWindow(m_pRadioGroup[m_nRadioGroups].pRadio[i].hWnd, 
            BOX_X + RADIO_X, 
            m_nNextControlPos + RADIO_HEIGHT * i + RADIO_Y, 
            CONTROL_WIDTH - (2 * (BOX_X + RADIO_X)) - SCROLL_WIDTH,
            RADIO_HEIGHT, 
            TRUE
            );
        ZeroMemory(szRN, nRNLen + 1);
        for(j=0; nRNCnt<nRNLen; nRNCnt++, j++)
        {
            if(szRadioName[nRNCnt] == '&')
            {
                nRNCnt++;
                break;
            }
            szRN[j] = szRadioName[nRNCnt];
        }
        SetWindowText(m_pRadioGroup[m_nRadioGroups].pRadio[i].hWnd, szRN);
        SendMessage(m_pRadioGroup[m_nRadioGroups].pRadio[i].hWnd,
                    BM_SETCHECK, (WPARAM)(i == nChecked), (LPARAM)0);
        m_pRadioGroup[m_nRadioGroups].pRadio[i].nID = pnRadioID[i];
    }
    m_pRadioGroup[m_nRadioGroups].nID = nGroupID;
    m_pRadioGroup[m_nRadioGroups].nRadios = nQuantity;
    m_nNextControlPos += nRadioBoxHeight + BOX_Y_SPACE;
    m_nRadioGroups++;
    delete [] szRN;

    Update();

    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::IsRadioChecked(int nGroupID, int nRadioID)
{
    int i, j;

    for(i=0; i<m_nRadioGroups; i++)
    {
        if(m_pRadioGroup[i].nID == nGroupID)
        {
            for(j=0; j<m_pRadioGroup[i].nRadios; j++)
            {
                if(m_pRadioGroup[i].pRadio[j].nID == nRadioID)
                {
                    return (BOOL)SendMessage(m_pRadioGroup[i].pRadio[j].hWnd, 
                            BM_GETCHECK, 0, 0);
                }
            }
        }
    }
    return FALSE;
}
    //--------------------------------------------------------------------
int C_ControlWnd::GetCheckedID(int nGroupID)
{
    int i, j;

    for(i=0; i<m_nRadioGroups; i++)
    {
        if(m_pRadioGroup[i].nID == nGroupID)
        {
            for(j=0; j<m_pRadioGroup[i].nRadios; j++)
            {
                if(SendMessage(m_pRadioGroup[i].pRadio[j].hWnd, BM_GETCHECK, 0, 0))
                {
                    return m_pRadioGroup[i].pRadio[j].nID;
                }
            }
        }
    }
    return 0;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::CheckRadio(int nGroupID, int nRadioID, BOOL fCheck)
{
    int i, j;
    
    for(i=0; i<m_nRadioGroups; i++)
    {
        if(m_pRadioGroup[i].nID == nGroupID)
        {
            for(j=0; j<m_pRadioGroup[i].nRadios; j++)
            {
                if(m_pRadioGroup[i].pRadio[j].nID == nRadioID)
                {
                    break;
                }
            }
            if(j < m_pRadioGroup[i].nRadios)
            {
                if(!fCheck)
                {
                    SendMessage(m_pRadioGroup[i].pRadio[j].hWnd, 
                            BM_SETCHECK, (WPARAM)FALSE, 0);
                }
                else
                {
                    for(j=0; j<m_pRadioGroup[i].nRadios; j++)
                    {
                        if(m_pRadioGroup[i].pRadio[j].nID == nRadioID)
                        {

                            SendMessage(m_pRadioGroup[i].pRadio[j].hWnd, 
                                    BM_SETCHECK, (WPARAM)TRUE, 0);
                        }
                        else
                        {
                            SendMessage(m_pRadioGroup[i].pRadio[j].hWnd, 
                                    BM_SETCHECK, (WPARAM)FALSE, 0);
                        }
                    }
                }
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    return FALSE;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::CreateCheckBox(PSTR szName, int nID, BOOL fCheck)
{
    if(m_nCheckBoxes == VS_MAX_CHECKBOX)
    {
        return FALSE;
    }

    RECT rcControl;
    GetClientRect(m_hWnd, &rcControl);  
    MoveWindow(m_pCheckBox[m_nCheckBoxes].hwndBox, 
            BOX_X, 
            m_nNextControlPos, 
            CONTROL_WIDTH - (2 * BOX_X) - SCROLL_WIDTH, 
            CHECKBOX_BOX_HEIGHT, 
            TRUE
            );
    m_pCheckBox[m_nCheckBoxes].nStartYPos = m_nNextControlPos;
    MoveWindow(m_pCheckBox[m_nCheckBoxes].hwndCheckBox, 
            BOX_X + CHECKBOX_X, 
            m_nNextControlPos + CHECKBOX_Y, 
            CONTROL_WIDTH - (2 * (BOX_X + CHECKBOX_X)) - SCROLL_WIDTH,
            CHECKBOX_HEIGHT, 
            TRUE
            );
    m_nNextControlPos += CHECKBOX_BOX_HEIGHT + BOX_Y_SPACE;
    m_pCheckBox[m_nCheckBoxes].nID = nID;
    SetWindowText(m_pCheckBox[m_nCheckBoxes].hwndCheckBox, szName);
    SendMessage(m_pCheckBox[m_nCheckBoxes].hwndCheckBox, BM_SETCHECK, (WPARAM)fCheck, 0);
    m_nCheckBoxes++;

    Update();
    
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::IsCheckBoxChecked(int nID)
{
    int i;

    for(i=0; i<m_nCheckBoxes; i++)
    {
        if(m_pCheckBox[i].nID == nID)
        {
            return (BOOL)SendMessage(m_pCheckBox[i].hwndCheckBox, 
                BM_GETCHECK, 0, 0);
        }
    }
    return FALSE;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::CheckBox(int nID, BOOL fCheck)
{
    int i;

    for(i=0; i<m_nCheckBoxes; i++)
    {
        if(m_pCheckBox[i].nID == nID)
        {
            SendMessage(m_pCheckBox[i].hwndCheckBox, 
                BM_GETCHECK, (WPARAM)fCheck, 0);
            return TRUE;
        }
    }
    return FALSE;
}

    //--------------------------------------------------------------------
BOOL C_ControlWnd::SetCheckBox(int nID, BOOL fCheck)
{
    int i;

    for(i=0; i<m_nCheckBoxes; i++)
    {
        if(m_pCheckBox[i].nID == nID)
        {
            SendMessage(m_pCheckBox[i].hwndCheckBox, 
                BM_SETCHECK, (WPARAM)fCheck, 0);
            return TRUE;
        }
    }
    return FALSE;
}

    //--------------------------------------------------------------------
BOOL C_ControlWnd::CreateEdit(PSTR szName, int nID)
{
    if(m_nEdits == VS_MAX_EDIT)
    {
        return FALSE;
    }

    RECT rcControl;
    GetClientRect(m_hWnd, &rcControl);  
    MoveWindow(m_pEdit[m_nEdits].hwndBox, 
            BOX_X, 
            m_nNextControlPos, 
            CONTROL_WIDTH - (2 * BOX_X) - SCROLL_WIDTH, 
            EDIT_BOX_HEIGHT, 
            TRUE
            );
    m_pEdit[m_nEdits].nStartYPos = m_nNextControlPos;
    MoveWindow(m_pEdit[m_nEdits].hwndEdit, 
            BOX_X + EDIT_X, 
            m_nNextControlPos + EDIT_Y, 
            CONTROL_WIDTH - (2 * (BOX_X + EDIT_X)) - SCROLL_WIDTH,
            EDIT_HEIGHT, 
            TRUE
            );
    MoveWindow(m_pEdit[m_nEdits].hwndLabel, 
            BOX_X + EDIT_X, 
            m_nNextControlPos + EDIT_Y + EDIT_HEIGHT, 
            CONTROL_WIDTH - (2 * (BOX_X + EDIT_X)) - SCROLL_WIDTH,
            FONT_HEIGHT, 
            TRUE
            );
    m_nNextControlPos += EDIT_BOX_HEIGHT + BOX_Y_SPACE;
    m_pEdit[m_nEdits].nID = nID;
    SetWindowText(m_pEdit[m_nEdits].hwndBox, szName);
    SetWindowText(m_pEdit[m_nEdits].hwndEdit, "");
    SetWindowText(m_pEdit[m_nEdits].hwndLabel, "");
    m_nEdits++;

    Update();
    
    return TRUE;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::GetEditText(int nID, PSTR szText)
{
    int i;

    if(!szText)
    {
        return FALSE;
    }

    for(i=0; i<m_nEdits; i++)
    {
        if(m_pEdit[i].nID == nID)
        {
            lstrcpy(szText, m_pEdit[i].szText);
            return TRUE;
        }
    }
    return FALSE;
}
    //--------------------------------------------------------------------
int C_ControlWnd::GetEditInt(int nID, BOOL *pfError)
{
    int i, nRes = 0;
    if(pfError)
    {
        *pfError = TRUE;
    }

    for(i=0; i<m_nEdits; i++)
    {
        if(m_pEdit[i].nID == nID)
        {
            char *szEndPtr = NULL;
            nRes = strtol(m_pEdit[i].szText, &szEndPtr, 10);
            if(szEndPtr && lstrlen(szEndPtr))
            {
                nRes = 0;
            }
            else if(pfError)
            {
                *pfError = FALSE;
            }
            break;
        }
    }
    return nRes;
}
    //--------------------------------------------------------------------
float C_ControlWnd::GetEditFloat(int nID, BOOL *pfError)
{
    int i;
    float fltRes = 0.0f;
    if(pfError)
    {
        *pfError = TRUE;
    }

    for(i=0; i<m_nEdits; i++)
    {
        if(m_pEdit[i].nID == nID)
        {
            char *szEndPtr = NULL;
            fltRes = (float)strtod(m_pEdit[i].szText, &szEndPtr);
            if(szEndPtr && lstrlen(szEndPtr))
            {
                fltRes = 0.0f;
            }
            else if(pfError)
            {
                *pfError = FALSE;
            }
            break;
        }
    }
    return fltRes;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::SetEditText(int nID, PSTR szText)
{
    int i;

    if(!szText)
    {
        return FALSE;
    }

    for(i=0; i<m_nEdits; i++)
    {
        if(m_pEdit[i].nID == nID)
        {
            lstrcpyn(m_pEdit[i].szText, szText, 64);
            SetWindowText(m_pEdit[i].hwndEdit, m_pEdit[i].szText);
            SetWindowText(m_pEdit[i].hwndLabel, m_pEdit[i].szText);
            return TRUE;
        }
    }
    return FALSE;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::SetEditInt(int nID, int nVal)
{
    int i;

    for(i=0; i<m_nEdits; i++)
    {
        if(m_pEdit[i].nID == nID)
        {
            sprintf(m_pEdit[i].szText, "%d", nVal);
            SetWindowText(m_pEdit[i].hwndEdit, m_pEdit[i].szText);
            SetWindowText(m_pEdit[i].hwndLabel, m_pEdit[i].szText);
            return TRUE;
        }
    }
    return FALSE;
}
    //--------------------------------------------------------------------
BOOL C_ControlWnd::SetEditFloat(int nID, float fltVal)
{
    int i;

    for(i=0; i<m_nEdits; i++)
    {
        if(m_pEdit[i].nID == nID)
        {
            sprintf(m_pEdit[i].szText, "%f", fltVal);
            SetWindowText(m_pEdit[i].hwndEdit, m_pEdit[i].szText);
            SetWindowText(m_pEdit[i].hwndLabel, m_pEdit[i].szText);
            return TRUE;
        }
    }
    return FALSE;
}
    //--------------------------------------------------------------------
void C_ControlWnd::OnEditReturn(HWND hwndEdit)
{
    int i;
    
    for(i=0; i<m_nEdits; i++)
    {
        if(m_pEdit[i].hwndEdit == hwndEdit)
        {
            GetWindowText(hwndEdit, m_pEdit[i].szText, 64);
            SetWindowText(m_pEdit[i].hwndLabel, m_pEdit[i].szText);
            return;
        }
    }
}
    //--------------------------------------------------------------------
void C_ControlWnd::VMoveWindow(HWND hWnd, int nPos)
{
    RECT rc;
    GetWindowRect(hWnd, &rc);
    ScreenToClient(m_hWnd, (LPPOINT)&rc);
    ScreenToClient(m_hWnd, (LPPOINT)((LONG*)&rc + 2));
    SetWindowPos(hWnd, NULL, rc.left, nPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
    //--------------------------------------------------------------------
void C_ControlWnd::ArrangeControls()
{
    int i, j;
    int nPos;
    int nDif = m_nCurScrollPos * SCROLL_STEP;
    for(i=0; i<m_nSliders; i++)
    {
        nPos = m_pSlider[i].nStartYPos - nDif;
        VMoveWindow(m_pSlider[i].hwndBox, nPos);
        VMoveWindow(m_pSlider[i].hwndSlider, nPos + SLIDER_Y);
        VMoveWindow(m_pSlider[i].hwndLabel, nPos + SLIDER_Y + SLIDER_HEIGHT);
    }
    for(i=0; i<m_nRadioGroups; i++)
    {
        nPos = m_pRadioGroup[i].nStartYPos - nDif;
        VMoveWindow(m_pRadioGroup[i].hwndBox, nPos);
        for(j=0; j<m_pRadioGroup[i].nRadios; j++)
        {
            VMoveWindow(m_pRadioGroup[i].pRadio[j].hWnd, nPos + RADIO_HEIGHT * j + RADIO_Y);
        }
    }
    for(i=0; i<m_nCheckBoxes; i++)
    {
        nPos = m_pCheckBox[i].nStartYPos - nDif;
        VMoveWindow(m_pCheckBox[i].hwndBox, nPos);
        VMoveWindow(m_pCheckBox[i].hwndCheckBox, nPos + CHECKBOX_Y);
    }
    for(i=0; i<m_nEdits; i++)
    {
        nPos = m_pEdit[i].nStartYPos - nDif;
        VMoveWindow(m_pEdit[i].hwndBox, nPos);
        VMoveWindow(m_pEdit[i].hwndEdit, nPos + EDIT_Y);
    }
}
    //--------------------------------------------------------------------
void C_ControlWnd::Update()
{    
    RECT rc;
    GetClientRect(m_hWnd, &rc);
    OnSize(m_hWnd, (WPARAM)0, (LPARAM)(rc.right | (rc.bottom << 16)));
}    
    //--------------------------------------------------------------------
void C_ControlWnd::SaveSetting()
{
    OPENFILENAME ofn;
    char szFileName[_MAX_PATH];
    
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ZeroMemory(szFileName, sizeof(szFileName));
    ofn.lStructSize         = sizeof( OPENFILENAME );
    ofn.hwndOwner           = NULL;
    ofn.hInstance           = m_hInstance;
    ofn.lpstrFilter         = "Config Files (*.cfg)\0*.cfg\0\0";
    ofn.lpstrFile           = szFileName;
    ofn.nMaxFile            = _MAX_PATH;
    ofn.nMaxFileTitle       = _MAX_FNAME + _MAX_EXT;
    ofn.lpstrTitle          = "Save control settings";

    if(GetSaveFileName(&ofn))
    {
        char szExt[10];
        int nLen;

        nLen = lstrlen(szFileName);
        if(nLen > 4)
        {
            lstrcpy(szExt, &szFileName[lstrlen(szFileName) - 3]);
            if(lstrcmp(szExt, "cfg") && lstrcmp(szExt, "CFG"))
            {
                lstrcat(szFileName, ".cfg");
            }
        }
        else
        {
            lstrcat(szFileName, ".cfg");
        }

        FILE *fp;
        if((fp = fopen(szFileName, "wb")) == NULL)
        {
            MessageBox(NULL, "Cannon create a file", "", MB_OK);
            return;
        }
        
        int i, j;
        for(i=0; i<m_nRadioGroups; i++)
        {
            for(j=0; j<m_pRadioGroup[i].nRadios; j++)
            {
                m_pRadioGroup[i].pRadio[j].fChecked = 
                        (BOOL)SendMessage(m_pRadioGroup[i].pRadio[j].hWnd, 
                        BM_GETCHECK, 0, 0);
            }
        }
        for(i=0; i<m_nCheckBoxes; i++)
        {
            m_pCheckBox[i].fChecked = (BOOL)SendMessage(m_pCheckBox[i].hwndCheckBox, 
                    BM_GETCHECK, 0, 0);
        }
        for(i=0; i<m_nEdits; i++)
        {
            GetWindowText(m_pEdit[i].hwndEdit, m_pEdit[i].szText, 63);
        }
        fwrite(m_pSlider, sizeof(S_Slider) * VS_MAX_SLIDER, 1, fp);
        fwrite(m_pRadioGroup, sizeof(S_RadioGroup) * VS_MAX_RADIO_GROUP, 1, fp);
        fwrite(m_pCheckBox, sizeof(S_CheckBox) * VS_MAX_CHECKBOX, 1, fp);
        fwrite(m_pEdit, sizeof(S_Edit) * VS_MAX_EDIT, 1, fp);
        fclose(fp);
    }
}
    //--------------------------------------------------------------------
void C_ControlWnd::LoadSetting()
{
    OPENFILENAME ofn;
    char szFileName[_MAX_PATH];
    
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ZeroMemory(szFileName, sizeof(szFileName));
    ofn.lStructSize         = sizeof(OPENFILENAME);
    ofn.hwndOwner           = NULL;
    ofn.lpstrFilter         = "Config Files (*.cfg)\0*.cfg\0\0";
    ofn.lpstrFile           = szFileName;
    ofn.nMaxFile            = _MAX_PATH;
    ofn.nMaxFileTitle       = _MAX_FNAME + _MAX_EXT;
    ofn.Flags               = OFN_FILEMUSTEXIST;
    ofn.lpstrTitle          = "Load control settings";

    if(GetOpenFileName(&ofn))
    {
        FILE *fp;
        if((fp = fopen(szFileName, "rb")) == NULL)
        {
            MessageBox(NULL, "Cannon open a file", "", MB_OK);
            return;
        }

        S_Slider *pSlider = new S_Slider[VS_MAX_SLIDER];
        S_RadioGroup *pRadioGroup = new S_RadioGroup[VS_MAX_RADIO_GROUP];
        S_CheckBox *pCheckBox = new S_CheckBox[VS_MAX_CHECKBOX];
        S_Edit *pEdit = new S_Edit[VS_MAX_EDIT];

        fread(pSlider, sizeof(S_Slider), VS_MAX_SLIDER, fp);
        fread(pRadioGroup, sizeof(S_RadioGroup), VS_MAX_RADIO_GROUP, fp);
        fread(pCheckBox, sizeof(S_CheckBox), VS_MAX_CHECKBOX, fp);
        fread(pEdit, sizeof(S_Edit), VS_MAX_EDIT, fp);
        fclose(fp);

        int i, j, k;

        for(i=0; i<m_nSliders; i++)
        {
            for(j=0; j<VS_MAX_SLIDER; j++)
            {
                if(m_pSlider[i].nID == pSlider[j].nID)
                {
                    m_pSlider[i].fChanged = TRUE;
                    SetSliderParameters(&m_pSlider[i], pSlider[j].fltMin, 
                            pSlider[j].fltMax, pSlider[j].fltStep, pSlider[j].fltCurValue);
                    break;
                }
            }
        }
        
        for(i=0; i<m_nRadioGroups; i++)
        {
            for(j=0; j<VS_MAX_RADIO_GROUP; j++)
            {
                if(m_pRadioGroup[i].nID == pRadioGroup[j].nID)
                {
                    for(k=0; k<m_pRadioGroup[i].nRadios; k++)
                    {
                        m_pRadioGroup[i].pRadio[k].fChecked = pRadioGroup[j].pRadio[k].fChecked;
                        SendMessage(m_pRadioGroup[i].pRadio[k].hWnd, 
                                BM_SETCHECK, (WPARAM)m_pRadioGroup[i].pRadio[k].fChecked, 0);
                    }
                }
            }
        }

        for(i=0; i<m_nCheckBoxes; i++)
        {
            for(j=0; j<VS_MAX_CHECKBOX; j++)
            {
                if(m_pCheckBox[i].nID == pCheckBox[j].nID)
                {
                    m_pCheckBox[i].fChecked = pCheckBox[j].fChecked;
                    SendMessage(m_pCheckBox[i].hwndCheckBox, 
                                BM_SETCHECK, (WPARAM)m_pCheckBox[i].fChecked, 0);
                    break;
                }
            }
        }

        for(i=0; i<m_nEdits; i++)
        {
            for(j=0; j<VS_MAX_EDIT; j++)
            {
                if(m_pEdit[i].nID == pEdit[j].nID)
                {
                    lstrcpy(m_pEdit[i].szText, m_pEdit[j].szText);
                    SetWindowText(m_pEdit[i].hwndEdit, m_pEdit[i].szText);
                    break;
                }
            }
        }

        delete [] pSlider;
        delete [] pRadioGroup;
        delete [] pCheckBox;
        delete [] pEdit;
    }
}
    //--------------------------------------------------------------------
void C_ControlWnd::Move(int nX, int nY, int nWidth, int nHeight)
{
    MoveWindow(m_hWnd, nX, nY, nWidth, nHeight, TRUE);
}

    //--------------------------------------------------------------------
void C_ControlWnd::Show(BOOL fShow)
{
    ShowWindow(m_hWnd, fShow ? SW_SHOWNORMAL : SW_HIDE);
}

    //--------------------------------------------------------------------
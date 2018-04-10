//------------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "VShell.h"
#include "FrameWnd.h"
#include "AdjustSetupDlg.h"
#include "AdjustSetupParamDlg.h"
#include "resource.h"
//------------------------------------------------------------------------
C_AdjustSetupDlg AdjustSetupDlg;
extern C_AdjustSetupParamDlg AdjustSetupParamDlg;

static const COLORREF CLR_HISTO_BACK = RGB(254, 254, 226);
static const COLORREF CLR_HISTO = RGB(128, 128, 255);
static const int HISTO_WIDTH = 256;
static const int HISTO_HEIGHT = 128;
//------------------------------------------------------------------------
LRESULT CALLBACK FrameHistoProc(HWND hwnd, UINT msg, WPARAM wParam,
						 LPARAM lParam)
{
	PAINTSTRUCT Paint;

	switch(msg)
    {
    case WM_MOUSEMOVE:
        return AdjustSetupDlg.OnMouseMoveInHisto(hwnd, LOWORD(lParam));
	case WM_PAINT:
		BeginPaint(hwnd, &Paint);
		EndPaint(hwnd, &Paint);
        return AdjustSetupDlg.DrawHisto();
	}
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
//------------------------------------------------------------------------
INT_PTR CALLBACK AdjustSetupDlgProc(HWND hDlg, UINT msg, 
	    WPARAM wParam, 	LPARAM lParam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        return AdjustSetupDlg.OnInitDialog(hDlg);
    case WM_CLOSE:
        return AdjustSetupDlg.OnCancel(hDlg);
    case WM_NOTIFY:
        return AdjustSetupDlg.OnNotify(hDlg, lParam);
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case BTN_OK:
            return AdjustSetupDlg.OnOk(hDlg);
        case BTN_CANCEL:
            return AdjustSetupDlg.OnCancel(hDlg);
        case CH_ENABLE_ADJUST:
            return AdjustSetupDlg.OnChAdjust(hDlg);
        case BTN_MULTIPLIER_SETUP:
            return AdjustSetupDlg.OnSetupParamMult(hDlg);
        case BTN_CONTRAST_RESET:
        case BTN_BRIGHTNESS_RESET:
            return AdjustSetupDlg.OnReset(hDlg, LOWORD(wParam));
        }
    }
    return FALSE;
}
//------------------------------------------------------------------------
int C_AdjustSetupDlg::Show(HINSTANCE hInstance, HWND hwndParent)
{
    return (int)DialogBox(m_hInstance = hInstance, 
            MAKEINTRESOURCE(ADJUST_SETUP_DIALOG), m_hwndParent = hwndParent, AdjustSetupDlgProc);
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupDlg::OnInitDialog(HWND hDlg)
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLong(m_hwndParent, 0);
    if(!pChData || !pChData->pBitmap)
    {
        OnCancel(hDlg);
        return TRUE;
    }
    m_nType = pChData->pBitmap->Type();

    S_Adjust Adjust;
    pChData->pBitmap->GetAdjust(&Adjust);

    SendMessage(GetDlgItem(hDlg, SL_CONTRAST), TBM_SETRANGEMIN, FALSE, -128);
    SendMessage(GetDlgItem(hDlg, SL_CONTRAST), TBM_SETRANGEMAX, FALSE, 127);
    SendMessage(GetDlgItem(hDlg, SL_CONTRAST), TBM_CLEARTICS, TRUE, 0);
    SendMessage(GetDlgItem(hDlg, SL_CONTRAST), TBM_SETPOS, TRUE, Adjust.nContrast);

    SendMessage(GetDlgItem(hDlg, SL_BRIGHTNESS), TBM_SETRANGEMIN, FALSE, -128);
    SendMessage(GetDlgItem(hDlg, SL_BRIGHTNESS), TBM_SETRANGEMAX, FALSE, 127);
    SendMessage(GetDlgItem(hDlg, SL_BRIGHTNESS), TBM_CLEARTICS, TRUE, 0);
    SendMessage(GetDlgItem(hDlg, SL_BRIGHTNESS), TBM_SETPOS, TRUE, Adjust.nBrightness);

    
    BOOL fAdjust = Adjust.fAdjust;
    float fltMax = Adjust.fltMaxMult;
    float fltMin = Adjust.fltMinMult;
    float fltStep = Adjust.fltStepMult;
    float fltStartPos = Adjust.fltMult;
    int nMax;
    if(fltStep != 0.0f)
    {
        nMax = (int)((fltMax - fltMin) / fltStep + 0.5);
    }
    else
    {
        nMax = 0;
    }
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

    SendMessage(GetDlgItem(hDlg, SL_MULTIPLIER), TBM_SETRANGEMIN, FALSE, 0);
    SendMessage(GetDlgItem(hDlg, SL_MULTIPLIER), TBM_SETRANGEMAX, FALSE, nMax);
    SendMessage(GetDlgItem(hDlg, SL_MULTIPLIER), TBM_CLEARTICS, TRUE, 0);
    SendMessage(GetDlgItem(hDlg, SL_MULTIPLIER), TBM_SETPOS, TRUE, nStartPos);

    CheckDlgButton(hDlg, CH_ENABLE_ADJUST, fAdjust);

    EnableWindow(GetDlgItem(hDlg, SL_BRIGHTNESS), fAdjust);
    EnableWindow(GetDlgItem(hDlg, BTN_BRIGHTNESS_RESET), fAdjust);
    EnableWindow(GetDlgItem(hDlg, BOX_BRIGHTNESS), fAdjust);
    EnableWindow(GetDlgItem(hDlg, LAB_BRIGHTNESS), fAdjust);
    if(m_nType == VS_RGB8_8 || m_nType == VS_RGB8_16 || m_nType == VS_RGB8_32)
    {
        EnableWindow(GetDlgItem(hDlg, SL_CONTRAST), FALSE);
        EnableWindow(GetDlgItem(hDlg, BTN_CONTRAST_RESET), FALSE);
        EnableWindow(GetDlgItem(hDlg, BOX_CONTRAST), FALSE);
        EnableWindow(GetDlgItem(hDlg, LAB_CONTRAST), FALSE);
        EnableWindow(GetDlgItem(hDlg, SL_MULTIPLIER), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BTN_MULTIPLIER_SETUP), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BOX_MULTIPLIER), fAdjust);
        EnableWindow(GetDlgItem(hDlg, LAB_MULTIPLIER), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BTN_SET_BY_IMAGE), fAdjust);
    }
    else
    {
        EnableWindow(GetDlgItem(hDlg, SL_CONTRAST), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BTN_CONTRAST_RESET), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BOX_CONTRAST), fAdjust);
        EnableWindow(GetDlgItem(hDlg, LAB_CONTRAST), fAdjust);
        EnableWindow(GetDlgItem(hDlg, SL_MULTIPLIER), FALSE);
        EnableWindow(GetDlgItem(hDlg, BTN_MULTIPLIER_SETUP), FALSE);
        EnableWindow(GetDlgItem(hDlg, BOX_MULTIPLIER), FALSE);
        EnableWindow(GetDlgItem(hDlg, LAB_MULTIPLIER), FALSE);
        EnableWindow(GetDlgItem(hDlg, BTN_SET_BY_IMAGE), FALSE);
    }

    char szWinName[256];
    char szDlgName[256];

    GetWindowText(m_hwndParent, szWinName, 255);
    sprintf(szDlgName, "Adjust [%s]", szWinName);
    SetWindowText(hDlg, szDlgName);

    RECT rcDlg, rcHisto;

    m_hwndHisto = GetDlgItem(hDlg, FRAME_HISTO); 
    SetWindowLongPtr(m_hwndHisto, GWLP_WNDPROC, (LONG)FrameHistoProc);

    GetClientRect(hDlg, &rcDlg);
    GetWindowRect(m_hwndHisto, &rcHisto);
    ScreenToClient(hDlg, (POINT*)&rcHisto);
    rcHisto.left = (rcDlg.right - HISTO_WIDTH) / 2;
    MoveWindow(m_hwndHisto, rcHisto.left, rcHisto.top, HISTO_WIDTH, HISTO_HEIGHT, TRUE);

    pChData->pBitmap->GetHistogram(m_pnHistogram, HISTO_HEIGHT);
    
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupDlg::OnOk(HWND hDlg)
{
	EndDialog(hDlg, 1);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupDlg::OnCancel(HWND hDlg)
{
	EndDialog(hDlg, 0);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupDlg::OnChAdjust(HWND hDlg)
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLong(m_hwndParent, 0);
    BOOL fAdjust = IsDlgButtonChecked(hDlg, CH_ENABLE_ADJUST);

    if(pChData && pChData->pBitmap)
    {
        S_Adjust Adjust;
        pChData->pBitmap->GetAdjust(&Adjust);
        Adjust.fAdjust = fAdjust;
        pChData->pBitmap->SetAdjust(&Adjust);
    }

    EnableWindow(GetDlgItem(hDlg, SL_BRIGHTNESS), fAdjust);
    EnableWindow(GetDlgItem(hDlg, BTN_BRIGHTNESS_RESET), fAdjust);
    EnableWindow(GetDlgItem(hDlg, BOX_BRIGHTNESS), fAdjust);
    EnableWindow(GetDlgItem(hDlg, LAB_BRIGHTNESS), fAdjust);

    if(m_nType == VS_RGB8_8 || m_nType == VS_RGB8_16 || m_nType == VS_RGB8_32)
    {
        EnableWindow(GetDlgItem(hDlg, SL_CONTRAST), FALSE);
        EnableWindow(GetDlgItem(hDlg, BTN_CONTRAST_RESET), FALSE);
        EnableWindow(GetDlgItem(hDlg, BOX_CONTRAST), FALSE);
        EnableWindow(GetDlgItem(hDlg, LAB_CONTRAST), FALSE);
        EnableWindow(GetDlgItem(hDlg, SL_MULTIPLIER), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BTN_MULTIPLIER_SETUP), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BOX_MULTIPLIER), fAdjust);
        EnableWindow(GetDlgItem(hDlg, LAB_MULTIPLIER), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BTN_SET_BY_IMAGE), fAdjust);
    }
    else
    {
        EnableWindow(GetDlgItem(hDlg, SL_CONTRAST), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BTN_CONTRAST_RESET), fAdjust);
        EnableWindow(GetDlgItem(hDlg, BOX_CONTRAST), fAdjust);
        EnableWindow(GetDlgItem(hDlg, LAB_CONTRAST), fAdjust);
        EnableWindow(GetDlgItem(hDlg, SL_MULTIPLIER), FALSE);
        EnableWindow(GetDlgItem(hDlg, BTN_MULTIPLIER_SETUP), FALSE);
        EnableWindow(GetDlgItem(hDlg, BOX_MULTIPLIER), FALSE);
        EnableWindow(GetDlgItem(hDlg, LAB_MULTIPLIER), FALSE);
        EnableWindow(GetDlgItem(hDlg, BTN_SET_BY_IMAGE), FALSE);
    }

    return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupDlg::OnNotify(HWND hDlg, LPARAM lp)
{
    if(((LPNMHDR)lp)->code != NM_CUSTOMDRAW)
    {
        return TRUE;
    }
    
    S_ChildData *pChData = (S_ChildData*)GetWindowLong(m_hwndParent, 0);
    if(!pChData || !pChData->pBitmap)
    {
        return TRUE;
    }

    int nID = (int)((LPNMHDR)lp)->idFrom;
    HWND hwndSlider = ((LPNMHDR)lp)->hwndFrom;
    int nPos = (int)SendMessage(hwndSlider, TBM_GETPOS, 0, 0);
    S_Adjust Adjust;

    pChData->pBitmap->GetAdjust(&Adjust);
    
    if(nID == SL_CONTRAST)
    {
        SetDlgItemInt(hDlg, LAB_CONTRAST, Adjust.nContrast = nPos, TRUE);
    }
    else if(nID == SL_BRIGHTNESS)
    {
        SetDlgItemInt(hDlg, LAB_BRIGHTNESS, Adjust.nBrightness = nPos, TRUE);
    }
    else if(nID == SL_MULTIPLIER)
    {
        char szStr[40];

        Adjust.fltMult = Adjust.fltMinMult +  Adjust.fltStepMult * (float)nPos;
        sprintf(szStr, "%f", Adjust.fltMult);
        SetDlgItemText(hDlg, LAB_MULTIPLIER, szStr);
    }
    pChData->pBitmap->SetAdjust(&Adjust);
    pChData->pBitmap->GetHistogram(m_pnHistogram, HISTO_HEIGHT);
    DrawHisto();

    return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupDlg::OnSetupParamMult(HWND hDlg)
{    
    if(AdjustSetupParamDlg.Show(m_hInstance, m_hwndParent))
    {
        S_ChildData *pChData = (S_ChildData*)GetWindowLong(m_hwndParent, 0);
        
        if(!pChData || !pChData->pBitmap)
        {
            return TRUE;
        }

        S_Adjust Adjust;
        pChData->pBitmap->GetAdjust(&Adjust);
        float fltStartPos = Adjust.fltMult;
        float fltMax = Adjust.fltMaxMult;
        float fltMin = Adjust.fltMinMult;
        float fltStep = Adjust.fltStepMult;
        int nMax;
        if(fltStep != 0.0f)
        {
            nMax = (int)((fltMax - fltMin) / fltStep + 0.5);
        }
        else
        {
            nMax = 0;
        }
        int nStartPos = (int)(((float)nMax * (fltStartPos - fltMin)) / (fltMax - fltMin) + 0.5);

        SendMessage(GetDlgItem(hDlg, SL_MULTIPLIER), TBM_SETRANGEMIN, FALSE, 0);
        SendMessage(GetDlgItem(hDlg, SL_MULTIPLIER), TBM_SETRANGEMAX, FALSE, nMax);
        SendMessage(GetDlgItem(hDlg, SL_MULTIPLIER), TBM_CLEARTICS, TRUE, 0);
        SendMessage(GetDlgItem(hDlg, SL_MULTIPLIER), TBM_SETPOS, TRUE, nStartPos);
        SendMessage(GetDlgItem(hDlg, SL_BRIGHTNESS), TBM_SETPOS, TRUE, Adjust.nBrightness);
        SendMessage(GetDlgItem(hDlg, SL_CONTRAST), TBM_SETPOS, TRUE, Adjust.nContrast);

        pChData->pBitmap->GetHistogram(m_pnHistogram, HISTO_HEIGHT);
        DrawHisto();
    }
    return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupDlg::OnReset(HWND hDlg, int nID)
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLong(m_hwndParent, 0);
    
    if(!pChData || !pChData->pBitmap)
    {
        return TRUE;
    }

    S_Adjust Adjust;

    pChData->pBitmap->GetAdjust(&Adjust);

    if(nID == BTN_CONTRAST_RESET)
    {
        Adjust.nContrast = 0;
        SendMessage(GetDlgItem(hDlg, SL_CONTRAST), TBM_CLEARTICS, TRUE, 0);
        SendMessage(GetDlgItem(hDlg, SL_CONTRAST), TBM_SETPOS, TRUE, 0);
    }
    else if(nID == BTN_BRIGHTNESS_RESET)
    {
        Adjust.nBrightness = 0;
        SendMessage(GetDlgItem(hDlg, SL_BRIGHTNESS), TBM_CLEARTICS, TRUE, 0);
        SendMessage(GetDlgItem(hDlg, SL_BRIGHTNESS), TBM_SETPOS, TRUE, 0);
    }
    pChData->pBitmap->SetAdjust(&Adjust);
    pChData->pBitmap->GetHistogram(m_pnHistogram, HISTO_HEIGHT);
    DrawHisto();

    return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupDlg::DrawHisto()
{
    HDC hDC;
    HDC hMemDC;
    HBITMAP hBmp;
    HBRUSH hbrBack, hbrHisto;
    HPEN hpenBack, hpenHisto;
    int i;

    
    if((hDC = GetDC(m_hwndHisto)) == NULL)
	    return NULL;

    hbrBack = CreateSolidBrush(CLR_HISTO_BACK);
    hpenBack = CreatePen(PS_SOLID, 1, CLR_HISTO_BACK);
    hbrHisto = CreateSolidBrush(CLR_HISTO);
    hpenHisto = CreatePen(PS_SOLID, 1, CLR_HISTO);
    hMemDC = CreateCompatibleDC(hDC);
    hBmp = (HBITMAP)CreateCompatibleBitmap(hDC, HISTO_WIDTH, HISTO_HEIGHT);
    SelectObject(hMemDC, hBmp);
    SelectObject(hMemDC, hbrBack);
    SelectObject(hMemDC, hpenBack);
    Rectangle(hMemDC, 0, 0, HISTO_WIDTH, HISTO_HEIGHT - 1);
    SelectObject(hMemDC, hbrHisto);
    SelectObject(hMemDC, hpenHisto);
    for(i=0; i<HISTO_WIDTH; i++)
    {
        MoveToEx(hMemDC, i, HISTO_HEIGHT - 1, NULL);
        LineTo(hMemDC, i, HISTO_HEIGHT - 1 - m_pnHistogram[i]);
    }
    
    BitBlt(hDC, 0, 0, HISTO_WIDTH, HISTO_HEIGHT, hMemDC, 0, 0, SRCCOPY);
    DeleteDC(hMemDC);
    DeleteObject(hBmp);
    DeleteObject(hbrBack);
    DeleteObject(hbrHisto);
    DeleteObject(hpenBack);
    DeleteObject(hpenHisto);
    ReleaseDC(m_hwndHisto, hDC);

    return NULL;    
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupDlg::OnMouseMoveInHisto(HWND hDlg, int nX)
{
    char szStr[32];
    int i, nSum;
    float fltPer;

    nSum = 0;
    for(i=0; i<256; i++)
    {
        nSum += m_pnHistogram[i];
    }
    fltPer = ((float)m_pnHistogram[nX] * 100.0f) / (float)nSum;

    sprintf(szStr, "%d (%.2f%%)", nX, fltPer);
    SetDlgItemText(GetParent(hDlg), ST_HISTO_VALUE, szStr);

    return TRUE;
}
//------------------------------------------------------------------------


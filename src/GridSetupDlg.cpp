//------------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include "GridSetupDlg.h"
#include "FrameWnd.h"
#include "resource.h"
//------------------------------------------------------------------------
C_GridSetupDlg GridSetupDlg;
extern C_FrameWnd FrameWnd;
//------------------------------------------------------------------------
INT_PTR CALLBACK GridSetupDlgProc(HWND hDlg, UINT msg, 
	    WPARAM wParam, 	LPARAM)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        return GridSetupDlg.OnInitDialog(hDlg);
    case WM_CLOSE:
        return GridSetupDlg.OnCancel(hDlg);
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case BTN_OK:
            return GridSetupDlg.OnOk(hDlg);
        case BTN_APPLY:
            return GridSetupDlg.OnApply(hDlg);
        case BTN_CANCEL:
            return GridSetupDlg.OnCancel(hDlg);
        case BTN_COLOR:
            return GridSetupDlg.OnColor(hDlg);
        }
    }
    return FALSE;
}
//------------------------------------------------------------------------
int C_GridSetupDlg::Show(HINSTANCE hInstance, HWND hwndParent, int *pnStepX, 
            int *pnStepY, COLORREF *pCR)
{
    if(!pnStepX || !pnStepY || !pCR)
    {
        return 0;
    }

    m_pnStepX = pnStepX; 
    m_pnStepY = pnStepY; 
    m_pCR = pCR;
    m_CR = *pCR;
    return (int)DialogBox(hInstance, MAKEINTRESOURCE(GRID_SETUP_DIALOG), 
            m_hwndParent = hwndParent, GridSetupDlgProc);
}
//------------------------------------------------------------------------
INT_PTR C_GridSetupDlg::OnInitDialog(HWND hDlg)
{
    SetDlgItemInt(hDlg, ED_STEP_X, *m_pnStepX, FALSE);
    SetDlgItemInt(hDlg, ED_STEP_Y, *m_pnStepY, FALSE);

    char szWinName[256];
    char szDlgName[256];

    GetWindowText(m_hwndParent, szWinName, 255);
    sprintf(szDlgName, "Grid Setup [%s]", szWinName);
    SetWindowText(hDlg, szDlgName);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_GridSetupDlg::OnOk(HWND hDlg)
{
    OnApply(hDlg);
	EndDialog(hDlg, 1);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_GridSetupDlg::OnCancel(HWND hDlg)
{
	EndDialog(hDlg, 0);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_GridSetupDlg::OnApply(HWND hDlg)
{
    BOOL fTrans;
    *m_pnStepX = GetDlgItemInt(hDlg, ED_STEP_X, &fTrans, FALSE);
    *m_pnStepY = GetDlgItemInt(hDlg, ED_STEP_Y, &fTrans, FALSE);
    *m_pCR = m_CR;
    InvalidateRect(m_hwndParent, NULL, FALSE);
    UpdateWindow(m_hwndParent);

    return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_GridSetupDlg::OnColor(HWND hDlg)
{
    CHOOSECOLOR cc;
    
    cc.lStructSize		= sizeof(CHOOSECOLOR);
    cc.hwndOwner		= hDlg;
    cc.hInstance		= NULL;
    cc.rgbResult		= m_CR;
    cc.lpCustColors		= &m_CR;
    cc.Flags			= 0L;
    cc.lpfnHook			= NULL;
    cc.lpTemplateName	= NULL;
    if(ChooseColor(&cc))
    {
        m_CR = cc.rgbResult;
    }

    return TRUE;
}
//------------------------------------------------------------------------

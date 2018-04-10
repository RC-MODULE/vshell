//------------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include "VectorsSetupDlg.h"
#include "FrameWnd.h"
#include "resource.h"
//------------------------------------------------------------------------
C_VectorsSetupDlg VectorsSetupDlg;
extern C_FrameWnd FrameWnd;
//------------------------------------------------------------------------
INT_PTR CALLBACK VectorsSetupDlgProc(HWND hDlg, UINT msg, 
	    WPARAM wParam, 	LPARAM)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        return VectorsSetupDlg.OnInitDialog(hDlg);
    case WM_CLOSE:
        return VectorsSetupDlg.OnCancel(hDlg);
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case BTN_OK:
            return VectorsSetupDlg.OnOk(hDlg);
        case BTN_APPLY:
            return VectorsSetupDlg.OnApply(hDlg);
        case BTN_CANCEL:
            return VectorsSetupDlg.OnCancel(hDlg);
        case BTN_VECTORS_COLOR:
            return VectorsSetupDlg.OnVectorsColor(hDlg);
        }
    }
    return FALSE;
}
//------------------------------------------------------------------------
int C_VectorsSetupDlg::Show(HINSTANCE hInstance, HWND hwndParent, COLORREF *pVectorsColor)
{
    if(!pVectorsColor)
    {
        return 0;
    }

    m_pVectorsColor = pVectorsColor;
    m_VectorsColor = *pVectorsColor;
    return (int)DialogBox(hInstance, MAKEINTRESOURCE(VECTORS_SETUP_DIALOG), 
            m_hwndParent = hwndParent, VectorsSetupDlgProc);
}
//------------------------------------------------------------------------
INT_PTR C_VectorsSetupDlg::OnInitDialog(HWND hDlg)
{
    char szWinName[256];
    char szDlgName[256];

    GetWindowText(m_hwndParent, szWinName, 255);
    sprintf(szDlgName, "Vectors Setup [%s]", szWinName);
    SetWindowText(hDlg, szDlgName);

    return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_VectorsSetupDlg::OnOk(HWND hDlg)
{
    OnApply(hDlg);
	EndDialog(hDlg, 1);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_VectorsSetupDlg::OnCancel(HWND hDlg)
{
	EndDialog(hDlg, 0);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_VectorsSetupDlg::OnApply(HWND)
{
    *m_pVectorsColor = m_VectorsColor;
    InvalidateRect(m_hwndParent, NULL, FALSE);
    UpdateWindow(m_hwndParent);

    return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_VectorsSetupDlg::OnVectorsColor(HWND hDlg)
{
    CHOOSECOLOR cc;
    
    cc.lStructSize		= sizeof(CHOOSECOLOR);
    cc.hwndOwner		= hDlg;
    cc.hInstance		= NULL;
    cc.rgbResult		= m_VectorsColor;
    cc.lpCustColors		= &m_VectorsColor;
    cc.Flags			= 0L;
    cc.lpfnHook			= NULL;
    cc.lpTemplateName	= NULL;
    if(ChooseColor(&cc))
    {
        m_VectorsColor = cc.rgbResult;
    }

    return TRUE;
}
//------------------------------------------------------------------------

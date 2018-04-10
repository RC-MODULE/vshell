//------------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include "SaveSequenceAsDlg.h"
#include "FrameWnd.h"
#include "resource.h"
//------------------------------------------------------------------------
C_SaveSequenceAsDlg SaveSequenceAsDlg;
extern C_FrameWnd FrameWnd;
//------------------------------------------------------------------------
INT_PTR CALLBACK SaveSequenceAsDlgProc(HWND hDlg, UINT msg, WPARAM wParam, 	LPARAM)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		return SaveSequenceAsDlg.OnInitDialog(hDlg);
	case WM_CLOSE:
		return SaveSequenceAsDlg.OnCancel(hDlg);
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case BTN_OK:
			return SaveSequenceAsDlg.OnOk(hDlg);
		case BTN_CANCEL:
			return SaveSequenceAsDlg.OnCancel(hDlg);
		case RD_ASK_FILENAME:
		case RD_SPECIFY_FILENAME:
			return SaveSequenceAsDlg.OnRd(hDlg);
		case BTN_BROWSE:
			return SaveSequenceAsDlg.OnBrowse(hDlg);
		}
	}
	return FALSE;
}
//------------------------------------------------------------------------
int C_SaveSequenceAsDlg::Show(HINSTANCE hInstance, HWND hwndParent, char szFileName[MAX_PATH])
{
	if(!szFileName)
	{
		return 0;
	}
	m_szFileName = szFileName; 
	return (int)DialogBox(hInstance, MAKEINTRESOURCE(SAVE_SEQUENCE_AS_DIALOG), hwndParent, SaveSequenceAsDlgProc);
}
//------------------------------------------------------------------------
INT_PTR C_SaveSequenceAsDlg::OnInitDialog(HWND hDlg)
{
	SetDlgItemText(hDlg, ED_FILENAME, m_szFileName);
	CheckDlgButton(hDlg, RD_ASK_FILENAME, strlen(m_szFileName) ? BST_UNCHECKED : BST_CHECKED);
	CheckDlgButton(hDlg, RD_SPECIFY_FILENAME, strlen(m_szFileName) ? BST_CHECKED : BST_UNCHECKED);
	OnRd(hDlg);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_SaveSequenceAsDlg::OnOk(HWND hDlg)
{
	if(IsDlgButtonChecked(hDlg, RD_SPECIFY_FILENAME) == BST_CHECKED) {
		GetDlgItemText(hDlg, ED_FILENAME, m_szFileName, MAX_PATH);
	}
	else {
		m_szFileName[0] = '\0';
	}
	EndDialog(hDlg, 1);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_SaveSequenceAsDlg::OnCancel(HWND hDlg)
{
	EndDialog(hDlg, 0);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_SaveSequenceAsDlg::OnRd(HWND hDlg) 
{
	EnableWindow(GetDlgItem(hDlg, ED_FILENAME), IsDlgButtonChecked(hDlg, RD_SPECIFY_FILENAME) == BST_CHECKED);
	EnableWindow(GetDlgItem(hDlg, BTN_BROWSE), IsDlgButtonChecked(hDlg, RD_SPECIFY_FILENAME) == BST_CHECKED);
	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_SaveSequenceAsDlg::OnBrowse(HWND hDlg)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(szFileName, sizeof(szFileName));
	ofn.lStructSize         = sizeof(OPENFILENAME);
	ofn.hwndOwner           = hDlg;
	ofn.hInstance           = GetModuleHandle(NULL);
	ofn.lpstrFilter         = "AVI Files\0*.avi\0BMP Files\0*.bmp\0\0";
	ofn.lpstrFile           = szFileName;
	ofn.nMaxFile            = _MAX_PATH - 5;
	ofn.nMaxFileTitle       = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrTitle          = "Save sequence as";

	if(GetSaveFileName(&ofn)) 
	{
		if(lstrlen(szFileName) <= 4 || (lstrlen(szFileName) > 4 && lstrcmpi(&szFileName[lstrlen(szFileName) - 4], ofn.nFilterIndex == 1 ? ".avi" : ".bmp")))
			lstrcat(szFileName, ofn.nFilterIndex == 1 ? ".avi" : ".bmp");
		SetDlgItemText(hDlg, ED_FILENAME, szFileName);
	}
	return TRUE;
}
//------------------------------------------------------------------------

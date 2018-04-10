    //--------------------------------------------------------------------
#include <windows.h>
#include "AboutDlg.h"
#include "Resource.h"

    //--------------------------------------------------------------------
C_AboutDlg AboutDlg;

    //--------------------------------------------------------------------
INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM)
{
	switch(msg)
	{
	case WM_CLOSE:
		return AboutDlg.OnClose(hDlg);
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
        case IDOK:
            return AboutDlg.OnClose(hDlg);
    	}
	}
    return FALSE;
}

    //--------------------------------------------------------------------
int C_AboutDlg::Show(HWND hwndParent, HINSTANCE hInst)
{
    return (int)DialogBox(hInst, MAKEINTRESOURCE(ABOUT_VSHELL_DIALOG), hwndParent, AboutDlgProc);
}

    //--------------------------------------------------------------------
INT_PTR C_AboutDlg::OnClose(HWND hDlg)
{
    EndDialog(hDlg, 1);
    return TRUE;
}

    //--------------------------------------------------------------------

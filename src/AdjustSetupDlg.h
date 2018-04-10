#ifndef	_ADJUSTSETUPDLG_H_
#define	_ADJUSTSETUPDLG_H_
//------------------------------------------------------------------- 
#include "BitmapEx.h"
//------------------------------------------------------------------- 
class C_AdjustSetupDlg
{
private:
    HINSTANCE m_hInstance;
    HWND m_hwndParent;
    int m_nType;
    int m_pnHistogram[256];
    HWND m_hwndHisto;

public:
    int Show(HINSTANCE hInstance, HWND hwndParent);
    INT_PTR OnInitDialog(HWND hDlg);
    INT_PTR OnOk(HWND hDlg);
    INT_PTR OnCancel(HWND hDlg);
    INT_PTR OnChAdjust(HWND hDlg);
    INT_PTR OnNotify(HWND hDlg, LPARAM lp);
    INT_PTR OnSetByImg(HWND hDlg);
    INT_PTR OnSetupParamMult(HWND hDlg);
    INT_PTR OnReset(HWND hDlg, int nID);
    INT_PTR OnMouseMoveInHisto(HWND hDlg, int nX);
    INT_PTR DrawHisto();
};
//-------------------------------------------------------------------
#endif	//_ADJUSTSETUPDLG_H_
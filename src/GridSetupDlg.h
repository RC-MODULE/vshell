#ifndef	_GRIDSETUPDLG_H_
#define	_GRIDSETUPDLG_H_
//------------------------------------------------------------------- 
class C_GridSetupDlg
{
private:
    int *m_pnStepX; 
    int *m_pnStepY; 
    COLORREF *m_pCR;
    COLORREF m_CR;
    HWND m_hwndParent;
public:
    int Show(HINSTANCE hInstance, HWND hwndParent, int *pnStepX, 
            int *pnStepY, COLORREF *pCR);
    INT_PTR OnInitDialog(HWND hDlg);
    INT_PTR OnOk(HWND hDlg);
    INT_PTR OnApply(HWND hDlg);
    INT_PTR OnCancel(HWND hDlg);
    INT_PTR OnColor(HWND hDlg);
};
//-------------------------------------------------------------------
#endif	//_GRIDSETUPDLG_H_
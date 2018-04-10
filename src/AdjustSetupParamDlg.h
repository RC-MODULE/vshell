#ifndef	_ADJUSTSETUPPARAMDLG_H_
#define	_ADJUSTSETUPPARAMDLG_H_
//------------------------------------------------------------------- 
class C_AdjustSetupParamDlg
{
private:
    HWND m_hwndParent;
public:
    int Show(HINSTANCE hInstance, HWND hwndParent);
    INT_PTR OnInitDialog(HWND hDlg);
    INT_PTR OnSetByImage(HWND hDlg);
    INT_PTR OnOk(HWND hDlg);
    INT_PTR OnCancel(HWND hDlg);
};
//-------------------------------------------------------------------
#endif	//_ADJUSTSETUPPARAMDLG_H_
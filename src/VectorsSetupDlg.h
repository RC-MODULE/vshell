#ifndef	_VECTORSSETUPDLG_H_
#define	_VECTORSSETUPDLG_H_
//------------------------------------------------------------------- 
class C_VectorsSetupDlg
{
private:
    COLORREF *m_pVectorsColor;
    COLORREF m_VectorsColor;
    HWND m_hwndParent;
public:
    int Show(HINSTANCE hInstance, HWND hwndParent, COLORREF *pVectorsColor);
    INT_PTR OnInitDialog(HWND hDlg);
    INT_PTR OnOk(HWND hDlg);
    INT_PTR OnApply(HWND hDlg);
    INT_PTR OnCancel(HWND hDlg);
    INT_PTR OnVectorsColor(HWND hDlg);
};
//-------------------------------------------------------------------
#endif	//_VECTORSSETUPDLG_H_
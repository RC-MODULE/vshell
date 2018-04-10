#ifndef	_SAVESEQUENCEASDLG_H_
#define	_SAVESEQUENCEASDLG_H_
//------------------------------------------------------------------- 
class C_SaveSequenceAsDlg
{
private:
	char *m_szFileName;

public:
	int Show(HINSTANCE hInstance, HWND hwndParent, char szFileName[MAX_PATH]);
	INT_PTR OnInitDialog(HWND hDlg);
	INT_PTR OnOk(HWND hDlg);
	INT_PTR OnCancel(HWND hDlg);
	INT_PTR OnRd(HWND hDlg);
	INT_PTR OnBrowse(HWND hDlg);
};
//-------------------------------------------------------------------
#endif	//_SAVESEQUENCEINDLG_H_
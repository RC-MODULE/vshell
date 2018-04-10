    //--------------------------------------------------------------------
#ifndef _TOOLBARWND_H_
#define _TOOLBARWND_H_

    //--------------------------------------------------------------------
class C_ToolbarWnd
{
private:
    HWND m_hwndToolbar;
	HBRUSH m_hBackBrush;
    HWND m_hwndToolTip;
    HWND m_hwndParent;
    HWND m_hWnd;
    HINSTANCE m_hInstance;

public:
    C_ToolbarWnd();
    ~C_ToolbarWnd();

    BOOL RegisterWndClass(HINSTANCE hInstance);
    BOOL Create(HWND hwndParent);

    LRESULT OnCreate(HWND hWnd);
    LRESULT OnDestroy();
    LRESULT OnSize(LPARAM lp);
    LRESULT OnNotify(LPARAM lp);
    
    HWND GetParentWnd() {return m_hwndParent;}
    void SetState(int nID, BOOL fEnable, BOOL fCheck);
    void SaveRestore(HKEY hKey, char *szSubKey, char *szKeyName);
    void Move(int nX, int nY, int nWidth, int nHeight);

};

    //--------------------------------------------------------------------
#endif  // _TOOLBARWND_H_
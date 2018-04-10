//------------------------------------------------------------------------
#ifndef _CONTROLWND_H_
#define _CONTROLWND_H_
//------------------------------------------------------------------------
#include "VShell.h"
//------------------------------------------------------------------------
struct S_Slider
{
    int nID;
    HWND hwndSlider;
    HWND hwndBox;
    HWND hwndLabel;
    float fltMin;
    float fltMax;
    float fltStep;
    float fltCurValue;
    BOOL fChanged;
    int nStartYPos;
};
struct S_Radio
{
    int nID;
    HWND hWnd;
    BOOL fChecked;
};
struct S_RadioGroup
{
    int nID;
    HWND hwndBox;
    S_Radio pRadio[VS_MAX_RADIO];
    int nRadios;
    int nStartYPos;
};
struct S_CheckBox
{
    int nID;
    HWND hwndBox;
    HWND hwndCheckBox;
    int nStartYPos;
    BOOL fChecked;
};
struct S_Edit
{
    int nID;
    HWND hwndBox;
    HWND hwndEdit;
    HWND hwndLabel;
    char szText[64];
    int nStartYPos;
};
//------------------------------------------------------------------------
class C_ControlWnd
{
private:
    HWND m_hWnd;
    HINSTANCE m_hInstance;
    S_Slider m_pSlider[VS_MAX_SLIDER];
    S_RadioGroup m_pRadioGroup[VS_MAX_RADIO_GROUP];
    S_CheckBox m_pCheckBox[VS_MAX_CHECKBOX];
    S_Edit m_pEdit[VS_MAX_EDIT];
    int m_nSliders;
    int m_nRadioGroups;
    int m_nCheckBoxes;
    int m_nEdits;
    HFONT m_hCtrlFont;
    int m_CharHeight;
    int m_nNextControlPos;
    int m_nCurScrollPos;
    int m_nMaxScrollPos;
    HBRUSH m_hBackBrush;
    
    
    void SetSliderParameters(S_Slider *pSlider, float fltMin, float fltMax, 
            float fltStep, float fltStartPos);
    S_Slider *FindSlider(HWND hWnd);
    S_Slider *FindSlider(int nID);
    void ArrangeControls();
    void VMoveWindow(HWND hWnd, int nPos);
    void Update();
public:
    C_ControlWnd();
    ~C_ControlWnd();

    BOOL RegisterWndClass(HINSTANCE hInstance);
    HWND Create(HWND hParentWnd);

    BOOL CreateSlider(PSTR szName, int nID, float fltMin, float fltMax, 
            float fltStep, float fltStartPos);
    float GetSlider(int nID);
    BOOL IsSliderChanged(int nID);
    BOOL SetSlider(int nID, float fVal);
    
    BOOL CreateRadioGroup(PSTR szGroupName, int nGroupID, int nQuantity, 
            PSTR szRadioName, int *pnRadioID, int nChecked);
    BOOL IsRadioChecked(int nGroupID, int nRadioID);
    int GetCheckedID(int nGroupID);
    BOOL CheckRadio(int nGroupID, int nRadioID, BOOL fCheck);

    BOOL CreateCheckBox(PSTR szName, int nID, BOOL fCheck);
    BOOL IsCheckBoxChecked(int nID);
    BOOL CheckBox(int nID, BOOL fCheck);
	BOOL SetCheckBox(int nID, BOOL fCheck);

    BOOL CreateEdit(PSTR szName, int nID);
    BOOL GetEditText(int nID, PSTR szText);
    int GetEditInt(int nID, BOOL *pfError);
    float GetEditFloat(int nID, BOOL *pfError);
    BOOL SetEditText(int nID, PSTR szText);
    BOOL SetEditInt(int nID, int nVal);
    BOOL SetEditFloat(int nID, float fltVal);

    LRESULT OnCreate(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnSize(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnVScroll(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnClose(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT OnDestroy(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnSlider(LPARAM lp);

    void OnEditReturn(HWND hwndEdit);

    void SaveSetting();
    void LoadSetting();
    
    void Move(int nX, int nY, int nWidth, int nHeight);
    void Show(BOOL fShow);
};
//------------------------------------------------------------------------
#endif  // _CONTROLWND_H_ 
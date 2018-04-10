    //--------------------------------------------------------------------
#ifndef _SEEKSLIDER_H_
#define _SEEKSLIDER_H_

    //--------------------------------------------------------------------
#include "BitmapEx.h"

    //--------------------------------------------------------------------
class C_SeekSlider
{
private:
    C_BitmapEx *m_pBitmapEx;
    HFONT m_hCtrlFont;
    HWND m_hwndSlider;
    HWND m_hwndLabel;

public:
    HWND m_hWnd;
    HINSTANCE m_hInstance;

    C_SeekSlider();
    ~C_SeekSlider();

    BOOL RegisterWndClass(HINSTANCE hInstance);
    HWND Create(HWND hParentWnd);

    LRESULT OnCreate(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnDestroy(HWND hWnd, WPARAM wp, LPARAM lp);
    LRESULT OnSlider(LPARAM lp);

    BOOL Update(C_BitmapEx *pBitmapEx);
};

    //--------------------------------------------------------------------
#endif  // _SEEKSLIDER_H_
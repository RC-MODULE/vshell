//------------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "VShell.h"
#include "FrameWnd.h"
#include "AdjustSetupParamDlg.h"
#include "resource.h"
//------------------------------------------------------------------------
C_AdjustSetupParamDlg AdjustSetupParamDlg;
//------------------------------------------------------------------------
INT_PTR CALLBACK AdjustSetupParamDlgProc(HWND hDlg, UINT msg, 
	    WPARAM wParam, 	LPARAM)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        return AdjustSetupParamDlg.OnInitDialog(hDlg);
    case WM_CLOSE:
        return AdjustSetupParamDlg.OnCancel(hDlg);
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case BTN_SET_BY_IMAGE:
            return AdjustSetupParamDlg.OnSetByImage(hDlg);
        case BTN_OK:
            return AdjustSetupParamDlg.OnOk(hDlg);
        case BTN_CANCEL:
            return AdjustSetupParamDlg.OnCancel(hDlg);
        }
    }
    return FALSE;
}
//------------------------------------------------------------------------
int C_AdjustSetupParamDlg::Show(HINSTANCE hInstance, HWND hwndParent)
{
    return (int)DialogBox(hInstance, MAKEINTRESOURCE(ADJUST_SETUP_PARAM_DIALOG), 
            m_hwndParent = hwndParent, AdjustSetupParamDlgProc);
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupParamDlg::OnInitDialog(HWND hDlg)
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(m_hwndParent, 0);

    if(!pChData || !pChData->pBitmap)
    {
        return TRUE;
    }

    S_Adjust Adjust;

    pChData->pBitmap->GetAdjust(&Adjust);
    char szStr[40];

    sprintf(szStr, "%f", Adjust.fltMinMult);
    SetDlgItemText(hDlg, ED_ASP_MUL_MIN, szStr);
    sprintf(szStr, "%f", Adjust.fltMult);
    SetDlgItemText(hDlg, ED_ASP_MUL_CUR, szStr);
    sprintf(szStr, "%f", Adjust.fltMaxMult);
    SetDlgItemText(hDlg, ED_ASP_MUL_MAX, szStr);
    sprintf(szStr, "%f", Adjust.fltStepMult);
    SetDlgItemText(hDlg, ED_ASP_MUL_STEP, szStr);
    SetDlgItemInt(hDlg, ED_ASP_BRIG_CUR, Adjust.nBrightness, TRUE);
    
    SetDlgItemText(hDlg, LAB_OUT_MIN, "");
    SetDlgItemText(hDlg, LAB_OUT_NULL, "");
    SetDlgItemText(hDlg, LAB_OUT_MAX, "");

	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupParamDlg::OnSetByImage(HWND hDlg)
{
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(m_hwndParent, 0);
    if(!pChData || !pChData->pBitmap || !pChData->pBitmap->Data())
    {
        return FALSE;
    }

    
    int nSize = pChData->pBitmap->Width() * pChData->pBitmap->Height();
    int i, nMin, nMax;
    char *pcData;
    short *pshData;
    int *pnData;

    switch(pChData->pBitmap->Type())
    {
    case VS_RGB8_8:
        pcData = (char*)pChData->pBitmap->Data();
        nMin = nMax = pcData[0];
        for(i=1; i<nSize; i++)
        {
            if(nMin > pcData[i])
            {
                nMin = pcData[i];
            }
            if(nMax < pcData[i])
            {
                nMax = pcData[i];
            }
        }
        break;
    case VS_RGB8_16:
        pshData = (short*)pChData->pBitmap->Data();
        nMin = nMax = pshData[0];
        for(i=1; i<nSize; i++)
        {
            if(nMin > pshData[i])
            {
                nMin = pshData[i];
            }
            if(nMax < pshData[i])
            {
                nMax = pshData[i];
            }
        }
        break;
    case VS_RGB8_32:
        pnData = (int*)pChData->pBitmap->Data();
        nMin = nMax = pnData[0];
        for(i=1; i<nSize; i++)
        {
            if(nMin > pnData[i])
            {
                nMin = pnData[i];
            }
            if(nMax < pnData[i])
            {
                nMax = pnData[i];
            }
        }
        break;
    default:
        return FALSE;
    }
    
    float fltMul;
    int nBrig;
    if(nMax == nMin)
    {
        if(nMax > 0)
        {
            fltMul = 128.0f / (float)nMax;
            nBrig = -128;
        }
        else if(nMax < 0)
        {
            fltMul = -128.0f / (float)nMax;
            nBrig = 127;
        }
        else
        {
            fltMul = 1.0f;
            nBrig = 0;
        }
    }
    else if(nMax <= 0)
    {
        fltMul = -128.0f / (float)nMin;
        nBrig = 127;
    }
    else if(nMin >= 0)
    {
        fltMul = 128.0f / (float)nMax;
        nBrig = -128;
    }
    else
    {
        if(abs(nMin) < nMax)
        {
            fltMul = 128.0f / nMax;
        }
        else
        {
            fltMul = -128.0f / nMin;
        }
        nBrig = 0;
    }

    S_Adjust Adjust;

    pChData->pBitmap->GetAdjust(&Adjust);

    Adjust.fltMinMult = 0.0f;
    Adjust.fltMult = fltMul;
    Adjust.fltMaxMult = fltMul * 2.0f;
    Adjust.fltStepMult = fltMul * 2.0f / 100.0f;
    Adjust.nBrightness = nBrig;

    pChData->pBitmap->SetAdjust(&Adjust);

    char szStr[40];

    sprintf(szStr, "%f", Adjust.fltMinMult);
    SetDlgItemText(hDlg, ED_ASP_MUL_MIN, szStr);
    sprintf(szStr, "%f", Adjust.fltMult);
    SetDlgItemText(hDlg, ED_ASP_MUL_CUR, szStr);
    sprintf(szStr, "%f", Adjust.fltMaxMult);
    SetDlgItemText(hDlg, ED_ASP_MUL_MAX, szStr);
    sprintf(szStr, "%f", Adjust.fltStepMult);
    SetDlgItemText(hDlg, ED_ASP_MUL_STEP, szStr);
    SetDlgItemInt(hDlg, ED_ASP_BRIG_CUR, Adjust.nBrightness, TRUE);
    
    float fltVal;

    fltVal = (float)nMin * fltMul;
    sprintf(szStr, "%d -> %d", nMin, (int)fltVal + nBrig + 128);
    SetDlgItemText(hDlg, LAB_OUT_MIN, szStr);
    sprintf(szStr, "%d -> %d", 0, nBrig + 128);
    SetDlgItemText(hDlg, LAB_OUT_NULL, szStr);
    fltVal = (float)nMax * fltMul;
    sprintf(szStr, "%d -> %d", nMax, (int)fltVal + nBrig + 128);
    SetDlgItemText(hDlg, LAB_OUT_MAX, szStr);
    
    return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupParamDlg::OnOk(HWND hDlg)
{
    char szStr[40], *szEnd;
    S_ChildData *pChData = (S_ChildData*)GetWindowLongPtr(m_hwndParent, 0);

    if(!pChData || !pChData->pBitmap)
    {
        return TRUE;
    }

    float flMin, flMul, flMax, flStep;
    int nBrig;

    GetDlgItemText(hDlg, ED_ASP_MUL_MIN, szStr, 39);
    flMin = (float)strtod(szStr, &szEnd);
    if(szEnd[0])
    {
        SetFocus(GetDlgItem(hDlg, ED_ASP_MUL_MIN));
        MessageBeep(0);
        return TRUE;
    }

    GetDlgItemText(hDlg, ED_ASP_MUL_MAX, szStr, 39);
    flMax = (float)strtod(szStr, &szEnd);
    if(szEnd[0])
    {
        SetFocus(GetDlgItem(hDlg, ED_ASP_MUL_MAX));
        MessageBeep(0);
        return TRUE;
    }

    GetDlgItemText(hDlg, ED_ASP_MUL_STEP, szStr, 39);
    flStep = (float)strtod(szStr, &szEnd);
    if(szEnd[0])
    {
        SetFocus(GetDlgItem(hDlg, ED_ASP_MUL_STEP));
        MessageBeep(0);
        return TRUE;
    }

    GetDlgItemText(hDlg, ED_ASP_MUL_CUR, szStr, 39);
    flMul = (float)strtod(szStr, &szEnd);
    if(szEnd[0])
    {
        SetFocus(GetDlgItem(hDlg, ED_ASP_MUL_MAX));
        MessageBeep(0);
        return TRUE;
    }

    BOOL fTrans;
    nBrig = GetDlgItemInt(hDlg, ED_ASP_BRIG_CUR, &fTrans, TRUE);
    if(nBrig < -255 || nBrig > 255)
    {
        SetFocus(GetDlgItem(hDlg, ED_ASP_BRIG_CUR));
        MessageBeep(0);
        return TRUE;
    }

    if(flMul < flMin)
    {
        flMul = flMin;
    }
    else if(flMul > flMax)
    {
        flMul = flMax;
    }

    S_Adjust Adjust;
    pChData->pBitmap->GetAdjust(&Adjust);
    Adjust.fltMinMult = flMin;
    Adjust.fltMaxMult = flMax;
    Adjust.fltStepMult = flStep;
    Adjust.fltMult = flMul;
    Adjust.nBrightness = nBrig;
    pChData->pBitmap->SetAdjust(&Adjust);

	EndDialog(hDlg, 1);

	return TRUE;
}
//------------------------------------------------------------------------
INT_PTR C_AdjustSetupParamDlg::OnCancel(HWND hDlg)
{
	EndDialog(hDlg, 0);
	return TRUE;
}
//------------------------------------------------------------------------

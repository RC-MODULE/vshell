//------------------------------------------------------------------------
#pragma warning(disable:4996)
#include <windows.h>
#include <process.h>
#include <vfw.h>
#include "VShell.h"
#include "FrameWnd.h"
#include "ControlWnd.h"
#include "stdio.h"
//------------------------------------------------------------------------
static HINSTANCE hInstance;
static HANDLE hThread;
//------------------------------------------------------------------------
extern C_FrameWnd FrameWnd;
extern C_ControlWnd ControlWnd;
//------------------------------------------------------------------------
UINT WINAPI VSThreadProc(LPVOID lpVoid)
{
    FrameWnd.VSRun((HINSTANCE)lpVoid);
    return 0;
}
//------------------------------------------------------------------------
int WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID)
{
    DWORD dwThreadStatus = 0;

    switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
        hInstance = hInst;
		break;
	case DLL_PROCESS_DETACH:
        if(GetExitCodeThread(hThread, &dwThreadStatus))
	    {
		    if(dwThreadStatus == STILL_ACTIVE)
		    {
			    if(WaitForSingleObject(hThread, 3000) == WAIT_TIMEOUT)
                {
                    TerminateThread(hThread, 0);
                    CloseHandle(hThread);
                }
		    }
        }
        hThread = NULL;
		break;
    case DLL_THREAD_DETACH:
        break;
	}
	return 1;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Init()
{
    UINT nThreadID;
    int i;
    hThread = (HANDLE)_beginthreadex(NULL, 0, VSThreadProc, hInstance, 0, &nThreadID);
    for(i=0; i<10 && !FrameWnd.m_fVSRun; i++)
    {
        Sleep(100);
    }
    if(i >= 10)
    {
        return false;
    }
    return true;
}
//------------------------------------------------------------------------
void VSHELL_STDCALL VS_Release()
{
    SendMessage(FrameWnd.m_hwndFrame, WM_CLOSE, 0, 0);
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_Run()
{
    return FrameWnd.Iteration();
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Draw(int nID)
{
    return FrameWnd.Draw(nID) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_CreateImage(char *szName, int nID, int nWidth, 
        int nHeight, int nType, S_VS_Pal *prgbPalette)
{
    FrameWnd.CreateChildWindow(szName, nID, nWidth, nHeight, nType, (RGBQUAD*)prgbPalette);
    Sleep(50);
    return true;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_DeleteImage(int nID)
{
    return FrameWnd.DeleteChildWindow(nID) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Bind(char *szFileName)
{
    return FrameWnd.Bind(szFileName) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetData(int nID, void *pvData)
{
    return FrameWnd.SetData(nID, (PBYTE)pvData) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_GetData(int nID, void *pvData)
{
    return FrameWnd.GetData(nID, (PBYTE)pvData) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_GetGrayData(int nID, void *pvData)
{
    return FrameWnd.GetGrayData(nID, pvData) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_GetBGRData(int nID, void *pvData)
{
    return FrameWnd.GetBGRData(nID, pvData) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_CreateSlider(char *szName, int nID, 
        float fMin, float fMax, float fStep, float fStartPos)
{
    return ControlWnd.CreateSlider(szName, nID, fMin, fMax, fStep, fStartPos) ? true : false;
}
//------------------------------------------------------------------------
float VSHELL_STDCALL VS_GetSlider(int nID)
{
    return ControlWnd.GetSlider(nID);
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_IsSliderChanged(int nID)
{
    return ControlWnd.IsSliderChanged(nID) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetSlider(int nID, float fVal)
{
    return ControlWnd.SetSlider(nID, fVal) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_CreateRadioGroup(char *szGroupName, int nGroupID, int nQuantity, 
        char *szRadioName, int *pnRadioID, int nChecked)
{
    return ControlWnd.CreateRadioGroup(szGroupName, nGroupID, nQuantity, 
            szRadioName, pnRadioID, nChecked) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_IsRadioChecked(int nGroupID, int nRadioID)
{
    return ControlWnd.IsRadioChecked(nGroupID, nRadioID) ? true : false;
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_GetCheckedID(int nGroupID)
{
    return ControlWnd.GetCheckedID(nGroupID);
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_CheckRadio(int nGroupID, int nRadioID, bool fCheck)
{
    return ControlWnd.CheckRadio(nGroupID, nRadioID, fCheck) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_CreateCheckBox(char *szName, int nID, bool fCheck)
{
    return ControlWnd.CreateCheckBox(szName, nID, fCheck) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_GetCheckBox(int nID)
{
    return ControlWnd.IsCheckBoxChecked(nID) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetCheckBox(int nID, bool fCheck)
{
    return ControlWnd.SetCheckBox(nID, fCheck) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_CreateEdit(char *szName, int nID)
{
    return ControlWnd.CreateEdit(szName, nID) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_GetEditText(int nID, char *szText)
{
    return ControlWnd.GetEditText(nID, szText) ? true : false;
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_GetEditInt(int nID, bool *pfError)
{
    BOOL fError;
    int nRes = ControlWnd.GetEditInt(nID, &fError);
    if(pfError)
    {
        *pfError = fError ? true : false;
    }
    return nRes;
}
//------------------------------------------------------------------------
float VSHELL_STDCALL VS_GetEditFloat(int nID, bool *pfError)
{
    BOOL fError;
    float fltRes = ControlWnd.GetEditFloat(nID, &fError);
    if(pfError)
    {
        *pfError = fError ? true : false;
    }
    return fltRes;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetEditText(int nID, char *szText)
{
    return ControlWnd.SetEditText(nID, szText) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetEditInt(int nID, int nVal)
{
    return ControlWnd.SetEditInt(nID, nVal) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetEditFloat(int nID, float fltVal)
{
    return ControlWnd.SetEditFloat(nID, fltVal) ? true : false;
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_GetWidth(int nID)
{
    return FrameWnd.GetWidth(nID);
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_GetHeight(int nID)
{
    return FrameWnd.GetHeight(nID);
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_GetBitsPerPixel(int nID)
{
    return FrameWnd.GetBitsPerPixel(nID);
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_GetBufSize(int nID)
{
    return FrameWnd.GetBufSize(nID);
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_GetType(int nID)
{
    return FrameWnd.GetType(nID);
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetPalette(int nID, S_VS_Pal *prgbPal)
{
    return FrameWnd.SetPalette(nID, (RGBQUAD*)prgbPal) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_GetPalette(int nID, S_VS_Pal *prgbPal)
{
    return FrameWnd.GetPalette(nID, (RGBQUAD*)prgbPal) ? true : false;
}
//------------------------------------------------------------------------

bool VSHELL_STDCALL VS_GetMouseStatus(S_VS_MouseStatus *pMouseStatus)
{
    return FrameWnd.GetMouseStatus(&(pMouseStatus->nID), 
            &(pMouseStatus->nX), &(pMouseStatus->nY), 
            &(pMouseStatus->nScaleX), &(pMouseStatus->nScaleY), 
            &(pMouseStatus->nKey), &(pMouseStatus->nCursor)) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetVectors(int nID, int nBlockSize, S_VS_Point *pVectors, unsigned int unVectorsColor)
{
    return FrameWnd.SetVectors(nID, nBlockSize, (int*)pVectors, (COLORREF)unVectorsColor) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetVectorsF(int nID, int nBlockSize, S_VS_PointF *pVectors, unsigned int unVectorsColor)
{
    return FrameWnd.SetVectors(nID, nBlockSize, (float*)pVectors, (COLORREF)unVectorsColor) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetPixel(int nID, int nX, int nY, unsigned int crColor)
{
    return FrameWnd.SetPixel(nID, nX, nY, crColor) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Line(int nID, int nX1, int nY1, int nX2, int nY2, unsigned int crColor)
{
    return FrameWnd.Line(nID, nX1, nY1, nX2, nY2, crColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Polyline(int nID, S_VS_Point *pptPoints, int nPoints, unsigned int crColor)
{
    return FrameWnd.Polyline(nID, (POINT*)pptPoints, nPoints, crColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolyBezier(int nID, S_VS_Point *pptPoints, int nPoints, unsigned int crColor)
{
    return FrameWnd.PolyBezier(nID, (POINT*)pptPoints, nPoints, crColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Arc(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
        unsigned int crColor)
{
    return FrameWnd.Arc(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
            nXStartArc, nYStartArc, nXEndArc, nYEndArc, crColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Rectangle(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        unsigned int crBoundColor,unsigned int crFillColor)
{
    return FrameWnd.Rectangle(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
            crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Polygon(int nID, S_VS_Point *pptPoints, int nPoints, 
        unsigned int crBoundColor, unsigned int crFillColor)
{
    return FrameWnd.Polygon(nID, (POINT*)pptPoints, nPoints, crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Ellipse(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        unsigned int crBoundColor, unsigned int crFillColor)
{
    return FrameWnd.Ellipse(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
            crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Chord(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        unsigned int crBoundColor, unsigned int crFillColor)
{
    return FrameWnd.Chord(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
            nXRadial1, nYRadial1, nXRadial2, nYRadial2, crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Pie(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        unsigned int crBoundColor, unsigned int crFillColor)
{
    return FrameWnd.Pie(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
            nXRadial1, nYRadial1, nXRadial2, nYRadial2, crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetPixelF(int nID, float fltX, float fltY, unsigned int crColor)
{
    return FrameWnd.SetPixel(nID, fltX, fltY, crColor) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_LineF(int nID, float fltX1, float fltY1, float fltX2, float fltY2, unsigned int crColor)
{
    return FrameWnd.Line(nID, fltX1, fltY1, fltX2, fltY2, crColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolylineF(int nID, S_VS_PointF *pptPoints, int nPoints, unsigned int crColor)
{
    return FrameWnd.Polyline(nID, (float*)pptPoints, nPoints, crColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolyBezierF(int nID, S_VS_PointF *pptPoints, int nPoints, unsigned int crColor)
{
    return FrameWnd.PolyBezier(nID, (float*)pptPoints, nPoints, crColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_ArcF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXStartArc, float fltYStartArc, float fltXEndArc, float fltYEndArc,
        unsigned int crColor)
{
    return FrameWnd.Arc(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            fltXStartArc, fltYStartArc, fltXEndArc, fltYEndArc, crColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_RectangleF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        unsigned int crBoundColor, unsigned int crFillColor)
{
    return FrameWnd.Rectangle(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolygonF(int nID, S_VS_PointF *pptPoints, int nPoints, 
        unsigned int crBoundColor, unsigned int crFillColor)
{
    return FrameWnd.Polygon(nID, (float*)pptPoints, nPoints, crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_EllipseF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        unsigned int crBoundColor, unsigned int crFillColor)
{
    return FrameWnd.Ellipse(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_ChordF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXRadial1, float fltYRadial1,
        float fltXRadial2, float fltYRadial2,
        unsigned int crBoundColor, unsigned int crFillColor)
{
    return FrameWnd.Chord(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
            fltXRadial1, fltYRadial1, fltXRadial2, fltYRadial2, crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PieF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXRadial1, float fltYRadial1,
        float fltXRadial2, float fltYRadial2,
        unsigned int crBoundColor, unsigned int crFillColor)
{
    return FrameWnd.Pie(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
        fltXRadial1, fltYRadial1, fltXRadial2, fltYRadial2, crBoundColor, crFillColor, 0) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_TextOut(int nID, int nX, int nY, int nSize, unsigned int unColor, const char *szText, ...)
{
  	char szTmp[1024];
    va_list ap;
    va_start(ap,szText);   // начало параметров
	vsprintf(szTmp, szText,ap);
    va_end(ap);     // очистка параметров
    return FrameWnd.TextOut(nID, szTmp, nX, nY, nSize, unColor) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_Text(const char *szText, ...)
{
	char temp[1024];
    va_list ap;
    va_start(ap,szText);   // начало параметров
	vsprintf(temp,szText,ap);
    va_end(ap);     // очистка параметров
	return FrameWnd.SetText(temp) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_CorrectSignedImage(int nID)
{
    return FrameWnd.CorrectSignedImage(nID) ? true : false;
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_GetSrcFrames()
{
    return FrameWnd.GetSrcFrames();
}
//------------------------------------------------------------------------
int VSHELL_STDCALL VS_GetSrcFrameNum()
{
    return FrameWnd.GetSrcFrameNum();
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetCirclePalette(int nID, int nColorDisp)
{
    return FrameWnd.SetCirclePalette(nID, nColorDisp) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetRandomPalette(int nID)
{
    return FrameWnd.SetRandomPalette(nID) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetGrayPalette(int nID)
{
    return FrameWnd.SetGrayPalette(nID) ? true : false;
}
//------------------------------------------------------------------------
/**
\defgroup TileVerticalColor TileVerticalColor
\ingroup Its_Func_Group
\brief
\ru	клеит из двух цветных картинок одну вертикально. Высота результирующей картинки равна сумме высот плюс 4.
ширина равна максимальной ширине
\en

\~

\param pSrc1
\ru Верхняя (первая)картинка
\en 
\param nSrcWidth1
\ru ширина первой картинки
\en 
\param nSrcHeight1
\ru высота первой картинки 
\en 
\param pSrc2
\ru Нижняя (вторая)картинка 
\en 
\param nSrcWidth2
\ru ширина второй картинки
\en 
\param nSrcHeight2
\ru высота второй картинки
\en 
\param pDst
\ru Результирующая картинка
\en 
\param DelimiterColor=200
\ru цвет разделителя двух картинок
\en 
\return \e void 
*/
inline int MAX(int nVal1, int nVal2)
{
	return nVal1 >= nVal2 ? nVal1 : nVal2;
}
inline int MIN(int nVal1, int nVal2)
{
	return nVal1 <= nVal2 ? nVal1 : nVal2;
}
void VSHELL_STDCALL VS_TileVerticalColor(unsigned char *pSrc1, int nSrcWidth1, int nSrcHeight1, 
					   unsigned char *pSrc2, int nSrcWidth2, int nSrcHeight2, 
					   unsigned char *pDst, unsigned char DelimiterColor)
{
	int i, j;
	int nDstHeight=nSrcHeight1+nSrcHeight2+4;
	int nDstWidth=MAX(nSrcWidth1, nSrcWidth2);
	memset(pDst, 128, nDstWidth*nDstHeight*3*sizeof(unsigned char));
	for(i=0; i<nDstWidth*3; i++)
		pDst[nDstWidth*3*nSrcHeight1+i]=0;
	for(i=0; i<nDstWidth*3; i++)
		pDst[nDstWidth*3*(nSrcHeight1+3)+i]=0;
	for(j=nSrcHeight1+1; j<nSrcHeight1+3; j++)
		for(i=0; i<nDstWidth*3; i++)
			pDst[nDstWidth*3*j+i]=DelimiterColor;

	for(j=0; j<nSrcHeight1; j++)
		for(i=0; i<nSrcWidth1*3; i++)
		{
			pDst[nDstWidth*3*j+i]=pSrc1[nSrcWidth1*3*j+i];
		}
		for(j=0; j<nSrcHeight2; j++)
			for(i=0; i<nSrcWidth2*3; i++)
			{
				pDst[nDstWidth*3*(j+nSrcHeight1+4)+i]=pSrc2[nSrcWidth2*3*j+i];
			}
}

/**
\defgroup TileHorizontalColor TileHorizontalColor
\ingroup Its_Func_Group
\brief
\ru	клеит из двух цветных картинок одну горизонтально. Ширина результирующей картинки равна сумме ширин плюс 4.
высота равна максимальной высоте
\en

\~

\param pSrc1
\ru Левая (первая)картинка
\en 
\param nSrcWidth1
\ru ширина первой картинки
\en 
\param nSrcHeight1
\ru высота первой картинки 
\en 
\param pSrc2
\ru Правая (вторая) картинка 
\en 
\param nSrcWidth2
\ru ширина второй картинки
\en 
\param nSrcHeight2
\ru высота второй картинки
\en 
\param pDst
\ru Результирующая картинка
\en 
\param DelimiterColor=200
\ru цвет разделителя двух картинок
\en 
\return \e void 
*/

void VSHELL_STDCALL VS_TileHorizontalColor(unsigned char *pSrc1, int nSrcWidth1, int nSrcHeight1,
							unsigned char *pSrc2, int nSrcWidth2, int nSrcHeight2, 
							unsigned char *pDst , unsigned char DelimiterColor)
{
	int i, j;
	int nDstWidth=nSrcWidth1+nSrcWidth2+4;
	int nDstHeight=MAX(nSrcHeight1,nSrcHeight2);

	memset(pDst, 128, nDstWidth*nDstHeight*3*sizeof(unsigned char));
	for(j=0; j<nDstHeight; j++)
		for(i=nSrcWidth1*3; i<nSrcWidth1*3+3; i++)
			pDst[nDstWidth*3*j+i]=0;
	for(j=0; j<nDstHeight; j++)
		for(i=nSrcWidth1*3+3; i<nSrcWidth1*3+9; i++)
			pDst[nDstWidth*3*j+i]=DelimiterColor;
	for(j=0; j<nDstHeight; j++)
		for(i=nSrcWidth1*3+9; i<nSrcWidth1*3+12; i++)
			pDst[nDstWidth*3*j+i]=0;

	for(j=0; j<nDstHeight; j++)
		for(i=0; i<nSrcWidth1*3; i++)
		{
			pDst[nDstWidth*3*j+i]=pSrc1[nSrcWidth1*3*j+i];
		}
		for(j=0; j<nDstHeight; j++)
			for(i=0; i<nSrcWidth2*3; i++)
			{
				pDst[nDstWidth*3*j+i+3*(nSrcWidth1+4)]=pSrc2[nSrcWidth2*3*j+i];
			}

}

/**
		\defgroup PutFrame PutFrame
		\ingroup Its_Func_Group
		\brief
			\ru	Вставляет одну картинку в другую
			\en

		\~

		\param pROI
			\ru Вставляемая картинка
			\en 
		\param nROIWidth
			\ru Ширина вставляемой картинки
			\en 
		\param nROIHeight
			\ru Высота вставляемой картинки
			\en 
		\param nImgWidth
			\ru Ширина результирующей картинки
			\en 
		\param nROIHeight
			\ru Высота результирующей картинки
			\en 
		\param x
			\ru x-координата результирующей картинки, соответствующая верхнему левому углу вставляемой картинки
			\en 
		\param y
			\ru y-координата результирующей картинки, соответствующая верхнему левому углу вставляемой картинки 
			\en 
		\param nType
			\ru VS_RGB24 - цветные картинки, VS_RGB8 - чёрно-белые картинки
			\en 
		\retval pImg
			\ru Результирующая картинка
			\en 
		\return \e void 
*/
template <class T> bool PutFrame(T* pROI, int nROIWidth, int nROIHeight, 
								 T* pImg, int nImgWidth, int nImgHeight, int x, int y, int nType)
{
	int j;

	if(nType==VS_RGB24)
	{
		int x1=MIN(MAX(0, x), (nImgWidth-1));
		int x2=MIN(MAX(0, (x+nROIWidth-1)), (nImgWidth-1));
		int length=3*(x2-x1+1);
		for(j=MIN(MAX(0, y), nImgHeight-1); j<=MIN(MAX(0, y+nROIHeight-1), nImgHeight-1); j++)
		{
			memcpy(&pImg[3*(nImgWidth*j+x1)], &pROI[nROIWidth*3*(j-y)], length*sizeof(T));
		}
		return true;
	}
	else if(nType==VS_RGB8)
	{
		int x1=MIN(MAX(0, x), (nImgWidth-1));
		int x2=MIN(MAX(0, (x+nROIWidth-1)), (nImgWidth-1));
		int length=x2-x1+1;
		for(j=MIN(MAX(0, y), nImgHeight-1); j<=MIN(MAX(0, y+nROIHeight-1), nImgHeight-1); j++)
		{
			memcpy(&pImg[nImgWidth*j+x1], &pROI[nROIWidth*(j-y)], length*sizeof(T));
		}
		return true;
	}
	return false;
}

bool VSHELL_STDCALL VS_PutFrame(unsigned char* pFrame, int nFrameWidth, int nFrameHeight, 
				 unsigned char* pImg, int nImgWidth, int nImgHeight, int x, int y, int nType)
{
	return PutFrame(pFrame, nFrameWidth, nFrameHeight, pImg, nImgWidth, nImgHeight, x, y, nType);
}

//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_LineEx(int nID, int nX1, int nY1, int nX2, int nY2, unsigned int crColor, int nDashDot)
{
	return FrameWnd.Line(nID, nX1, nY1, nX2, nY2, crColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolylineEx(int nID, S_VS_Point *pptPoints, int nPoints, unsigned int crColor, int nDashDot)
{
	return FrameWnd.Polyline(nID, (POINT*)pptPoints, nPoints, crColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolyBezierEx(int nID, S_VS_Point *pptPoints, int nPoints, unsigned int crColor, int nDashDot)
{
	return FrameWnd.PolyBezier(nID, (POINT*)pptPoints, nPoints, crColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_ArcEx(int nID, int nLeftRect, int nTopRect, 
					 int nRightRect, int nBottomRect,
					 int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
					 unsigned int crColor, int nDashDot)
{
	return FrameWnd.Arc(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
		nXStartArc, nYStartArc, nXEndArc, nYEndArc, crColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_RectangleEx(int nID, int nLeftRect, int nTopRect, 
						   int nRightRect, int nBottomRect,
						   unsigned int crBoundColor,unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Rectangle(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
		crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolygonEx(int nID, S_VS_Point *pptPoints, int nPoints, 
						 unsigned int crBoundColor, unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Polygon(nID, (POINT*)pptPoints, nPoints, crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_EllipseEx(int nID, int nLeftRect, int nTopRect, 
						 int nRightRect, int nBottomRect,
						 unsigned int crBoundColor, unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Ellipse(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
		crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_ChordEx(int nID, int nLeftRect, int nTopRect, 
					   int nRightRect, int nBottomRect,
					   int nXRadial1, int nYRadial1,
					   int nXRadial2, int nYRadial2,
					   unsigned int crBoundColor, unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Chord(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
		nXRadial1, nYRadial1, nXRadial2, nYRadial2, crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PieEx(int nID, int nLeftRect, int nTopRect, 
					 int nRightRect, int nBottomRect,
					 int nXRadial1, int nYRadial1,
					 int nXRadial2, int nYRadial2,
					 unsigned int crBoundColor, unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Pie(nID, nLeftRect, nTopRect, nRightRect, nBottomRect,
		nXRadial1, nYRadial1, nXRadial2, nYRadial2, crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_SetPixelFEx(int nID, float fltX, float fltY, unsigned int unColor) 
{
	return FrameWnd.SetPixel(nID, fltX, fltY, unColor) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_LineFEx(int nID, float fltX1, float fltY1, float fltX2, float fltY2, unsigned int crColor, int nDashDot)
{
	return FrameWnd.Line(nID, fltX1, fltY1, fltX2, fltY2, crColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolylineFEx(int nID, S_VS_PointF *pptPoints, int nPoints, unsigned int crColor, int nDashDot)
{
	return FrameWnd.Polyline(nID, (float*)pptPoints, nPoints, crColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolyBezierFEx(int nID, S_VS_PointF *pptPoints, int nPoints, unsigned int crColor, int nDashDot)
{
	return FrameWnd.PolyBezier(nID, (float*)pptPoints, nPoints, crColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_ArcFEx(int nID, float fltLeftRect, float fltTopRect, 
					  float fltRightRect, float fltBottomRect,
					  float fltXStartArc, float fltYStartArc, float fltXEndArc, float fltYEndArc,
					  unsigned int crColor, int nDashDot)
{
	return FrameWnd.Arc(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
		fltXStartArc, fltYStartArc, fltXEndArc, fltYEndArc, crColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_RectangleFEx(int nID, float fltLeftRect, float fltTopRect, 
							float fltRightRect, float fltBottomRect,
							unsigned int crBoundColor, unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Rectangle(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
		crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PolygonFEx(int nID, S_VS_PointF *pptPoints, int nPoints, 
						  unsigned int crBoundColor, unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Polygon(nID, (float*)pptPoints, nPoints, crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_EllipseFEx(int nID, float fltLeftRect, float fltTopRect, 
						  float fltRightRect, float fltBottomRect,
						  unsigned int crBoundColor, unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Ellipse(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
		crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_ChordFEx(int nID, float fltLeftRect, float fltTopRect, 
						float fltRightRect, float fltBottomRect,
						float fltXRadial1, float fltYRadial1,
						float fltXRadial2, float fltYRadial2,
						unsigned int crBoundColor, unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Chord(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
		fltXRadial1, fltYRadial1, fltXRadial2, fltYRadial2, crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_PieFEx(int nID, float fltLeftRect, float fltTopRect, 
					  float fltRightRect, float fltBottomRect,
					  float fltXRadial1, float fltYRadial1,
					  float fltXRadial2, float fltYRadial2,
					  unsigned int crBoundColor, unsigned int crFillColor, int nDashDot)
{
	return FrameWnd.Pie(nID, fltLeftRect, fltTopRect, fltRightRect, fltBottomRect,
		fltXRadial1, fltYRadial1, fltXRadial2, fltYRadial2, crBoundColor, crFillColor, nDashDot) ? true : false;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_OpReset()
{
	FrameWnd.OnMMOReset();
	return true;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_OpRunForward()
{
	FrameWnd.OnMMORun(FALSE);
	return true;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_OpRunBackward()
{
	FrameWnd.OnMMORun(TRUE);
	return true;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_OpPause()
{
 	FrameWnd.OnMMOPause();
	return true;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_OpStepForward()
{
	FrameWnd.OnMMOStep(FALSE);
	return true;
}
//------------------------------------------------------------------------
bool VSHELL_STDCALL VS_OpStepBackward()
{
	FrameWnd.OnMMOStep(TRUE);
	return true;
}
//------------------------------------------------------------------------

void VSHELL_STDCALL VS_CreateCustomPalette(S_VS_Pal* pPalette, VS_STOP_COLOR* pStopColor, int count){
	int stopidx=0;
	int palidx=0;

	S_VS_Pal *stopcolor=(S_VS_Pal*)pStopColor;


	for(; palidx<=stopcolor[0].ucReserved; palidx++){
		pPalette[palidx]=stopcolor[stopidx];
		pPalette[palidx].ucReserved=0;
	}

	for(stopidx=1; stopidx<count; stopidx++){
		S_VS_Pal color0=stopcolor[stopidx-1];
		S_VS_Pal color1=stopcolor[stopidx];
		for(palidx=color0.ucReserved; palidx<color1.ucReserved; palidx++){
			pPalette[palidx].ucBlue = (BYTE)((palidx-color0.ucReserved)*(color1.ucBlue-color0.ucBlue) / (color1.ucReserved-color0.ucReserved)+color0.ucBlue);
			pPalette[palidx].ucGreen = (BYTE)((palidx-color0.ucReserved)*(color1.ucGreen-color0.ucGreen) / (color1.ucReserved-color0.ucReserved)+color0.ucGreen);
			pPalette[palidx].ucRed =	(BYTE)((palidx-color0.ucReserved)*(color1.ucRed-color0.ucRed) / (color1.ucReserved-color0.ucReserved)+color0.ucRed);
		}
	}

	for( palidx=stopcolor[count-1].ucReserved; palidx<256; palidx++){
		pPalette[palidx]= stopcolor[count-1];
	}
}
//------------------------------------------------------------------------

void VSHELL_STDCALL VS_Seek(int nFrameNum) {
	FrameWnd.Seek(nFrameNum);
}
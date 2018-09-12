//------------------------------------------------------------------------
//
//  Workfile   ::VShell.h
//
//  VSHELL
//
//  Copyright (c) RC Module Inc. 2005
//
//	Author Sergey Landyshev
//------------------------------------------------------------------------
#ifndef _VSHELL_H_
#define _VSHELL_H_

#define VSHELL_STDCALL __stdcall

#define VS_RGB1 1
#define VS_RGB4 2
#define VS_RGB8 3
#define VS_RGB16 4
#define VS_RGB24 5
#define VS_RGB32 6
#define VS_RGB8_8 7
#define VS_RGB8_16 8
#define VS_RGB8_32 9

#define VS_CLOSE 0
#define VS_FORWARD 1
#define VS_BACK 2
#define VS_STEP 4
#define VS_PAUSE 8

#define VS_MAX_SLIDER 32
#define VS_MAX_RADIO_GROUP 16
#define VS_MAX_RADIO 16
#define VS_MAX_CHECKBOX 32
#define VS_MAX_EDIT 32

#define VS_DRAW_ALL 0x80000000
#define VS_SOURCE 0x80000000

#define VS_MOUSE_LBUTTON 0x01
#define VS_MOUSE_RBUTTON 0x02
#define VS_MOUSE_SHIFT 0x04
#define VS_MOUSE_CONTROL 0x08
#define VS_MOUSE_MBUTTON 0x10

#define VS_CURSOR_ARROW 1
#define VS_CURSOR_MAGNIFY 2
#define VS_CURSOR_DRAG 3

#define VS_NULL_COLOR 0x80000000 

#define VS_NULL_VEC 0x80000000
#define VS_DEF_VEC_COLOR 0x80000000

#define VS_DASH 1
#define VS_DOT 2

#ifndef RGB
#define RGB(r,g,b)  (((r) & 0xFF) | (((g) & 0xFF)<<8) | (((b) & 0xFF)<<16))
#endif

#define RGB2BGR(wikiColor) ((((wikiColor) & 0xFF)<<16) | (((wikiColor) & 0xFF00)) | (((wikiColor) & 0xFF0000)>>16))
// http://ru.wikipedia.org/wiki/¬икипеди€:“аблица_цветов
#define VS_RED			RGB2BGR(0xFF0000)
#define VS_ORANGERED	RGB2BGR(0xFF4500)
#define VS_ORANGE		RGB2BGR(0xFFA500)
#define VS_GOLD			RGB2BGR(0xFFD700)
#define VS_YELLOW		RGB2BGR(0xFFFF00)
#define VS_YELLOWGREEN	RGB2BGR(0x9ACD32)
#define VS_GREENYELLOW	RGB2BGR(0xADFF2F)
#define VS_GREEN		RGB2BGR(0x008000)
#define VS_LIME			RGB2BGR(0x00FF00)
#define VS_LIMEGREEN	RGB2BGR(0x32CD32)
#define VS_CYAN			RGB2BGR(0x00FFFF)
#define VS_DARKBLUE		RGB2BGR(0x00008B)
#define VS_BLUE			RGB2BGR(0x0000FF)
#define VS_BLUEVIOLET	RGB2BGR(0x8A2BE2)
#define VS_PURPLE		RGB2BGR(0x800080)
#define VS_MAGENTA		RGB2BGR(0xFF00FF)
#define VS_BLACK		RGB2BGR(0x000000)
#define VS_WHITE		RGB2BGR(0xFFFFFF)
#define VS_GRAY			RGB2BGR(0x808080)


#define VS_MAKE_STOP_COLOR(stop,color) (((stop)<<24)|(color))
#define VS_STOP_COLOR unsigned int 

typedef struct _S_VS_MouseStatus
{
    int nID;
    int nX;
    int nY;
    int nScaleX;
    int nScaleY;
    int nKey;
    int nCursor;
} S_VS_MouseStatus;

typedef struct _S_VS_Pal
{
    unsigned char ucBlue;
    unsigned char ucGreen;
    unsigned char ucRed;
    unsigned char ucReserved;
} S_VS_Pal;

typedef struct _S_VS_Point
{
    int nX;
    int nY;
} S_VS_Point;

typedef struct _S_VS_PointF
{
    float fltX;
    float fltY;
} S_VS_PointF;









//------------------------------------------------------------------------
#ifndef __cplusplus
#ifndef bool
typedef int bool;
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
        
bool VSHELL_STDCALL VS_Init();
void VSHELL_STDCALL VS_Release();
int VSHELL_STDCALL VS_Run();
bool VSHELL_STDCALL VS_Bind(char *szFileName);
bool VSHELL_STDCALL VS_CreateImage(char *szName, int nID, int nWidth, 
        int nHeight, int nType, S_VS_Pal *pPalette);
bool VSHELL_STDCALL VS_DeleteImage(int nID);
bool VSHELL_STDCALL VS_Draw(int nID);
bool VSHELL_STDCALL VS_SetData(int nID, void *pvData);
bool VSHELL_STDCALL VS_GetData(int nID, void *pvData);
bool VSHELL_STDCALL VS_GetGrayData(int nID, void *pvData);
bool VSHELL_STDCALL VS_GetBGRData(int nID, void *pvData);
int VSHELL_STDCALL VS_GetWidth(int nID);
int VSHELL_STDCALL VS_GetHeight(int nID);
int VSHELL_STDCALL VS_GetType(int nID);
int VSHELL_STDCALL VS_GetBitsPerPixel(int nID);
int VSHELL_STDCALL VS_GetBufSize(int nID);
bool VSHELL_STDCALL VS_SetPalette(int nID, S_VS_Pal *pPal);
bool VSHELL_STDCALL VS_GetPalette(int nID, S_VS_Pal *pPal);
bool VSHELL_STDCALL VS_Text(const char *szText,...);
bool VSHELL_STDCALL VS_CreateSlider(char *szName, int nID, float fltMin, 
        float fltMax, float fltStep, float fltStartPos);
float VSHELL_STDCALL VS_GetSlider(int nID);
bool VSHELL_STDCALL VS_IsSliderChanged(int nID);
bool VSHELL_STDCALL VS_SetSlider(int nID, float fltVal);
bool VSHELL_STDCALL VS_CreateRadioGroup(char *szGroupName, int nGroupID, int nQuantity, 
        char *szRadioName, int *pnRadioID, int nChecked);
bool VSHELL_STDCALL VS_IsRadioChecked(int nGroupID, int nRadioID);
int VSHELL_STDCALL VS_GetCheckedID(int nGroupID);
bool VSHELL_STDCALL VS_CheckRadio(int nGroupID, int nRadioID, bool fCheck);
bool VSHELL_STDCALL VS_CreateCheckBox(char *szName, int nID, bool fCheck);
bool VSHELL_STDCALL VS_GetCheckBox(int nID);
bool VSHELL_STDCALL VS_SetCheckBox(int nID, bool fCheck);
bool VSHELL_STDCALL VS_CreateEdit(char *szName, int nID);
bool VSHELL_STDCALL VS_GetEditText(int nID, char *szText);
int VSHELL_STDCALL VS_GetEditInt(int nID, bool *pfError);
float VSHELL_STDCALL VS_GetEditFloat(int nID, bool *pfError);
bool VSHELL_STDCALL VS_SetEditText(int nID, char *szText);
bool VSHELL_STDCALL VS_SetEditInt(int nID, int nVal);
bool VSHELL_STDCALL VS_SetEditFloat(int nID, float fltVal);
bool VSHELL_STDCALL VS_SetPixel(int nID, int nX, int nY, unsigned int crColor);
bool VSHELL_STDCALL VS_Line(int nID, int nX1, int nY1, int nX2, int nY2, unsigned int crColor);
bool VSHELL_STDCALL VS_Polyline(int nID, S_VS_Point *pPoints, int nPoints, unsigned int unColor);
bool VSHELL_STDCALL VS_PolyBezier(int nID, S_VS_Point *pPoints, int nPoints, unsigned int unColor);
bool VSHELL_STDCALL VS_Arc(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
        unsigned int unColor);
bool VSHELL_STDCALL VS_Rectangle(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        unsigned int unBoundColor, unsigned int unFillColor);
bool VSHELL_STDCALL VS_Polygon(int nID, S_VS_Point *pPoints, int nPoints, 
        unsigned int unBoundColor, unsigned int unFillColor);
bool VSHELL_STDCALL VS_Ellipse(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        unsigned int unBoundColor, unsigned int unFillColor);
bool VSHELL_STDCALL VS_Chord(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        unsigned int unBoundColor, unsigned int unFillColor);
bool VSHELL_STDCALL VS_Pie(int nID, int nLeftRect, int nTopRect, 
        int nRightRect, int nBottomRect,
        int nXRadial1, int nYRadial1,
        int nXRadial2, int nYRadial2,
        unsigned int unBoundColor, unsigned int unFillColor);

bool VSHELL_STDCALL VS_SetPixelF(int nID, float fltX, float fltY, unsigned int unColor);
bool VSHELL_STDCALL VS_LineF(int nID, float fltX1, float fltY1, float fltX2, float fltY2, unsigned int unColor);
bool VSHELL_STDCALL VS_PolylineF(int nID, S_VS_PointF *pPoints, int nPoints, unsigned int unColor);
bool VSHELL_STDCALL VS_PolyBezierF(int nID, S_VS_PointF *pPoints, int nPoints, unsigned int unColor);
bool VSHELL_STDCALL VS_ArcF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXStartArc, float fltYStartArc, float fltXEndArc, float fltYEndArc,
        unsigned int unColor);
bool VSHELL_STDCALL VS_RectangleF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        unsigned int unBoundColor, unsigned int unFillColor);
bool VSHELL_STDCALL VS_PolygonF(int nID, S_VS_PointF *pPoints, int nPoints, 
        unsigned int unBoundColor, unsigned int unFillColor);
bool VSHELL_STDCALL VS_EllipseF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        unsigned int unBoundColor, unsigned int unFillColor);
bool VSHELL_STDCALL VS_ChordF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXRadial1, float fltYRadial1,
        float fltXRadial2, float fltYRadial2,
        unsigned int unBoundColor, unsigned int unFillColor);
bool VSHELL_STDCALL VS_PieF(int nID, float fltLeftRect, float fltTopRect, 
        float fltRightRect, float fltBottomRect,
        float fltXRadial1, float fltYRadial1,
        float fltXRadial2, float fltYRadial2,
        unsigned int unBoundColor, unsigned int unFillColor);
bool VSHELL_STDCALL VS_TextOut(int nID, int nX, int nY, int nSize, unsigned int unColor, const char *szText, ...);
bool VSHELL_STDCALL VS_GetMouseStatus(S_VS_MouseStatus *pMouseStatus);
bool VSHELL_STDCALL VS_SetVectors(int nID, int nBlockSize, S_VS_Point *pVectors, unsigned int unVectorsColor);
bool VSHELL_STDCALL VS_SetVectorsF(int nID, int nBlockSize, S_VS_PointF *pVectors, unsigned int unVectorsColor);
bool VSHELL_STDCALL VS_CorrectSignedImage(int nID);

int VSHELL_STDCALL VS_GetSrcFrames();
int VSHELL_STDCALL VS_GetSrcFrameNum();

bool VSHELL_STDCALL VS_SetCirclePalette(int nID, int nColorDisp);
bool VSHELL_STDCALL VS_SetRandomPalette(int nID);
bool VSHELL_STDCALL VS_SetGrayPalette(int nID);

// ‘ункци€ создает кусочно-линейную палитру pPalette, интерполированную по массиву опорных точек pStopColor
// S_VS_Pal* pPalette - генерируема€ палитра
// S_VS_STOPCOLOR* pStopColor - массив опорных цветов
// 		S_VS_STOPCOLOR - 32р. слово сото€щее из четрых байтов:
// 		[0:7]	blue
// 		[8:15]	green
// 		[16:23]	red
// 		[24:31]	index
// int count - длина массива pStopColor 
// значени€ палитры в диапазоне от 0 до pStopColor[0].index , заполн€етс€ значением pStopColor[0].rgb
// значени€ палитры в диапазоне от pStopColor[count-1].index до 255 , заполн€етс€ значением pStopColor[count-1].rgb
void VSHELL_STDCALL VS_CreateCustomPalette(S_VS_Pal* pPalette, VS_STOP_COLOR* pStopColor, int count);

void VSHELL_STDCALL VS_TileVerticalColor(unsigned char *pSrc1, int nSrcWidth1, int nSrcHeight1, 
		unsigned char *pSrc2, int nSrcWidth2, int nSrcHeight2, 
		unsigned char *pDst, unsigned char DelimiterColor);
void VSHELL_STDCALL VS_TileHorizontalColor(unsigned char *pSrc1, int nSrcWidth1, int nSrcHeight1,
		unsigned char *pSrc2, int nSrcWidth2, int nSrcHeight2, 
		unsigned char *pDst , unsigned char DelimiterColor);
bool VSHELL_STDCALL VS_PutFrame(unsigned char* pFrame, int nFrameWidth, int nFrameHeight, 
 						unsigned char* pImg, int nImgWidth, int nImgHeight, int x, int y, int nType);


	/* Auxiliary functions at Sergey Vaganov desire ----------------------------*/
bool VSHELL_STDCALL VS_LineEx(int nID, int nX1, int nY1, int nX2, int nY2, unsigned int crColor, int nDashDot);
bool VSHELL_STDCALL VS_PolylineEx(int nID, S_VS_Point *pPoints, int nPoints, unsigned int unColor, int nDashDot);
bool VSHELL_STDCALL VS_PolyBezierEx(int nID, S_VS_Point *pPoints, int nPoints, unsigned int unColor, int nDashDot);
bool VSHELL_STDCALL VS_ArcEx(int nID, int nLeftRect, int nTopRect, 
					 int nRightRect, int nBottomRect,
					 int nXStartArc, int nYStartArc, int nXEndArc, int nYEndArc,
					 unsigned int unColor, int nDashDot);
bool VSHELL_STDCALL VS_RectangleEx(int nID, int nLeftRect, int nTopRect, 
						   int nRightRect, int nBottomRect,
						   unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);
bool VSHELL_STDCALL VS_PolygonEx(int nID, S_VS_Point *pPoints, int nPoints, 
						 unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);
bool VSHELL_STDCALL VS_EllipseEx(int nID, int nLeftRect, int nTopRect, 
						 int nRightRect, int nBottomRect,
						 unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);
bool VSHELL_STDCALL VS_ChordEx(int nID, int nLeftRect, int nTopRect, 
					   int nRightRect, int nBottomRect,
					   int nXRadial1, int nYRadial1,
					   int nXRadial2, int nYRadial2,
					   unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);
bool VSHELL_STDCALL VS_PieEx(int nID, int nLeftRect, int nTopRect, 
					 int nRightRect, int nBottomRect,
					 int nXRadial1, int nYRadial1,
					 int nXRadial2, int nYRadial2,
					 unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);

bool VSHELL_STDCALL VS_SetPixelFEx(int nID, float fltX, float fltY, unsigned int unColor);
bool VSHELL_STDCALL VS_LineFEx(int nID, float fltX1, float fltY1, float fltX2, float fltY2, unsigned int unColor, int nDashDot);
bool VSHELL_STDCALL VS_PolylineFEx(int nID, S_VS_PointF *pPoints, int nPoints, unsigned int unColor, int nDashDot);
bool VSHELL_STDCALL VS_PolyBezierFEx(int nID, S_VS_PointF *pPoints, int nPoints, unsigned int unColor, int nDashDot);
bool VSHELL_STDCALL VS_ArcFEx(int nID, float fltLeftRect, float fltTopRect, 
					  float fltRightRect, float fltBottomRect,
					  float fltXStartArc, float fltYStartArc, float fltXEndArc, float fltYEndArc,
					  unsigned int unColor, int nDashDot);
bool VSHELL_STDCALL VS_RectangleFEx(int nID, float fltLeftRect, float fltTopRect, 
							float fltRightRect, float fltBottomRect,
							unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);
bool VSHELL_STDCALL VS_PolygonFEx(int nID, S_VS_PointF *pPoints, int nPoints, 
						  unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);
bool VSHELL_STDCALL VS_EllipseFEx(int nID, float fltLeftRect, float fltTopRect, 
						  float fltRightRect, float fltBottomRect,
						  unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);
bool VSHELL_STDCALL VS_ChordFEx(int nID, float fltLeftRect, float fltTopRect, 
						float fltRightRect, float fltBottomRect,
						float fltXRadial1, float fltYRadial1,
						float fltXRadial2, float fltYRadial2,
						unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);
bool VSHELL_STDCALL VS_PieFEx(int nID, float fltLeftRect, float fltTopRect, 
					  float fltRightRect, float fltBottomRect,
					  float fltXRadial1, float fltYRadial1,
					  float fltXRadial2, float fltYRadial2,
					  unsigned int unBoundColor, unsigned int unFillColor, int nDashDot);
bool VSHELL_STDCALL VS_OpReset();
bool VSHELL_STDCALL VS_OpRunForward();
bool VSHELL_STDCALL VS_OpRunBackward();
bool VSHELL_STDCALL VS_OpPause();
bool VSHELL_STDCALL VS_OpStepForward();
bool VSHELL_STDCALL VS_OpStepBackward();
void VSHELL_STDCALL VS_Seek(int nFrameNum);

	/*--------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif
#endif 


  

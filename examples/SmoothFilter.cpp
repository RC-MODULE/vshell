//------------------------------------------------------------------------
//  VShell example: smooth filter. 
//
//  Author: Sergey Landyshev
//
//  Copyright (c) 2005 RC Module Inc.
//------------------------------------------------------------------------

#include <windows.h>
#include <vector>
#include "VShell.h"

namespace {
void SmoothFilter(const std::vector<BYTE> &vecSrc, std::vector<BYTE> &vecDst, int nWidth, int nHeight, int nFilterSize) {
	int i, j, k, m, nInd, nTmp;

	for(i=0; i<(nHeight-nFilterSize); i++)
		for(j=0; j<(nWidth-nFilterSize); j++) {
			nInd = i * nWidth + j;
			nTmp = 0;
			for(k=0; k<nFilterSize; k++)
				for(m=0; m<nFilterSize; m++)
					nTmp += vecSrc[nInd + k * nWidth + m];
			vecDst[nInd + (nFilterSize / 2) * (nWidth + 1)] = (BYTE)(nTmp / (nFilterSize * nFilterSize));
		}
}
}

int main()
{
	if(!VS_Init())
		return 0;
	if(!VS_Bind("lena.bmp"))
		return 0;

	std::vector<BYTE> vecOriginal, vecSrc, vecDst;

	VS_CreateImage("Original", 0, VS_GetWidth(VS_SOURCE), VS_GetHeight(VS_SOURCE), VS_GetType(VS_SOURCE), NULL);
    VS_CreateImage("Smooth Filter", 1, VS_GetWidth(VS_SOURCE), VS_GetHeight(VS_SOURCE), VS_RGB8, NULL);
	VS_CreateEdit("Filter size", 0);
	VS_SetEditInt(0, 3);
    
    vecOriginal.resize(VS_GetBufSize(0));
    vecSrc.resize(VS_GetBufSize(1));
	vecDst.resize(VS_GetBufSize(1));

    VS_Text("Пример демонстрирует программу в которой заданное изображение\r\n");
    VS_Text("обрабатывается сглаживающим фильтром. В функции VS_Bind() задаётся\r\n");
    VS_Text("путь к изображению - источнику. В примере создаётся два окна:\r\n");
    VS_Text("для вывода источника и результата обработки.\r\n");
    VS_Text("Размер сглаживающего фильтра задаётся в окне ввода,\r\n");
    VS_Text("поэтому в примере демонстрируется ещё и работа с элементом управления.\r\n");
    VS_Text("Чтобы увидеть окно ввода нажмите кнопку \"Элементы управления\"\r\n");
    VS_Text("на панели инструментов.\r\n");
    VS_Text("После ввода размера фильтра нажмите клавишу <ENTER>\r\n");
    VS_Text("для того, чтобы показать VShell, что ввод окончен.\r\n");
    
	while(VS_Run()) {
		VS_GetData(VS_SOURCE, &vecOriginal[0]);
        VS_SetData(0, &vecOriginal[0]);
        VS_GetGrayData(VS_SOURCE, &vecSrc[0]);
		SmoothFilter(vecSrc, vecDst, VS_GetWidth(VS_SOURCE), VS_GetHeight(VS_SOURCE), VS_GetEditInt(0, NULL));
		VS_SetData(1, &vecDst[0]);
		VS_TextOut(1, 10, 10, 10, RGB(0, 255, 0), "Smooth filter");
		VS_Draw(VS_DRAW_ALL);
	}
    
    return 0;
}

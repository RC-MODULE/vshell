//------------------------------------------------------------------------
//  VShell example: smooth filter. 
//
//  Author: Sergey Landyshev
//
//  Copyright (c) 2005 RC Module Inc.
//------------------------------------------------------------------------

#include <windows.h>
#include "VShell.h"
#include "math.h"

int main()
{
	if(!VS_Init())
	{
		return 0;
	}
	if(!VS_Bind("lena.bmp"))
	{
		return 0;
	}

	BYTE *pbOriginal, *pbWork;

	// B G R
	
	VS_STOP_COLOR pStopColor[7]={

		VS_MAKE_STOP_COLOR(3	,VS_MAGENTA),
		VS_MAKE_STOP_COLOR(26	,VS_PURPLE),
		VS_MAKE_STOP_COLOR(64	,VS_BLUE),
		VS_MAKE_STOP_COLOR(137,VS_CYAN),
		VS_MAKE_STOP_COLOR(167,VS_LIME),
		VS_MAKE_STOP_COLOR(206,VS_YELLOW),
		VS_MAKE_STOP_COLOR(255,VS_RED),

	};
	S_VS_Pal TemperaturePalette[256];
	VS_CreateCustomPalette(TemperaturePalette,pStopColor,sizeof(pStopColor)/4);

	BYTE pbGradient[256][128];
	for(int i=0; i<256;i++){
		for(int j=0; j<128; j++){
			pbGradient[i][j]=i;
		}
	}

	VS_CreateImage("Original", 0, VS_GetWidth(VS_SOURCE), VS_GetHeight(VS_SOURCE), VS_RGB8, NULL);
    VS_CreateImage("Image colored", 1, VS_GetWidth(VS_SOURCE), VS_GetHeight(VS_SOURCE), VS_RGB8, TemperaturePalette);
	VS_CreateImage("Pallete", 2, 128, 256, VS_RGB8, TemperaturePalette);

    pbOriginal = new BYTE[VS_GetBufSize(0)];
    pbWork = new BYTE[VS_GetBufSize(1)];
              

    VS_Text("Пример демонстрирует управление палитрами \r\n");
	VS_Text("Для запуска нажмите пуск  \r\n");
    VS_Text("Для настройки палитры выведите панель управления.\r\n");
   
	
	VS_CreateSlider("VS_MAGENTA",	0, 0, 255, 1, 3		);
	VS_CreateSlider("VS_PURPLE",	1, 0, 255, 1, 26	);
	VS_CreateSlider("VS_BLUE",		2, 0, 255, 1, 64	);
	VS_CreateSlider("VS_CYAN",		3, 0, 255, 1, 137	);
	VS_CreateSlider("VS_LIME",		4, 0, 255, 1, 167	);
	VS_CreateSlider("VS_YELLOW",	5, 0, 255, 1, 206	);
	VS_CreateSlider("VS_RED",		6, 0, 255, 1, 255	);

	while(VS_Run())
	{
	
        VS_GetGrayData(VS_SOURCE, pbWork);
		VS_SetData(0, pbWork);
		VS_SetData(1, pbWork);

		pStopColor[0]=VS_MAKE_STOP_COLOR(int(VS_GetSlider(0)),VS_MAGENTA);
		pStopColor[1]=VS_MAKE_STOP_COLOR(int(VS_GetSlider(1)),VS_PURPLE);
		pStopColor[2]=VS_MAKE_STOP_COLOR(int(VS_GetSlider(2)),VS_BLUE);
		pStopColor[3]=VS_MAKE_STOP_COLOR(int(VS_GetSlider(3)),VS_CYAN);
		pStopColor[4]=VS_MAKE_STOP_COLOR(int(VS_GetSlider(4)),VS_LIME);
		pStopColor[5]=VS_MAKE_STOP_COLOR(int(VS_GetSlider(5)),VS_YELLOW);
		pStopColor[6]=VS_MAKE_STOP_COLOR(int(VS_GetSlider(6)),VS_RED);

		VS_CreateCustomPalette(TemperaturePalette,pStopColor,sizeof(pStopColor)/4);
		VS_SetPalette(1,TemperaturePalette);
		VS_SetPalette(2,TemperaturePalette);
		VS_SetData(2,pbGradient);
		
		VS_Draw(VS_DRAW_ALL);
	}
    
    delete [] pbOriginal;
    delete [] pbWork;

    return 0;
}

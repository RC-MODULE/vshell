//------------------------------------------------------------------------
//  VShell example: show image 
//
//  Author: Sergey Landyshev
//
//  Copyright (c) 2005 RC Module Inc.
//------------------------------------------------------------------------
#include <windows.h>
#include "VShell.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
int main()
{
    if(!VS_Init())
    {
        return 0;
    }
    if(!VS_Bind(NULL))
    {
        return 0;
    }

    BYTE *pbBuffer;

    VS_CreateImage("Original", 1, VS_GetWidth(VS_SOURCE), VS_GetHeight(VS_SOURCE), VS_GetType(VS_SOURCE), NULL);
    
    pbBuffer = new BYTE[VS_GetWidth(VS_SOURCE) * VS_GetHeight(VS_SOURCE) * 4];

    VS_Text("Пример демонстрирует создание\r\n");
    VS_Text("простого проигрывателя последовательностей\r\n"); 
    VS_Text("изображений и AVI файлов. В функцию VS_Bind()\r\n");
    VS_Text("передаётся NULL, что позволяет пользователю\r\n");
    VS_Text("выбрать источник после запуска приложения.\r\n");
    VS_Text("В примере создаётся окно для вывода изображений\r\n");
    VS_Text("совместимое с источником.");

    while(VS_Run())
    {
        VS_GetData(VS_SOURCE, pbBuffer);
        VS_SetData(1, pbBuffer);
        VS_Draw(VS_DRAW_ALL);
    }

    delete [] pbBuffer;

    return 0;
}
